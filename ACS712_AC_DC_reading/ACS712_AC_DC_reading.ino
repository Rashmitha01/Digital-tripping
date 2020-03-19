
#include "ACS712.h"

#define sensorInput A0  // sensor connects to analog 0 
#define dcMeasure   2   // measure DC/AC when input 2 is HIGH/LOW
#define calInput    3   // offset calibration performed when input 3 = LOW

// Sensor options:  ACS712_05B = 5 amp, ACS712_20A = 20 amp, ACS712_30A = 30 amp
ACS712 sensor(ACS712_20A, sensorInput);

void setup() {
  Serial.begin(9600);

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
