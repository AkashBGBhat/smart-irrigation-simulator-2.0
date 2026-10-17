# 💧 Smart Irrigation Simulator 2.0

An Arduino UNO multi-sensor irrigation system that pumps water **only when
two conditions are both true:** the soil is dry **and** the temperature is
below 32°C. Live readings, pump status, and DRY/WET state stream to an I2C
LCD — and the pump won't rapid-cycle near the threshold, thanks to a
hysteresis band.

**▶️ Try it live on Wokwi (no hardware required):** https://wokwi.com/projects/478062753740325889

**Upgraded from:** [smart-irrigation-simulator](https://github.com/AkashBGBhat/smart-irrigation-simulator) (v1: single moisture sensor, no interlocks, no display)

## What's New in 2.0

| | v1 | v2.0 |
|---|---|---|
| Pump condition | soil dry only | soil dry **AND** temp < 32°C |
| Thresholds | one (500) | two — hysteresis band (600 ON / 450 OFF) |
| Sensor health | none | `isnan()` check on DHT22 |
| Display | none | 16×2 I2C LCD with live status |
| Startup | — | splash screen |

## How It Works

Every 2 seconds, the Arduino reads soil moisture (potentiometer on A0,
0–1023) and temperature (DHT22 on D2), then decides:

```
soilDry  = reading > 600          (higher = drier in this build)
tempOK   = temp is valid AND < 32.0°C

Pump turns ON  when:  soilDry  AND  tempOK
Pump turns OFF when:  reading < 450  OR  temp not OK
```

### Why two soil thresholds?
That's a **hysteresis band** (600 → ON, 450 → OFF). With a single threshold,
soil moisture hovering right at 500 would make the pump flicker on and off
every cycle — bad for any real pump or relay. The 150-count gap forces the
soil to genuinely re-wet before the pump stops, giving stable, deliberate
switching. (This was the top improvement listed in v1's roadmap.)

### Why a temperature interlock?
Watering during peak heat risks fast evaporation and leaf scorch. The DHT22
reading must be a valid number (`isnan`-checked) and below 32°C, or the pump
refuses to run.

The blue LED on pin 8 stands in for the pump in the Wokwi simulation.

## Hardware

| Part | Role |
|---|---|
| Arduino UNO | Microcontroller board |
| Soil moisture sensor (potentiometer in sim) | Moisture reading on A0 |
| DHT22 | Temperature interlock (digital, pin 2) |
| Blue LED (pin 8) + 220Ω resistor | Represents the water pump |

## Wiring

| Part | Arduino UNO Pin |
|---|---|
| Soil pot VCC / GND / SIG | 5V / GND / A0 |
| DHT22 VCC / GND / SDA | 5V / GND / D2 |
| LCD VCC / GND / SDA / SCL | 5V / GND / A4 / A5 |
| Resistor → LED anode | D8 |
| LED cathode | GND |

The exact layout is defined in [`diagram.json`](diagram.json) — open the Wokwi
link above to see the wired circuit.

## Files

- `sketch.ino` — the Arduino program (dual-sensor reads, hysteresis + interlock logic, LCD status)
- `diagram.json` — Wokwi circuit definition (board, parts, wiring)

## Sample LCD Output

```
Soil: 720 DRY
T:28.5°C PUMP ON
```

## How to Run

**Option 1 — Wokwi (easiest):** click the link above, press ▶️ Play. Drag the
potentiometer **up** past ~600 with the DHT22 temperature **below 32°C** —
the blue pump LED lights and the LCD flips to `PUMP ON`. Then:
- Drop the pot below 450 → pump stops (hysteresis: it won't stop at 600)
- Raise the DHT22 temperature past 32°C → pump locks out even with dry soil

**Option 2 — Real hardware:** wire the circuit as above, flash the sketch with
the Arduino IDE, and watch the LCD.

## Concepts Demonstrated

- Compound boolean logic (`&&`, `||`) for multi-condition control
- Hysteresis bands to prevent output oscillation
- Sensor failure handling (`isnan()` on DHT22 reads)
- State tracking across loop iterations (`pumpOn` flag)
- I2C LCD status display with degree symbol (`(char)223`)
- I/O interlocks: one bad input blocks the actuator

## Planned Improvements

- [ ] Replace `delay()` with `millis()` for non-blocking reads (see my [weather-station-datalogger](https://github.com/AkashBGBhat/weather-station-datalogger))
- [ ] Drive an actual relay + water pump
- [ ] Capacitive soil sensor (real hardware)
- [ ] Auto mode toggle button (ON / AUTO / OFF states)
- [ ] Log watering events with timestamps
- [ ] ESP32 + Wi-Fi scheduling version

## Built With

- Arduino UNO (C++ / Arduino framework)
- [Wokwi](https://wokwi.com) — browser-based simulation
- Adafruit DHT + LiquidCrystal_I2C libraries

## Author

**Akash Bhat** — [github.com/AkashBGBhat](https://github.com/AkashBGBhat)
