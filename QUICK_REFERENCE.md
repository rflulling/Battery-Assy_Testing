# Quick Reference Card

## Serial Commands
```
help        Show available commands
status      Show current system status
start <n>   Start test (0=Lead Acid, 1=LiFePO4, 2=NiCad, 3=Li-ion)
stop        Stop current test
summary     Print test summary
time        Show RTC time
settime     Set RTC to compile time
```

## Pin Assignments
```
I2C Bus:
  Pin 13   SDA (INA3221 #1, INA3221 #2, DS3231)
  Pin 16   SCL (INA3221 #1, INA3221 #2, DS3231)

Relay Control:
  Pin 32   Main Relay (Charge/Discharge)
  Pin 33   AC Charger Relay
  Pin 25   AC Discharge Relay
  Pin 26   Charger Select Relay

Status LEDs:
  Pin 2    Status LED
  Pin 27   Charging LED
  Pin 14   Discharge LED
```

## I2C Addresses
```
INA3221 #1 (Charge):     0x40
INA3221 #2 (Discharge):  0x41
DS3231 RTC:              0x68
```

## Battery Types
```
Type 0: Lead Acid      12.0V nominal, 13Ah,  cutoff 10.5V
Type 1: LiFePO4        12.8V nominal, 90Ah,  cutoff 10.0V
Type 2: NiCad 4xAA      4.8V nominal,  2.5Ah, cutoff  4.0V
Type 3: Li-ion Cell     3.7V nominal,  3.0Ah, cutoff  2.8V
```

## Test States
```
Idle              → System ready, no test active
Precharge         → Charging battery
Charge Complete   → Charge finished, preparing for discharge
Discharge Ready   → About to start discharge
Discharging       → Actively discharging battery
Discharge Complete→ Discharge finished
Test Complete     → Entire test cycle finished
Error             → Error occurred, test stopped
```

## Display Format
```
========================================
  BATTERY TEST MONITOR
========================================
Time: 2025-10-19 14:30:00
State: Charging
Battery: Lead Acid
Running Time: 02:15:30
----------------------------------------
Voltage:     12.450 V  (Peak: 14.400 V)
Charge:      2.350 A  (Avg: 2.100 A, Peak: 2.500 A)
Discharge:   0.000 A  (Avg: 0.000 A, Peak: 0.000 A)
----------------------------------------
Total Charge:    8.234 Ah  (118.50 Wh)
Total Discharge: 0.000 Ah  (0.00 Wh)
========================================
```

## CSV Format
```
[CSV] Timestamp,BatteryID,State,Voltage,ChargeCurrent,DischargeCurrent,TotalChargeAh,TotalDischargeAh,TotalChargeWh,TotalDischargeWh
```

## Safety Limits
```
Max Test Duration:  48 hours
Min Voltage Cutoff: 0.5V (emergency)
Battery Cutoffs:    Varies by type (see above)
```

## Timing Intervals
```
Measurement Update:  1000ms (1 second)
Display Refresh:      500ms (0.5 seconds)
CSV Logging:         5000ms (5 seconds)
```

## Common Troubleshooting
```
Problem                    Solution
--------------------       ------------------------
No I2C devices found       Check SDA/SCL connections, add pull-ups
Current reads zero         Verify shunt connections and polarity
Voltage incorrect          Check INA3221 VS+ connections
RTC time wrong             Use 'settime' command, replace CR2032
Relay not switching        Check 12V power, verify driver circuit
Test won't start           Check relay power and connections
```

## Shunt Configuration
```
Resistance: 0.01Ω (10 milliohms)
Power:      5W minimum
Location:   Series with battery paths
Protection: Schottky diode (150V, 10A)
```

## Build & Upload
```bash
# Build firmware
pio run -e esp32-poe-iso

# Upload firmware
pio run -e esp32-poe-iso --target upload

# Open serial monitor
pio device monitor -b 115200
```

## File Structure
```
Battery-Assy_Testing/
├── src/main.cpp           - Main application code
├── include/
│   ├── config.h          - Pin & constant definitions
│   └── battery_data.h    - Data structures
├── platformio.ini        - PlatformIO configuration
├── README.md             - Project overview
├── USER_GUIDE.md         - Operating instructions
├── WIRING_GUIDE.md       - Hardware connections
├── EXAMPLES.md           - Usage scenarios
└── TESTING_CHECKLIST.md  - Testing procedures
```

## Typical Test Workflow
```
1. Connect battery and equipment
2. Open serial monitor (115200 baud)
3. Verify time: > time
4. Check status: > status
5. Start test: > start 0
6. Monitor real-time display
7. Wait for completion
8. Review summary: > summary
9. Save CSV data from log
```

## Data Analysis
```bash
# Extract CSV from PuTTY log
grep "^\[CSV\]" battery_test.log > data.csv

# Remove CSV prefix
sed 's/^\[CSV\] //' data.csv > clean_data.csv

# Import to Excel/LibreOffice
# Create charts for voltage, current over time
```

## Emergency Procedures
```
Software Emergency Stop:
  > stop

Hardware Emergency Stop:
  Press emergency stop button

Both will:
  - Deactivate all relays
  - Stop charge/discharge
  - Preserve data
  - Return to safe state
```

## Important Notes
- Never leave system unattended during testing
- Ensure proper ventilation for battery gases
- Keep fire extinguisher nearby
- Use appropriate PPE (safety glasses, gloves)
- Follow battery manufacturer safety guidelines
- Monitor temperature if possible
- Check connections before each test

## Support Resources
- GitHub: https://github.com/rflulling/Battery-Assy_Testing
- Documentation: All .md files in repository
- Issues: Report via GitHub Issues
