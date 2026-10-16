/*
 * Smart Irrigation 2.0 — Multi-Sensor Irrigation Controller
 *
 * Pump (simulated by LED) turns ON only if:
 *   - soil is dry (raw ADC > SOIL_DRY), AND
 *   - temperature is below TEMP_LIMIT (no watering in peak heat).
 *
 * Hysteresis prevents rapid on/off flicker around thresholds.
 * I2C LCD 16x2 shows live soil and temperature readings.
 *
 * Simulated in Wokwi:
 *   - Potentiometer on A0 simulates soil moisture sensor
 *   - wokwi-dht22 on D2 provides temperature
 *   - Blue LED on D8 simulates the pump
 *   - wokwi-lcd1602 (I2C, address 0x27) on A4/A5 displays readings
 */

#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define SOIL_PIN A0
#define PUMP_PIN 8   // Blue LED = pump in Wokwi; relay module in hardware

// --- Thresholds (calibrate SOIL_DRY/SOIL_WET for your sensor) ---
const int SOIL_DRY = 600;      // raw ADC: above this = dry soil
const int SOIL_WET = 450;      // hysteresis: pump stops below this
const float TEMP_LIMIT = 32.0; // deg C: no watering in peak heat

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // try 0x3F if 0x27 doesn't work

bool pumpOn = false;

void setup() {
  Serial.begin(9600);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.print("Smart Irrigation");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);
}

void loop() {
  int soilRaw = analogRead(SOIL_PIN);      // pot position = soil reading
  float tempC = dht.readTemperature();     // from DHT22 (attrs in Wokwi)

  // --- AND logic with hysteresis ---
  bool soilDry = soilRaw > SOIL_DRY;
  bool tempOK  = (!isnan(tempC)) && (tempC < TEMP_LIMIT);

  if (!pumpOn && soilDry && tempOK)                     pumpOn = true;   // dry AND cool
  else if (pumpOn && (soilRaw < SOIL_WET || !tempOK))   pumpOn = false;  // wet OR too hot

  digitalWrite(PUMP_PIN, pumpOn ? HIGH : LOW);

  // --- LCD: line 1 = soil, line 2 = temp + pump state ---
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Soil: ");
  lcd.print(soilRaw);
  lcd.print(soilDry ? " DRY" : " WET");

  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(isnan(tempC) ? "--" : String(tempC, 1));
  lcd.print((char)223);  // degree symbol
  lcd.print("C ");
  lcd.print(pumpOn ? "PUMP ON" : "PUMP OFF");

  // Serial monitor logging
  Serial.print("Soil="); Serial.print(soilRaw);
  Serial.print(" Temp="); Serial.print(tempC);
  Serial.print(" Pump="); Serial.println(pumpOn ? "ON" : "OFF");

  delay(2000);  // DHT22 needs ~2s between reads
}
