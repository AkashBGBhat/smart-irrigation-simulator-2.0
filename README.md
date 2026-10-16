# smart-irrigation-simulator-2.0
Arduino UNO multi-sensor smart irrigation system — pumps only when soil is dry AND temperature is below 32°C, with live I2C LCD readings. Simulated in Wokwi.
https://wokwi.com/projects/478062753740325889

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


      {
      "version": 1,
      "author": "Akash",
      "editor": "wokwi",
      "parts": [
        { "type": "wokwi-arduino-uno", "id": "uno", "top": 0, "left": 0, "attrs": {} },
        { "type": "wokwi-potentiometer", "id": "soilPot", "top": -170, "left": -140, "attrs": { "value": "700" } },
        { "type": "wokwi-dht22", "id": "dht1", "top": -170, "left": 60, "attrs": { "temperature": "28", "humidity": "60" } },
        { "type": "wokwi-lcd1602", "id": "lcd1", "top": -170, "left": 220, "attrs": { "pins": "i2c" } },
        { "type": "wokwi-led", "id": "pumpLed", "top": 130, "left": 290, "attrs": { "color": "blue" } },
        { "type": "wokwi-resistor", "id": "r1", "top": 130, "left": 180, "attrs": { "value": "220" } }
      ],
      "connections": [
        [ "soilPot:VCC", "uno:5V", "red", [] ],
        [ "soilPot:GND", "uno:GND.1", "black", [] ],
        [ "soilPot:SIG", "uno:A0", "green", [] ],
        [ "dht1:VCC", "uno:5V", "red", [] ],
        [ "dht1:SDA", "uno:2", "green", [] ],
        [ "dht1:GND", "uno:GND.1", "black", [] ],
        [ "lcd1:GND", "uno:GND.1", "black", [] ],
        [ "lcd1:VCC", "uno:5V", "red", [] ],
        [ "lcd1:SDA", "uno:A4", "blue", [] ],
        [ "lcd1:SCL", "uno:A5", "yellow", [] ],
        [ "uno:8", "r1:1", "orange", [] ],
        [ "r1:2", "pumpLed:A", "orange", [] ],
        [ "pumpLed:C", "uno:GND.1", "black", [] ]
      ]
    }
    
    



