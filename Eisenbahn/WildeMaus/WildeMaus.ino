/*
  Wilde Maus Lauflicht Wipf                 27.01.2018 bis 24.02.2018
  Überarbeitet - Erweiterung F10                           2022-08-08

  DCC Function Decoder
  Orginal Author: Ruud Boer - September 2015
  This sketch turns an Arduino into a DCC function decoder for F0 - F12
  Output pins used: 3-19 (14-19 = A0-A5)
  The DCC signal is optically separated and fed to pin 2 (=Interrupt 0).
*/

int decoderAddress = 7; // Decoder Adresse, Pin Definition
#define F0_pin 13       //      LED auf Arduino NANO
#define PIN_MOTOR 14    // A0 / F1 Motor
#define PIN_LOGO 15     // A1 / F2 WildeMaus Dauerlicht, F5 Blinken WildeMaus
#define PIN_HAUS 16     // A2 / F3 Licht 1 Häuschen
#define PIN_LATERNEN 17 // A3 / F4 Licht 2 Laternen
#define PIN_BAND_A 3    // D3 / F7 Lauflicht, F6,F7,F8,F9 verschiedene Modi   nach links 
#define PIN_BAND_B 4    // D4 / F8 Lauflicht, F6,F7,F8,F9 verschiedene Modi   nach recht
#define PIN_BAND_C 5    // D5 / F9 Lauflicht, F6,F7,F8,F9 verschiedene Modi


#include <DCC_Decoder.h>
#define kDCC_INTERRUPT 0  // ?

byte Func[4]; // 0=L F4-F1, 1=F9-F5, 2=F12-F9, 3=F20-F13, 4=F28-F21
byte instrByte1;
boolean bF0 = false;
boolean bF1 = false;
boolean bF2 = false;
boolean bF3 = false;
boolean bF4 = false;
boolean bF5 = false;
boolean bF6 = false;
boolean bF7 = false;
boolean bF8 = false;
boolean bF9 = false;
boolean bF10 = false;
int nTimerBlink = 0;
int Address;
int lauflicht;
int blinken;

boolean RawPacket_Handler(byte pktByteCount, byte* dccPacket) {
  Address = 0;
  if (!bitRead(dccPacket[0], 7))
  { // bit7=0 -> Loc Decoder Short Address
    Address = dccPacket[0];
    instrByte1 = dccPacket[1];
  } else if (bitRead(dccPacket[0], 6))
  { // bit7=1 AND bit6=1 -> Loc Decoder Long Address
    Address = 256 * (dccPacket[0] & B00000111) + dccPacket[1];
    instrByte1 = dccPacket[2];
  }

  if (Address == decoderAddress) {
    byte instructionType = instrByte1 >> 5;
    switch (instructionType)
    {
    case 4: // Loc Function L-4-3-2-1
      Func[0] = instrByte1 & B00011111;
      break;
    case 5: // Loc Function 8-7-6-5
      if (bitRead(instrByte1, 4))
      {
        Func[1] = instrByte1 & B00001111;
      } else
      { // Loc Function 12-11-10-9
        Func[2] = instrByte1 & B00001111;
      }
      break;
    }
    
    bF0 = (Func[0] & B00010000);  // F0  LED auf Arduino NANO
    bF1 = (Func[0] & B00000001);  // F1  Motor ein/aus
    bF2 = (Func[0] & B00000010);  // F2  WildeMaus Anzeige
    bF3 = (Func[0] & B00000100);  // F3  Licht 1 Häuschen
    bF4 = (Func[0] & B00001000);  // F4  Licht 2 Laternen
    bF5 = (Func[1] & B00000001);  // F5  blinken von Wilde Maus
    bF6 = (Func[1] & B00000010);  // F6  animation1 schnell / langsam
    bF7 = (Func[1] & B00000100);  // F7  animation2
    bF8 = (Func[1] & B00001000);  // F8  animation3
    bF9 = (Func[2] & B00000001);  // F9  animation4
    bF10 = (Func[2] & B00000010); // F10 Lauflicht alle Leuchtpunkte ein/aus

    //  von 0 bis 8 Steps
    if (nTimerBlink < 9){
      nTimerBlink++;
    } else{
      nTimerBlink = 0;
    }
  }
}

void doLichtband() {
  if (bF10) {          // Wenn F10 an ist, keine Animation, alle Leuchtpunkte an
    lichtband111();
  } else {
    if (bF6) {
      animation1();
    } else if (bF7) {
      animation2();
    } else if (bF8) {
      animation3();
    } else if (bF9) {
      animation4();
    }

    else {             // Weder F10 noch Animation -> Aus
      lichtband000();
    }
  }
}



