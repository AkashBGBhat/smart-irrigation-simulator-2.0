#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define SOIL_PIN A0
#define PUMP_PIN 8   // Blue LED = pump in Wokwi

const int SOIL_DRY = 600;
const int SOIL_WET = 450;
const float TEMP_LIMIT = 32.0;

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
bool pumpOn = false;

void setup() {
  Serial.begin(9600);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.print("Smart Irrigation");
  delay(2000);
}

void loop() {
  int soilRaw = analogRead(SOIL_PIN);      // pot position = soil reading
  float tempC = dht.readTemperature();     // comes from DHT22 attrs

  bool soilDry = soilRaw > SOIL_DRY;
  bool tempOK  = (!isnan(tempC)) && (tempC < TEMP_LIMIT);

  if (!pumpOn && soilDry && tempOK)          pumpOn = true;   // dry AND cool
  else if (pumpOn && (soilRaw < SOIL_WET || !tempOK)) pumpOn = false;

  digitalWrite(PUMP_PIN, pumpOn ? HIGH : LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Soil: ");
  lcd.print(soilRaw);
  lcd.print(soilDry ? " DRY" : " WET");

  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(isnan(tempC) ? "--" : String(tempC, 1));
  lcd.print((char)223);
  lcd.print("C ");
  lcd.print(pumpOn ? "PUMP ON" : "PUMP OFF");

  delay(2000);
}
