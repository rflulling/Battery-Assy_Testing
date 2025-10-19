# Battery Testing System - User Guide

## Overview
This guide provides instructions for operating the ESP32-POE-ISO Battery Testing System for monitoring battery charge and discharge cycles.

## Hardware Setup

### Required Components
1. ESP32-POE-ISO board (Olimex)
2. Two INA3221 current sensor modules
3. DS3231 RTC module
4. Relay modules (4 channels)
5. Battery charger(s)
6. Battery discharge station
7. USB cable for serial communication
8. PuTTY or similar terminal software

### I2C Connections
- **SDA**: Pin 13 on ESP32-POE-ISO
- **SCL**: Pin 16 on ESP32-POE-ISO

Connect both INA3221 modules and DS3231 to the same I2C bus:
- INA3221 #1 (Charge): Address 0x40
- INA3221 #2 (Discharge): Address 0x41
- DS3231 RTC: Address 0x68

### Relay Connections
- **Main Relay** (Pin 32): Switches between charge and discharge paths
- **AC Charger** (Pin 33): Controls AC power to charging station
- **AC Discharge** (Pin 25): Controls AC power to discharge station
- **Charger Select** (Pin 26): Switches between Lead Acid/Lithium chargers

### Current Shunt Configuration
- Install 10mΩ (0.01Ω) shunt resistors in series with battery connections
- **Charge Shunt**: Connected to INA3221 #1, Channel 1
- **Discharge Shunt**: Connected to INA3221 #2, Channel 1
- Use Schottky diodes (150V, 10A) for reverse bias protection

### Status LEDs
- **Pin 2**: General status LED
- **Pin 27**: Charging indicator
- **Pin 14**: Discharging indicator

## Software Setup

### Installing PlatformIO
```bash
pip install platformio
```

### Building the Firmware
```bash
cd Battery-Assy_Testing
pio run -e esp32-poe-iso
```

### Uploading to ESP32
```bash
pio run -e esp32-poe-iso --target upload
```

### Opening Serial Monitor
```bash
pio device monitor -b 115200
```

Or use PuTTY with the following settings:
- **Baud Rate**: 115200
- **Data Bits**: 8
- **Stop Bits**: 1
- **Parity**: None
- **Flow Control**: None

## Operation

### Initial Startup
1. Connect ESP32-POE-ISO to computer via USB
2. Open PuTTY or serial terminal
3. Reset the board if necessary
4. You should see the startup message and system information

### Available Commands

#### `help`
Displays list of available commands.

```
> help
Available Commands:
  help        - Show this help message
  status      - Show current system status
  start <n>   - Start test (0=Lead Acid, 1=LiFePO4, 2=NiCad, 3=Li-ion)
  stop        - Stop current test
  summary     - Print test summary
  time        - Show current RTC time
  settime     - Set RTC time to compile time
```

#### `status`
Shows current system status including test state and battery type.

```
> status
Current Status:
  State: Idle
  Test Active: No
  Battery Type: Lead Acid
```

#### `start <n>`
Starts a battery test cycle. Battery type codes:
- **0**: Lead Acid (~13Ah motorcycle battery)
- **1**: LiFePO4 (~90Ah battery)
- **2**: NiCad 4xAA pack
- **3**: Li-ion cell

Example:
```
> start 1
Starting test for LiFePO4 battery...
```

#### `stop`
Stops the current test and prints a summary.

```
> stop
Test stopped.
```

#### `summary`
Displays detailed test results including:
- Voltage measurements (start, end, peak)
- Charge data (duration, average, peak, total Ah/Wh)
- Discharge data (duration, average, peak, total Ah/Wh)
- Total test duration

#### `time`
Shows the current RTC time.

```
> time
RTC Time: 2025-10-19 14:30:00
```

#### `settime`
Sets the RTC to the firmware compile time. Use this after uploading new firmware.

```
> settime
RTC time set to compile time.
```

## Test Cycle Operation

### Automatic Test Sequence
1. **Precharge State**: Battery is connected to charger
   - Relays configure for charging
   - Charging LED illuminated
   - Current flows through charge shunt
   - Accumulates charge Ah and Wh
   - Continues until charge current drops below 0.1A for 60 seconds

2. **Charge Complete**: Brief transition state
   - AC power to charger is disconnected
   - System prepares for discharge

3. **Discharge Ready**: Transition to discharge
   - Relays switch to discharge configuration
   - AC power to discharge station enabled

