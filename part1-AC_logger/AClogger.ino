/*
 * Martins Klavins 10/03/2018
 * =================================================================================================================================
 *            AC monitorings, kur uz Serial monitor izvada 'tekoshos' radijumus, bet SD karte ieraksta ik pec 30 sec
 * =================================================================================================================================
 * 
 * SD kartes modulis - CATALEX
 * SD kartes modula pieslegums pie SPI bus
 * CS   -> 10
 * SCK  -> ICSP=SCLK vai 13
 * MOSI -> ICSP=MOSI vai 11
 * MISO -> ICSP=MISO vai 12 
 * VCC  -> VCC (5V, bet pats modulis ar AMS1117 3.3 pazemina spriegumu uz 3.3V)
 * GND  -> GND 
 * 
 * AC monitorings - https://learn.openenergymonitor.org/electricity-monitoring/ctac/how-to-build-an-arduino-energy-monitor
 * (Viss ir Copy&Paste, tur ir ari viss labi aprasktits)
 * Stravas sensors:
 * YHDC SCT013 100A:50mA      - tatad current output, lai parverstu sprieguma, zimantots "burden/slodze" rezistors 33Ohm
 *                            - izmantots analogais pins 0 = A0
 *                            - kalibracijas koeficents 60.6
 * Sprieguma sensors:
 * Pashtaisits.........       - izmantots analogais pins 1 = A1
 *                            - kalibracijas koeficents 164.06
 * 
 * PS:  *Projekts veidots ta, lai to varetu palaist no foldera. Vienkarsi atver ACloggers.ino
 *      *DATI.txt jaiekope sava SD. Atbalsta SD, SDHC kartes un FAT16, FAT32 failu sistemu.
 *      *SPI.h un SD.h tiek nemts no arduino standarta library
 *      *Laiks nav precizs, jo Emon.calc() patere laiku (merijumiem izmanto noteiktu pusperiodu skaitu?! + time-out).
 * 
 */



#include <SPI.h>
#include <SD.h>
#include "EmonLib.h"             



// Global variables
// ----------------
    // taimerim
    unsigned long previousMillis = 0, sekundes, minutes;
    // AC vertibam
    float apparentPower, Vrms, Irms;


// Constants
// ---------
    // taimerim
    unsigned long sekundes30 = 30000;


// Class Objects
// -------------
EnergyMonitor EMON1;
File DATALOGGER;



// initialize SD card
// ------------------
void SDkarte(){
  if (!SD.begin(10)){                                         // SD.begin(chip select pin) 
    Serial.println("neatpazist SD karti");                    
    return;                                                  
    } 
  Serial.println("Karte atpazita");
  }


// ieraskta SD karte
// -----------------
void ierakstitSD(){
  DATALOGGER = SD.open("dati.txt", FILE_WRITE);               // tikai vienu failu var atvert vienllaicigi, ja vajag citu, šis jāver ciet
    if (DATALOGGER){                                          // ja fails atveras ok, ierakstam taja - if(true)
      DATALOGGER.print(minutes);
      DATALOGGER.print(":");
      DATALOGGER.print(sekundes);
      DATALOGGER.print("\t\t");
      DATALOGGER.print("V = ");
      DATALOGGER.print(Vrms);
      DATALOGGER.print("\t A = ");
      DATALOGGER.println(Irms);
      DATALOGGER.close();
    } else{                                                   // ja neatver failu
      Serial.println("error opening dati.txt");
    }
  }

void setup()
{  
  
  Serial.begin(115200);
  
  EMON1.voltage(1, 164.06, 1.2);                              // Voltage: input pin, calibration, phase_shift
  EMON1.current(0, 60.6);                                     // Current: input pin, calibration.

  SDkarte();
  
}

void loop()
{

  EMON1.calcVI(100,1000);                                     // Calculate all: No.of half wavelengths (crossings), time-out
  
  apparentPower   = EMON1.apparentPower;                      //extract Apparent Power into variable
  Vrms            = EMON1.Vrms;                               //extract Vrms into Variable
  Irms            = EMON1.Irms;                               //extract Irms into Variable
  
  Serial.print("V = ");
  Serial.print(Vrms);
  Serial.print("\t A = ");
  Serial.println(Irms);


  
  unsigned long laiks = millis();                             // taimerim izmanto millis()
  
  if (laiks > 999){                                           // ms sadalu pa minutem un sekundem
    sekundes = laiks / 1000;
    if (sekundes > 59){
      minutes = sekundes / 60;
      sekundes = sekundes % 60;
      if ((minutes % 2) >= 5){
        minutes--;
        }  
      }
    }

  if (laiks - previousMillis >= sekundes30){                  // taimeris, kas ik pec 30sec ieraksta merijumus SD karte
    previousMillis = laiks;
    ierakstitSD();
  }
  
}

