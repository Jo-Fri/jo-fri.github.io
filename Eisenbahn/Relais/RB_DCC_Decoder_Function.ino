//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DCC Function Decoder
// Author: Ruud Boer - September 2015
// This sketch turns an Arduino into a DCC function decoder for F0 - F12
// Output pins used: 3-19 (14-19 = A0-A5)
// The DCC signal is optically separated and fed to pin 2 (=Interrupt 0).
// Optocoupler schematics: www.rudysmodelrailway.wordpress.com/software
// Many thanks to www.mynabay.com for publishing their DCC monitor and -decoder code.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Funktions Decoder fuer  8 Relais mit DCC Adresse 8                                                       06.10.2018
// Funktions Decoder fuer 12 Relais mit DCC Adresse 8                                                       14.10.2021
// Die Funktion F13 ok,    F14, F15 reagiert invers ???
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IMPORTANT: GOTO lines 15 - 42 to configure some data!
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int decoderAddress = 8; // DCC Decoder Adresse,
#define F0_pin 13   // F0 LED auf dem Arduino

#define F1_pin 3    // D3
#define F2_pin 4    // D4
#define F3_pin 5    // D5
#define F4_pin 6    // D6

#define F5_pin 7    // D7
#define F6_pin 8    // D8
#define F7_pin 9    // D9
#define F8_pin 10   // D10

#define F9_pin 11   // D11     
#define F10_pin 12  // D12     
#define F11_pin A0  // A0
#define F12_pin A1  // A1

#define F13_pin A2  // A2      nicht verwendet
#define F14_pin A3  // A3      nicht verwendet
#define F15_pin A4  // A4      nicht verwendet
#define F16_pin A5  // A5      nicht verwendet
/* F17-F24 commented out
  #define F17_pin 0
  #define F18_pin 0
  #define F19_pin 0
  #define F20_pin 0
  #define F21_pin 0
  #define F22_pin 0
  #define F23_pin 0
  #define F24_pin 0
*/

#include <DCC_Decoder.h>                      // Comment for Test 
#define kDCC_INTERRUPT 0

byte Func[4]; // 0=L F4-F1, 1=F8-F5, 2=F12-F9, 3=F16-F13, 4=F28-F21
byte instrByte1;
int Address;

