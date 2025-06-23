#include <LiquidCrystal.h>
#include "EmonLib.h"
#include <math.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8); 
EnergyMonitor emon1;
const int Sensor_Pin = A1;
int sensitivity = 185;
int offsetvoltage = 2542;
const float freqNominal = 50.0;
float calculateFrequency() {
  unsigned long pulseTime = pulseIn(A0, HIGH);
  if (pulseTime > 0) {
    return 1.0 / (2.0 * pulseTime * 1e-6);
  }
  return freqNominal;
}
void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("AC Wattmeter");
  lcd.setCursor(0, 1);
  lcd.print("Using Arduino");
  delay(200);
  lcd.clear();
  emon1.voltage(A0, 187, 1.7);
  emon1.current(A1, sensitivity);
}
void loop() {
  emon1.calcVI(20, 200);
  float Vrms = emon1.Vrms;
  float Irms = emon1.Irms;
  float realPower = emon1.realPower;
  float apparentPower = Vrms * Irms;
  float powerFactor = realPower / apparentPower;
  float reactivePower = sqrt(apparentPower * apparentPower - realPower * realPower);
  float phaseAngle = acos(powerFactor) * (180.0 / M_PI);
  float capacitance = (reactivePower / (2 * M_PI * freqNominal * Vrms * Vrms)) * 1e6;
  float frequency = calculateFrequency();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(Vrms, 1);
  lcd.print(" I:");
  lcd.print(Irms, 1);
  lcd.setCursor(0, 1);
  lcd.print("F:");
  lcd.print(frequency, 1);
  lcd.print("Hz P:");
  lcd.print(realPower, 0);
  lcd.print("W");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PF:");
  lcd.print(powerFactor, 2);
  lcd.print(" Ph:");
  lcd.print(phaseAngle, 1);
  lcd.setCursor(0, 1);
  lcd.print("Q:");
  lcd.print(reactivePower, 1);
  lcd.print("VAR C:");
  lcd.print(capacitance, 0);
  lcd.print("uF");
  delay(3000);
}