void animation1() {    // F6  schnelles Lauflicht
  if (nTimerBlink == 0 || nTimerBlink == 3 || nTimerBlink == 6)
    lichtband110();
  if (nTimerBlink == 1 || nTimerBlink == 4 || nTimerBlink == 7)
    lichtband101();
  if (nTimerBlink == 2 || nTimerBlink == 5 || nTimerBlink == 8)
    lichtband011();
}

 void animation2() {    // F7 nach links, Werte mehrfach damit es langsamer wird
  if (nTimerBlink == 0)
    lichtband100();
  if (nTimerBlink == 1)
    lichtband100();
  if (nTimerBlink == 2)
    lichtband100();
  if (nTimerBlink == 3)
    lichtband010();
  if (nTimerBlink == 4)
    lichtband010();
  if (nTimerBlink == 5)
    lichtband010();
  if (nTimerBlink == 6)
    lichtband001();
  if (nTimerBlink == 7)
    lichtband001();
  if (nTimerBlink == 8)
    lichtband001();
}

void animation3() {    // F8 nach rechts, Werte mehrfach damit es langsamer wird
  if (nTimerBlink == 0)
    lichtband001();
  if (nTimerBlink == 1)
    lichtband001();
  if (nTimerBlink == 2)
    lichtband001();
  if (nTimerBlink == 3)
    lichtband010();
  if (nTimerBlink == 4)
    lichtband010();
  if (nTimerBlink == 5)
    lichtband010();
  if (nTimerBlink == 6)
    lichtband100();
  if (nTimerBlink == 7)
    lichtband100();
  if (nTimerBlink == 8)
    lichtband100();
}

void animation4() {    // F9 
  if (nTimerBlink == 0)
    lichtband110();
  if (nTimerBlink == 1)
    lichtband110();
  if (nTimerBlink == 2)
    lichtband110();
  if (nTimerBlink == 3)
    lichtband011();
  if (nTimerBlink == 4)
    lichtband011();
  if (nTimerBlink == 5)
    lichtband011();
  if (nTimerBlink == 6)
    lichtband101();
  if (nTimerBlink == 7)
    lichtband101();
  if (nTimerBlink == 8)
    lichtband101();
}

void lichtband000() {              // HIGH aus, LOW aktiv, alle aus 
  digitalWrite(PIN_BAND_A, HIGH);
  digitalWrite(PIN_BAND_B, HIGH);
  digitalWrite(PIN_BAND_C, HIGH);
}
void lichtband001() {
  digitalWrite(PIN_BAND_A, HIGH);
  digitalWrite(PIN_BAND_B, HIGH);
  digitalWrite(PIN_BAND_C, LOW);
}
void lichtband010() {
  digitalWrite(PIN_BAND_A, HIGH);
  digitalWrite(PIN_BAND_B, LOW);
  digitalWrite(PIN_BAND_C, HIGH);
}
void lichtband011() {
  digitalWrite(PIN_BAND_A, HIGH);
  digitalWrite(PIN_BAND_B, LOW);
  digitalWrite(PIN_BAND_C, LOW);
}
void lichtband100() {
  digitalWrite(PIN_BAND_A, LOW);
  digitalWrite(PIN_BAND_B, HIGH);
  digitalWrite(PIN_BAND_C, HIGH);
}
void lichtband101() {
  digitalWrite(PIN_BAND_A, LOW);
  digitalWrite(PIN_BAND_B, HIGH);
  digitalWrite(PIN_BAND_C, LOW);
}
void lichtband110() {
  digitalWrite(PIN_BAND_A, LOW);
  digitalWrite(PIN_BAND_B, LOW);
  digitalWrite(PIN_BAND_C, HIGH);
}
void lichtband111() {
  digitalWrite(PIN_BAND_A, LOW);
  digitalWrite(PIN_BAND_B, LOW);
  digitalWrite(PIN_BAND_C, LOW);
}

void doSystemLED() {               // LED auf Nano
  digitalWrite(F0_pin, bF0);
}

void doMotor() {                   // Motor
  digitalWrite(PIN_MOTOR, bF1);
}

void doHausLichter() {             // HausLichter
  digitalWrite(PIN_HAUS, bF3);
  digitalWrite(PIN_LATERNEN, bF4);
}

void doWildeMausLogo() {           // WildeMausLogo, F2 immer an 
  if (bF2) {
    digitalWrite(PIN_LOGO, HIGH);
  } else {                         // Wenn kein F2, prüfe auf F5
    if (bF5 && (nTimerBlink > 3)) {
      digitalWrite(PIN_LOGO, HIGH);
    } else {
      digitalWrite(PIN_LOGO, LOW);
    }
  }
}

void setup() {
  DCC.SetRawPacketHandler(RawPacket_Handler);
  DCC.SetupMonitor(kDCC_INTERRUPT);
  for (byte n = 3; n < 20; n++) {
    pinMode(n, OUTPUT);
  }

  // Startzustand
  digitalWrite(PIN_MOTOR, LOW);
  digitalWrite(PIN_LOGO, LOW);
  digitalWrite(PIN_HAUS, LOW);
  digitalWrite(PIN_LATERNEN, LOW);
  digitalWrite(PIN_BAND_A, HIGH);
  digitalWrite(PIN_BAND_B, HIGH);
  digitalWrite(PIN_BAND_C, HIGH);
}

void loop() {
  DCC.loop();

  doSystemLED();
  doMotor();
  doWildeMausLogo();
  doHausLichter();
  doLichtband();
}
