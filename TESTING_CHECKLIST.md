# Testing Checklist

This document provides a comprehensive checklist for testing the Battery Assembly Testing System.

## Pre-Testing Hardware Verification

### Power and Connections
- [ ] ESP32-POE-ISO powered via USB or PoE
- [ ] All I2C devices have 3.3V and GND connections
- [ ] 4.7kΩ pull-up resistors on SDA and SCL lines
- [ ] Relay modules have 12V power supply
- [ ] Common ground between ESP32 and relay power

### I2C Device Check
- [ ] DS3231 RTC connected to pins 13 (SDA) and 16 (SCL)
- [ ] INA3221 #1 (0x40) connected to I2C bus
- [ ] INA3221 #2 (0x41) connected to I2C bus with A0 pulled high
- [ ] CR2032 battery installed in DS3231

### Current Sensing Circuit
- [ ] 0.01Ω shunt resistor in charge path
- [ ] 0.01Ω shunt resistor in discharge path
- [ ] Schottky diodes (150V, 10A) installed for reverse protection
- [ ] INA3221 #1 CH1 connected across charge shunt
- [ ] INA3221 #2 CH1 connected across discharge shunt
- [ ] VS+ pins connected to measure battery voltage

### Relay Connections
- [ ] Main relay (Pin 32) wired with transistor driver
- [ ] AC Charger relay (Pin 33) wired with transistor driver
- [ ] AC Discharge relay (Pin 25) wired with transistor driver
- [ ] Charger Select relay (Pin 26) wired with transistor driver
- [ ] Flyback diodes (1N4007) across all relay coils
- [ ] Emergency stop button wired to interrupt relay power

### Status LEDs
- [ ] Status LED on Pin 2 with 220Ω resistor
- [ ] Charging LED on Pin 27 with 220Ω resistor
- [ ] Discharge LED on Pin 14 with 220Ω resistor

### Safety Components
- [ ] Fuses installed in charge and discharge paths
- [ ] Emergency stop button tested and functional
- [ ] Ventilation adequate for battery testing
- [ ] Fire extinguisher nearby
- [ ] Safety glasses and protective equipment available

## Software Testing

### Initial Upload
- [ ] PlatformIO installed and working
- [ ] Project builds without errors
- [ ] Firmware uploaded successfully to ESP32-POE-ISO
- [ ] Serial monitor opens at 115200 baud

### System Initialization
- [ ] System boots and displays startup banner
- [ ] ESP32-POE-ISO identified correctly
- [ ] Chip model and revision displayed
- [ ] Flash size shown correctly (8MB or 16MB)

### I2C Device Detection
- [ ] DS3231 RTC initialization successful
- [ ] INA3221 #1 initialization successful
- [ ] INA3221 #2 initialization successful
- [ ] No I2C errors in serial output

### RTC Functionality
- [ ] `time` command shows current date/time
- [ ] `settime` command sets time to compile time
- [ ] Time continues incrementing correctly
- [ ] Time persists through ESP32 reset (via CR2032)

### Relay Testing (Without Battery)
- [ ] `start 0` command activates charge relays
- [ ] Main relay switches to charge position
- [ ] AC Charger relay turns on
- [ ] Charging LED illuminates
- [ ] `stop` command deactivates all relays
- [ ] All relays return to safe state

### Current Measurement (With Load)
- [ ] Connect known current source to charge shunt
- [ ] Verify INA3221 #1 reads correct current ±5%
- [ ] Connect known current source to discharge shunt
- [ ] Verify INA3221 #2 reads correct current ±5%
- [ ] Voltage reading matches multimeter ±2%

### Command Interface
- [ ] `help` command displays all commands
- [ ] `status` command shows system state
- [ ] `start 0` starts Lead Acid test
- [ ] `start 1` starts LiFePO4 test
- [ ] `start 2` starts NiCad test
- [ ] `start 3` starts Li-ion test
- [ ] `stop` command halts test immediately
- [ ] `summary` command displays test results

## Functional Testing

### Test Cycle - Lead Acid Battery
- [ ] Connect 12V Lead Acid battery
- [ ] Connect charger to charge input
- [ ] Connect discharge load to discharge output
- [ ] Execute `start 0` command
- [ ] Verify voltage reading is correct
- [ ] Verify charge current is displayed
- [ ] Precharge state activates correctly
- [ ] AC Charger relay turns on
- [ ] Charging LED illuminates
- [ ] Current accumulation (Ah) increases
- [ ] Energy accumulation (Wh) increases
- [ ] Average current calculated correctly
- [ ] Peak current tracked correctly

### Charge Completion Detection
- [ ] Monitor charge current during charging
- [ ] Verify charge current decreases as battery fills
- [ ] System detects when current drops below 0.1A
- [ ] Charge Complete state activated after 60 seconds low current
- [ ] AC Charger relay turns off
- [ ] Total charge Ah recorded

### Discharge Cycle
- [ ] System transitions to Discharge Ready
- [ ] Main relay switches to discharge position
- [ ] AC Discharge relay turns on
- [ ] Discharge LED illuminates
- [ ] Discharge current displayed
- [ ] Voltage decreases during discharge
- [ ] Discharge Ah accumulates
- [ ] Cutoff voltage (10.5V for Lead Acid) detected
- [ ] Discharge stops at cutoff
- [ ] All relays turn off

