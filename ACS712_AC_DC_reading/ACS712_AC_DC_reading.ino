
#include "ACS712.h"
#include <SD.h>
#include<SPI.h>

#define currentSensor A0  // sensor connects to analog 0 
#define dcMeasure   2   // measure DC/AC when input 2 is HIGH/LOW
#define calInput    3   // offset calibration performed when input 3 = LOW

// Sensor options:  ACS712_05B = 5 amp, ACS712_20A = 20 amp, ACS712_30A = 30 amp
ACS712 sensor(ACS712_20A, currentSensor);
File ReadingsFile;

void SDInit() {
  while (!Serial) {
    ;//wait for port to connect.Needed for native USB port only
  }
  Serial.print("Initializing SD card..");
  if (!SD.begin(4)) {
    Serial.println("Initialization failed..");
    while (1);
  }
  Serial.println("Initialization DONE.");
}

void writefile() {
  //opening the file can be done once at a time.
  ReadingsFile = SD.open("AmpReadings.txt", FILE_WRITE);
  if (ReadingsFile) {
    Serial.print("writing to ampRaedings file..")
    ReadingsFile.println(analogRead(currentSensor));
  } else {
    Serial.println("error opening in AmpReadings.txt");
  }
}

void readfile() {
  //re-open the file for redaing
  ReadingsFile = SD.open("AmpReadings.txt");
  if (AmpReadings) {
    Serial.println("AmpReadings.txt opened.");
    Serial.write(ReadingsFile.read());
    //-->also we need to get a return fron here
    //-->String ampReadingString = readingsfile.read();
  }
  ReadingsFile.close();
} else {
  Serial.println("erroe opening AmpReadings.txt");
}

void setup() {
  Serial.begin(9600);
  SDInit();

  pinMode (dcMeasure, INPUT_PULLUP);
  pinMode (calInput, INPUT_PULLUP);
}

void loop() {

  float currentReading;

  if (digitalRead(calInput) == LOW) {
    // calibrates the zero point of sensor.
    // only run when there is no current flow in order to detect the zero point.
    Serial.println("Calibrating... there must be no current flow through sensor during calibration.");
    sensor.calibrate();
    Serial.println("Done!");
  }

  if (digitalRead(dcMeasure))
    currentReading = sensor.getCurrentDC();     // measure DC current
  else
    currentReading = sensor.getCurrentAC(50);   // measure AC current at specified frequency (Hz)

  Serial.print("I = ");
  Serial.print(currentReading, 3);
  Serial.print(" A");

  if (digitalRead(dcMeasure))
    Serial.println(" DC");
  else
    Serial.println(" AC");

  delay(1000);
}