boolean RawPacket_Handler(byte pktByteCount, byte* dccPacket) {
  Address = 0;
  if (!bitRead(dccPacket[0], 7)) { //bit7=0 -> Loc Decoder Short Address
    Address = dccPacket[0];
    instrByte1 = dccPacket[1];
  }
  else if (bitRead(dccPacket[0], 6)) { //bit7=1 AND bit6=1 -> Loc Decoder Long Address
    Address = 256 * (dccPacket[0] & B00000111) + dccPacket[1];
    instrByte1 = dccPacket[2];
  }

  if (Address == decoderAddress) {
    byte instructionType = instrByte1 >> 5;
    switch (instructionType) {
      case 4: // Loc Function L-4-3-2-1
        Func[0] = instrByte1 & B00011111;
        break;
      case 5: // Loc Function 8-7-6-5
        if (bitRead(instrByte1, 4)) {
          Func[1] = instrByte1 & B00001111;
        }
        else { // Loc Function 12-11-10-9
          Func[2] = instrByte1 & B00001111;
        }
        break;
        /* F13-F28 commented out
          case 6: // Future Expansions
          switch (instrByte1 & B00011111) {
            case B00011110: // Loc Function F13-F20
              Func[3] = dccPacket[pktByteCount - 1];
              break;
            case B00011111: // Loc Function F21-F28
              Func[4] = dccPacket[pktByteCount - 1];
              break;
          }
          break;
        */
        //F13-F28 commented out end
    }
    //  Low und High getauscht,  low ist aktiv
    if (Func[0]&B00010000) digitalWrite(F0_pin, HIGH); else digitalWrite(F0_pin, LOW);

    if (Func[0]&B00000001) digitalWrite(F1_pin, LOW); else digitalWrite(F1_pin, HIGH);
    if (Func[0]&B00000010) digitalWrite(F2_pin, LOW); else digitalWrite(F2_pin, HIGH);
    if (Func[0]&B00000100) digitalWrite(F3_pin, LOW); else digitalWrite(F3_pin, HIGH);
    if (Func[0]&B00001000) digitalWrite(F4_pin, LOW); else digitalWrite(F4_pin, HIGH);

    if (Func[1]&B00000001) digitalWrite(F5_pin, LOW); else digitalWrite(F5_pin, HIGH);
    if (Func[1]&B00000010) digitalWrite(F6_pin, LOW); else digitalWrite(F6_pin, HIGH);
    if (Func[1]&B00000100) digitalWrite(F7_pin, LOW); else digitalWrite(F7_pin, HIGH);
    if (Func[1]&B00001000) digitalWrite(F8_pin, LOW); else digitalWrite(F8_pin, HIGH);

    if (Func[2]&B00000001) digitalWrite(F9_pin, LOW); else digitalWrite(F9_pin, HIGH);
    if (Func[2]&B00000010) digitalWrite(F10_pin, LOW); else digitalWrite(F10_pin, HIGH);
    if (Func[2]&B00000100) digitalWrite(F11_pin, LOW); else digitalWrite(F11_pin, HIGH);
    if (Func[2]&B00001000) digitalWrite(F12_pin, LOW); else digitalWrite(F12_pin, HIGH);

    if (Func[3]&B00000001) digitalWrite(F13_pin, LOW); else digitalWrite(F13_pin, HIGH);
    if (Func[3]&B00000010) digitalWrite(F14_pin, LOW); else digitalWrite(F14_pin, HIGH);
    if (Func[3]&B00000100) digitalWrite(F15_pin, LOW); else digitalWrite(F15_pin, HIGH);
    if (Func[3]&B00001000) digitalWrite(F16_pin, LOW); else digitalWrite(F16_pin, HIGH);

    /*//Print DCC packet bytes for testing purposes
      for (byte n=0; n<5; n++) {
      Serial.print(Func[n],BIN);
      Serial.print(" ");
      }
      Serial.println(" ");
    */
  }
}

void setup() {
  Serial.begin(38400);
  DCC.SetRawPacketHandler(RawPacket_Handler); // Comment for Test
  DCC.SetupMonitor( kDCC_INTERRUPT );         // Comment for Test

  pinMode(F0_pin, OUTPUT);

  pinMode(F1_pin, OUTPUT);
  pinMode(F2_pin, OUTPUT);
  pinMode(F3_pin, OUTPUT);
  pinMode(F4_pin, OUTPUT);

  pinMode(F5_pin, OUTPUT);
  pinMode(F6_pin, OUTPUT);
  pinMode(F7_pin, OUTPUT);
  pinMode(F8_pin, OUTPUT);

  pinMode(F9_pin, OUTPUT);
  pinMode(F10_pin, OUTPUT);
  pinMode(F11_pin, OUTPUT);
  pinMode(F12_pin, OUTPUT);

  pinMode(F13_pin, OUTPUT);
  pinMode(F14_pin, OUTPUT);
  pinMode(F15_pin, OUTPUT);
  pinMode(F16_pin, OUTPUT);

  // Ausgange default festlegen, nach Power on auf HIGH, low ist aktiv
  digitalWrite(F0_pin, LOW);

  digitalWrite(F1_pin, HIGH);
  digitalWrite(F2_pin, HIGH);
  digitalWrite(F3_pin, HIGH);
  digitalWrite(F4_pin, HIGH);

  digitalWrite(F5_pin, HIGH);
  digitalWrite(F6_pin, HIGH);
  digitalWrite(F7_pin, HIGH);
  digitalWrite(F8_pin, HIGH);

  digitalWrite(F9_pin, HIGH);
  digitalWrite(F10_pin, HIGH);
  digitalWrite(F11_pin, HIGH);
  digitalWrite(F12_pin, HIGH);

  digitalWrite(F13_pin, HIGH);
  digitalWrite(F14_pin, HIGH);
  digitalWrite(F15_pin, HIGH);
  digitalWrite(F16_pin, HIGH);
}

void loop() {
  DCC.loop();   // Comment for Test
}
