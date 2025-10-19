# Quick Wiring Guide - ESP32-POE-ISO Battery Tester

## Pin Connections Summary

### ESP32-POE-ISO I2C Bus
```
Pin 13 (SDA) ──┬── INA3221 #1 (0x40) SDA
               ├── INA3221 #2 (0x41) SDA  
               └── DS3231 RTC SDA

Pin 16 (SCL) ──┬── INA3221 #1 (0x40) SCL
               ├── INA3221 #2 (0x41) SCL
               └── DS3231 RTC SCL
```

**Note**: I2C bus requires 4.7kΩ pull-up resistors on both SDA and SCL lines to 3.3V.

### Relay Control Outputs
```
Pin 32 ──> Main Relay (Charge/Discharge Selector)
Pin 33 ──> AC Charger Relay
Pin 25 ──> AC Discharge Relay
Pin 26 ──> Charger Type Select Relay (Lead Acid/Lithium)
```

### Status LED Outputs
```
Pin 2  ──> Status LED (+ 220Ω resistor to ground)
Pin 27 ──> Charging Indicator LED (+ 220Ω resistor to ground)
Pin 14 ──> Discharging Indicator LED (+ 220Ω resistor to ground)
```

### Power Connections
```
ESP32-POE-ISO: USB or PoE power (5V)
INA3221 #1:    3.3V and GND from ESP32
INA3221 #2:    3.3V and GND from ESP32
DS3231:        3.3V and GND from ESP32
Relays:        12V supply (with common ground to ESP32)
```

## Current Sensing Circuit

### Charge Path
```
Battery (+) ──> [Charger] ──> [0.01Ω Shunt] ──> [Relay] ──> Battery Under Test (+)
                                    │
                              INA3221 #1 (0x40)
                              Channel 1 (IN+ / IN-)
```

### Discharge Path
```
Battery Under Test (+) ──> [Relay] ──> [0.01Ω Shunt] ──> [Discharge Load] ──> Battery (-)
                                              │
                                        INA3221 #2 (0x41)
                                        Channel 1 (IN+ / IN-)
```

### Reverse Protection
```
Each power path includes:
- Schottky Diode (150V, 10A) in series for one-way current flow
- Prevents backflow and protects INA3221 from reverse current
```

## INA3221 Module Connections

### INA3221 #1 (Charge Monitor) - Address 0x40
```
VCC  ──> 3.3V (ESP32)
GND  ──> GND (ESP32)
SDA  ──> Pin 13 (ESP32)
SCL  ──> Pin 16 (ESP32)

CH1 IN+ ──> Shunt High Side (from charger)
CH1 IN- ──> Shunt Low Side (to battery)
VS+     ──> Battery voltage measurement point
```

### INA3221 #2 (Discharge Monitor) - Address 0x41
```
VCC  ──> 3.3V (ESP32)
GND  ──> GND (ESP32)
SDA  ──> Pin 13 (ESP32)
SCL  ──> Pin 16 (ESP32)
A0   ──> 3.3V (to set address to 0x41)

CH1 IN+ ──> Shunt High Side (from battery)
CH1 IN- ──> Shunt Low Side (to load)
VS+     ──> Battery voltage measurement point
```

**Note**: To set INA3221 address to 0x41, connect A0 pin to VCC (3.3V).

## DS3231 RTC Module
```
VCC  ──> 3.3V (ESP32)
GND  ──> GND (ESP32)
SDA  ──> Pin 13 (ESP32)
SCL  ──> Pin 16 (ESP32)
SQW  ──> Not connected (optional interrupt)

Battery: CR2032 installed in DS3231 module socket
```

## Relay Wiring

### Main Relay (Pin 32) - SPDT
```
Common ──> Battery Under Test
NO     ──> Discharge Path (to discharge shunt)
NC     ──> Charge Path (from charge shunt)

Coil   ──> 12V when Pin 32 is HIGH
```

### AC Charger Relay (Pin 33) - SPST
```
Contacts: In series with AC power to charger
Coil:     12V when Pin 33 is HIGH
```

### AC Discharge Relay (Pin 25) - SPST
```
Contacts: In series with AC power to discharge station
Coil:     12V when Pin 25 is HIGH
```

### Charger Type Select (Pin 26) - SPDT
```
Common ──> From charge circuit
NO     ──> Lithium Charger Input
NC     ──> Lead Acid Charger Input

Coil   ──> 12V when Pin 26 is HIGH
```

