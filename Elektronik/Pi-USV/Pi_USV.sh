#!/bin/bash
#
# 
# Pi_USV.sh      
# Raspberry Pi USV selber bauen                                                                                                 2024-08-30 JoFri
# Der Status wird über einen Optokoppler an den Raspberry Pi gemeldet (GPIO18 = wiringpi Pin1), auf LOW,  
# nach eine Wartezeit und falls das Stromnetz nicht wieder kommt, wird der Pi herunter gefahren.   
# 
# Voraussetzung WiringPi muss installiert sein
# siehe: Anleitung WiringPi installieren 
#        https://tutorials-raspberrypi.de/wiringpi-installieren-pinbelegung/   
#
# verwendete wird der Pin 1 entspricht GPIO18 
#
# Start: ./Pi_USV.sh 2>&1 | tee -a ./usv-logfile.txt
#
    echo "" 
    echo $(date +"%Y-%m-%d %T") "Starte Script: $0 "

    gpio mode 1 'in'
    if [ $? -eq 0 ]; then
        echo $(date +"%Y-%m-%d %T") "goio mode Eingang ok"
    else
        echo $(date +"%Y-%m-%d %T") "goio mode fail"	
        exit 1
    fi
    gpio mode 1 up
    if [ $? -eq 0 ]; then
        echo $(date +"%Y-%m-%d %T") "goio mode pull up Widerstand ok"
    else
        echo $(date +"%Y-%m-%d %T") "goio mode fail"
        exit 1
    fi
	
    while true
    do
	result="$( gpio read 1 )"        # Status von Pin 1,  0 ist Netz Betrieb normal, 1 ist Akku-USV Betrieb
	if [ "$result" = "1" ]; then
	    echo $(date +"%Y-%m-%d %T") "Stromausfall, Akku-USV ist nun aktiv, warte 2 Minuten und prüfe erneut"
	    sleep 120

	    result="$( gpio read 1 )"
	    if [ "$result" = "1" ]; then
	        echo $(date +"%Y-%m-%d %T") "shutdown -h 0  , 0 sofort herunterfahren"
			sudo shutdown -h 0
	        exit 0
	    else 
	    echo $(date +"%Y-%m-%d %T") "Stromausfall behoben, nicht herunterfahren"
	    fi
	fi
    sleep 2
    done	