### Test Summary
- [ ] Summary displays automatically at end
- [ ] Battery ID includes timestamp
- [ ] Start voltage recorded
- [ ] End voltage recorded
- [ ] Peak voltage recorded
- [ ] Charge duration in HH:MM:SS format
- [ ] Average charge current calculated
- [ ] Peak charge current recorded
- [ ] Total charge Ah displayed
- [ ] Total charge Wh displayed
- [ ] Discharge duration in HH:MM:SS format
- [ ] Average discharge current calculated
- [ ] Peak discharge current recorded
- [ ] Total discharge Ah displayed
- [ ] Total discharge Wh displayed
- [ ] Overall test duration recorded

### CSV Data Logging
- [ ] CSV header format is correct
- [ ] CSV data logged every 5 seconds during test
- [ ] Timestamp includes date and time
- [ ] Battery ID included in each line
- [ ] State name correct in CSV
- [ ] Voltage value correct
- [ ] Charge current value correct
- [ ] Discharge current value correct
- [ ] Total Ah values correct
- [ ] Total Wh values correct
- [ ] CSV can be imported to Excel/LibreOffice

### Display Updates
- [ ] Real-time display refreshes every 500ms
- [ ] Screen clears properly (ANSI codes working)
- [ ] All values update correctly
- [ ] No display flickering or corruption
- [ ] Time format is readable
- [ ] Numbers formatted with appropriate decimals

## Battery Type Testing

### LiFePO4 Battery Test
- [ ] Start test with `start 1`
- [ ] System uses 10.0V cutoff
- [ ] Test completes successfully
- [ ] Summary shows LiFePO4 battery type

### NiCad Battery Test
- [ ] Start test with `start 2`
- [ ] System uses 4.0V cutoff
- [ ] Test completes successfully
- [ ] Summary shows NiCad battery type

### Li-ion Cell Test
- [ ] Start test with `start 3`
- [ ] System uses 2.8V cutoff
- [ ] Test completes successfully
- [ ] Summary shows Li-ion battery type

## Safety Testing

### Voltage Cutoff
- [ ] Discharge stops at correct cutoff voltage
- [ ] Relays immediately deactivate
- [ ] Error message displayed if voltage too low
- [ ] System enters safe state

### Maximum Duration
- [ ] Set MAX_TEST_DURATION_HOURS to 1 hour (for testing)
- [ ] Start test and wait 1+ hours
- [ ] Verify test stops automatically
- [ ] System returns to safe state

### Emergency Stop
- [ ] During active test, issue `stop` command
- [ ] All relays deactivate immediately
- [ ] Charging stops
- [ ] Discharging stops
- [ ] Summary still available
- [ ] Data preserved up to stop point

### Hardware Emergency Stop
- [ ] Press emergency stop button during test
- [ ] Relay power interrupted
- [ ] All relays open
- [ ] Battery disconnected from all paths
- [ ] ESP32 continues operating
- [ ] Can restart after clearing emergency stop

## Long-Term Testing

### 24-Hour Test
- [ ] Start test on large capacity battery
- [ ] Monitor periodically over 24 hours
- [ ] CSV logging continuous
- [ ] No system crashes or resets
- [ ] Data integrity maintained
- [ ] Test completes successfully

### Multiple Test Cycles
- [ ] Run complete test cycle
- [ ] Review and save results
- [ ] Start new test immediately
- [ ] Data from previous test cleared
- [ ] New test runs successfully
- [ ] Repeat 5 times
- [ ] No memory leaks or degradation

### Data Logging
- [ ] Capture 24 hours of CSV data
- [ ] Extract CSV lines from log
- [ ] Import to spreadsheet
- [ ] Create voltage vs time chart
- [ ] Create current vs time chart
- [ ] Verify data integrity
- [ ] Calculate capacity from integrated current

## Performance Validation

### Measurement Accuracy
- [ ] Compare voltage reading to calibrated multimeter
- [ ] Error less than ±0.05V
- [ ] Compare current to calibrated ammeter
- [ ] Error less than ±50mA (±5%)
- [ ] Repeat at multiple current levels
- [ ] Calculate and document calibration factors if needed

### Timing Accuracy
- [ ] Start test and note exact time
- [ ] Wait exactly 1 hour by external clock
- [ ] Check test duration display
- [ ] Error less than ±2 seconds per hour
- [ ] RTC drift acceptable (±2ppm typical for DS3231)

### Capacity Measurement
- [ ] Test battery with known capacity
- [ ] Compare measured Ah to rated capacity
- [ ] Account for battery age and condition
- [ ] Verify discharge capacity within expected range
- [ ] Calculate round-trip efficiency
- [ ] Compare to specifications

## Documentation Verification

- [ ] README.md instructions followed successfully
- [ ] USER_GUIDE.md commands all work as described
- [ ] WIRING_GUIDE.md connections are correct
- [ ] EXAMPLES.md scenarios can be reproduced
- [ ] ASSEMBLY_NOTES.md safety guidelines followed
- [ ] CHANGELOG.md accurately describes version

## Issues Found

Document any issues discovered during testing:

| Issue # | Description | Severity | Status |
|---------|-------------|----------|--------|
| 1       |             |          |        |
| 2       |             |          |        |

## Test Results Summary

- **Date**: _____________
- **Tester**: _____________
- **Hardware Version**: _____________
- **Firmware Version**: 1.0.0
- **Overall Status**: [ ] PASS  [ ] FAIL
- **Notes**:

---

## Sign-Off

- [ ] All critical tests passed
- [ ] Documentation is accurate
- [ ] System is safe for operation
- [ ] Ready for deployment

**Tested by**: ________________  **Date**: ________

**Reviewed by**: ________________  **Date**: ________
