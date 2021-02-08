/*
 * Martins Klavins 10/03/2018
 * 
 * SD card - CATALEX
 * SD card connection to Arduino SPI:
 * CS   -> 10
 * SCK  -> ICSP=SCLK or 13
 * MOSI -> ICSP=MOSI or 11
 * MISO -> ICSP=MISO or 12 
 * VCC  -> 5V ( on SD module is placed AMS1117-3.3 )
 * GND  -> GND 
 * 
 * AC monitor - https://learn.openenergymonitor.org/electricity-monitoring/ctac/how-to-build-an-arduino-energy-monitor
 
 * Current sensor:
 * YHDC SCT013 100A:50mA      - current output, converted to voltage with "burden/load" resistor 33Ohm
 *                            - pin A0
 *                            - calibration coefficient 60.6
 * Voltage sensor:
 * DIY.........               - pins A1
 *                            - calibration coefficient 164.06
 */



#include <SPI.h>
#include <SD.h>
#include <EmonLib.h>             




    unsigned long previousMillis = 0, sekundes, minutes;
    float apparentPower, Vrms, Irms;

    unsigned long sekundes30 = 30000;


// Class Objects
// -------------
EnergyMonitor EMON1;
File DATALOGGER;



// initialize SD card
// ------------------
void SDkarte(){
  if (!SD.begin(10)){                                         // SD.begin(chip select pin) 
    Serial.println("SD card undetected");                    
    return;                                                  
    } 
  Serial.println("SD card detected");
  }


// write to SD card
// ----------------
void ierakstitSD(){
  DATALOGGER = SD.open("dati.txt", FILE_WRITE);
    if (DATALOGGER){
      DATALOGGER.print(minutes);
      DATALOGGER.print(":");
      DATALOGGER.print(sekundes);
      DATALOGGER.print("\t\t");
      DATALOGGER.print("V = ");
      DATALOGGER.print(Vrms);
      DATALOGGER.print("\t A = ");
      DATALOGGER.println(Irms);
      DATALOGGER.close();
    } else{
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


  
  unsigned long laiks = millis();
  
  if (laiks > 999){
    sekundes = laiks / 1000;
    if (sekundes > 59){
      minutes = sekundes / 60;
      sekundes = sekundes % 60;
      if ((minutes % 2) >= 5){
        minutes--;
        }  
      }
    }

  if (laiks - previousMillis >= sekundes30){
    previousMillis = laiks;
    ierakstitSD();
  }
  
}