4. **Discharging State**: Battery discharges through load
   - Discharge LED illuminated
   - Current flows through discharge shunt
   - Accumulates discharge Ah and Wh
   - Continues until voltage reaches cutoff for battery type:
     - Lead Acid: 10.5V
     - LiFePO4: 10.0V
     - NiCad: 4.0V
     - Li-ion: 2.8V

5. **Discharge Complete**: Test finished
   - All relays turned off
   - Summary automatically printed
   - CSV data logged

### Real-Time Display
During active testing, the display updates every 500ms showing:
- Current RTC date and time
- Test state
- Battery type
- Running time (HH:MM:SS)
- Voltage (current and peak)
- Charge current (real-time, average, peak)
- Discharge current (real-time, average, peak)
- Total charge (Ah and Wh)
- Total discharge (Ah and Wh)

### CSV Data Logging
Every 5 seconds during testing, CSV-formatted data is output to serial:

```
[CSV] 2025-10-19 14:30:00,LeadAcid_20251019_143000,Charging,12.450,2.350,0.000,0.125,0.000,1.512,0.000
```

CSV Format:
```
Timestamp, BatteryID, State, Voltage, ChargeCurrent, DischargeCurrent, TotalChargeAh, TotalDischargeAh, TotalChargeWh, TotalDischargeWh
```

You can redirect this to a file for later analysis.

## Safety Features

### Voltage Cutoff
The system automatically stops discharge when battery voltage reaches the safe minimum for each battery type.

### Maximum Duration
Tests automatically stop after 48 hours to prevent excessive cycling.

### Emergency Stop
Use the `stop` command at any time to immediately halt the test and disable all relays.

### Relay Safe State
On startup and during idle, all relays are set to safe states:
- Main relay in discharge position (prevents charging)
- All AC relays off

## Troubleshooting

### "Could not find DS3231 RTC"
- Check I2C connections (SDA=13, SCL=16)
- Verify RTC module power supply
- Check I2C pull-up resistors (typically 4.7kΩ)

### No Current Readings
- Verify INA3221 modules are powered
- Check I2C addresses (0x40 and 0x41)
- Ensure shunt resistors are properly connected
- Verify current is actually flowing (check battery connections)

### RTC Lost Power
- Replace CR2032 battery in DS3231 module
- Use `settime` command to reset time

### Incorrect Measurements
- Verify shunt resistor values (should be 0.01Ω)
- Check for loose connections
- Ensure proper polarity
- Calibrate if necessary

## Data Analysis

### Extracting CSV Data
To capture CSV data for analysis:

**Using PuTTY:**
1. Right-click title bar → Change Settings
2. Session → Logging
3. Select "All session output"
4. Specify log file name
5. Start logging

**Using pio device monitor:**
```bash
pio device monitor -b 115200 | tee battery_test.log
```

### Filtering CSV Data
Extract only CSV lines from log:
```bash
grep "^\[CSV\]" battery_test.log > battery_data.csv
```

### Importing to Excel/LibreOffice
1. Remove "[CSV] " prefix from lines
2. Import as CSV with comma delimiter
3. Set appropriate column types (numbers, dates)
4. Create charts for voltage, current over time

## Maintenance

### Regular Checks
- Inspect relay contacts monthly
- Clean shunt resistor connections
- Verify RTC time accuracy
- Test emergency stop function
- Check all wire connections

### Calibration
If readings seem inaccurate:
1. Use known voltage source to verify voltage readings
2. Use known current load to verify current measurements
3. Update shunt resistance values in `config.h` if needed
4. Recompile and upload firmware

## Advanced Usage

### Multiple Battery Types
Test different battery types by using the appropriate `start <n>` command:
```
> start 0    # Lead Acid
> start 1    # LiFePO4
> start 2    # NiCad 4xAA
> start 3    # Li-ion cell
```

### Long-Term Testing
For multi-day tests:
- Ensure stable power supply
- Monitor initial readings to verify proper operation
- Check CSV logs periodically
- System will auto-stop at 48 hours

### Custom Battery Types
To add custom battery types:
1. Edit `include/config.h`
2. Add new battery type to `BatteryType` enum
3. Define voltage and capacity constants
4. Update cutoff voltages in state machine
5. Recompile and upload

## Support

For issues or questions:
- Check this user guide
- Review ASSEMBLY_NOTES.md for hardware setup
- Examine serial output for error messages
- Verify all connections and settings

---

**Warning**: Always follow proper battery handling procedures. Never leave system unattended during operation. Ensure proper ventilation and fire safety measures are in place.