### Relay Driver Circuit
For each relay control pin:
```
ESP32 GPIO ──> [1kΩ] ──> NPN Transistor Base (2N2222 or similar)
                          Emitter ──> GND
                          Collector ──> Relay Coil (-)
                          
Relay Coil (+) ──> +12V
Flyback Diode (1N4007) across relay coil (cathode to +12V)
```

## Shunt Resistor Specifications
```
Resistance: 0.01Ω (10 milliohms)
Power:      5W minimum (for 10A max current)
Type:       Low-inductance, current sense resistor
Tolerance:  1% or better for accuracy
```

## Safety Components

### Schottky Diodes
```
Type:     Schottky barrier diode
Voltage:  150V minimum
Current:  10A continuous
Example:  MBR10150 or similar
Location: Series with each current path
```

### Fuses
```
Charge Path:   10A fast-blow fuse
Discharge Path: 10A fast-blow fuse
Location:       Before shunt resistors
```

## Complete System Block Diagram
```
┌─────────────────────────────────────────────────────────────┐
│ ESP32-POE-ISO (Olimex)                                      │
│                                                              │
│  Pin 13 (SDA) ────┬── INA3221 #1 (0x40)                     │
│  Pin 16 (SCL) ────│   INA3221 #2 (0x41)                     │
│                   └── DS3231 RTC                             │
│                                                              │
│  Pin 32 ────> Main Relay Control                            │
│  Pin 33 ────> AC Charger Relay                              │
│  Pin 25 ────> AC Discharge Relay                            │
│  Pin 26 ────> Charger Select Relay                          │
│                                                              │
│  Pin 2  ────> Status LED                                    │
│  Pin 27 ────> Charging LED                                  │
│  Pin 14 ────> Discharge LED                                 │
└──────────────────┬───────────────────────────────────────────┘
                   │
                   │ USB (for programming & serial monitor)
                   ▼
              Computer running PuTTY


┌──────────┐    ┌──────────┐    ┌─────────┐    ┌──────────────┐
│ Charger  │───▶│  Charge  │───▶│  Main   │───▶│   Battery    │
│  (AC)    │    │  Shunt   │    │  Relay  │    │ Under Test   │
└──────────┘    └────┬─────┘    └────┬────┘    └──────┬───────┘
                     │               │                 │
                INA3221 #1           │                 │
                  (0x40)             │                 │
                                     │                 │
                                     ▼                 ▼
               ┌─────────┐    ┌──────────┐    ┌─────────────┐
               │Discharge│◀───│   Main   │◀───│   Battery   │
               │  Load   │    │  Relay   │    │ Under Test  │
               │  (AC)   │    └──────────┘    └─────────────┘
               └────┬────┘
                    │
                    │
              ┌─────▼─────┐
              │ Discharge │
              │   Shunt   │
              └─────┬─────┘
                    │
               INA3221 #2
                 (0x41)
```

## Testing Checklist Before First Power-On

- [ ] Verify all I2C connections (SDA, SCL, VCC, GND)
- [ ] Check I2C pull-up resistors are installed (4.7kΩ)
- [ ] Confirm INA3221 addresses (0x40 and 0x41)
- [ ] Verify shunt resistor values (0.01Ω)
- [ ] Check polarity of all shunt connections
- [ ] Confirm Schottky diode orientation
- [ ] Test relay coil voltages with multimeter
- [ ] Verify flyback diodes across all relay coils
- [ ] Check LED resistor values (220Ω)
- [ ] Ensure DS3231 has CR2032 battery installed
- [ ] Verify no short circuits between power rails
- [ ] Check all ground connections are secure
- [ ] Confirm AC power circuits are properly isolated
- [ ] Test emergency stop functionality
- [ ] Verify USB connection to ESP32

## Troubleshooting Quick Reference

**No I2C devices found:**
- Check SDA and SCL connections
- Verify 4.7kΩ pull-up resistors
- Ensure 3.3V power to all I2C devices

**Incorrect current readings:**
- Verify shunt resistor value (0.01Ω)
- Check shunt polarity
- Ensure good connections at shunt terminals

**Relays not switching:**
- Check transistor driver circuit
- Verify 12V relay power supply
- Test GPIO output with LED
- Check flyback diodes

**RTC time incorrect:**
- Replace CR2032 battery
- Use `settime` command after upload

---

**Safety Warning**: Double-check all connections before connecting battery or AC power. Ensure proper fusing and reverse polarity protection is in place.
