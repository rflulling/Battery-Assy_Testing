# Example Usage Scenarios

## Scenario 1: Testing a Lead Acid Motorcycle Battery

### Hardware Setup
1. Connect ESP32-POE-ISO to computer via USB
2. Connect battery to test fixture
3. Connect charger to charge input
4. Connect discharge load to discharge output
5. Ensure all relays are properly wired

### Steps
1. Open PuTTY at 115200 baud
2. Wait for system initialization
3. Verify RTC time is correct:
   ```
   > time
   RTC Time: 2025-10-19 14:30:00
   ```
4. If time is wrong, set it:
   ```
   > settime
   RTC time set to compile time.
   ```
5. Check system status:
   ```
   > status
   Current Status:
     State: Idle
     Test Active: No
     Battery Type: Lead Acid
   ```
6. Start the test:
   ```
   > start 0
   ========================================
   STARTING BATTERY TEST
   ========================================
   Battery Type: Lead Acid
   Battery ID: Lead Acid_20251019_143500
   ========================================
   ```
7. Monitor real-time display showing:
   - Voltage: 12.456 V (Peak: 14.400 V)
   - Charge current: 2.350 A (Avg: 2.100 A, Peak: 2.500 A)
   - Total Charge: 8.234 Ah (99.21 Wh)

8. Wait for precharge to complete (when charge current drops below 0.1A)
9. System automatically switches to discharge mode
10. Wait for discharge to complete (voltage reaches 10.5V cutoff)
11. Review summary:
    ```
    > summary
    ========================================
      BATTERY TEST SUMMARY
    ========================================
    Battery ID: Lead Acid_20251019_143500
    Battery Type: Lead Acid

    --- Voltage ---
    Start: 12.456 V
    End:   10.500 V
    Peak:  14.400 V

    --- Charge Data ---
    Duration:      02:15:30
    Average:       2.100 A
    Peak:          2.500 A
    Total Charge:  8.234 Ah (118.50 Wh)

    --- Discharge Data ---
    Duration:      01:45:20
    Average:       1.850 A
    Peak:          2.100 A
    Total Charge:  7.125 Ah (89.25 Wh)

    --- Overall ---
    Total Duration: 04:00:50
    ========================================
    ```

### Results Interpretation
- **Capacity**: 7.125 Ah (discharge) vs 13 Ah (rated)
  - Battery at ~55% of rated capacity
  - May indicate aging or sulfation
  
- **Efficiency**: 7.125 / 8.234 = 86.5%
  - Good charge/discharge efficiency
  - Within normal range for Lead Acid

## Scenario 2: Testing LiFePO4 Battery

### Steps
1. Connect LiFePO4 battery (nominal 12.8V, 90Ah)
2. Set charger select relay for Lithium charger
3. Start test:
   ```
   > start 1
   ```
4. Monitor longer test cycle (may take 10-20 hours)
5. System uses 10.0V cutoff for LiFePO4
6. Review capacity and efficiency

### Expected Results
- Higher voltage plateau during discharge
- Better efficiency (90-95%)
- More stable voltage curve
- Longer test duration due to larger capacity

## Scenario 3: Testing NiCad 4xAA Pack

### Steps
1. Connect 4xAA NiCad pack (nominal 4.8V)
2. Use appropriate low-current charger
3. Start test:
   ```
   > start 2
   ```
4. System uses 4.0V cutoff for NiCad
5. Monitor for voltage depression effect
6. Multiple charge/discharge cycles may be needed

### Considerations
- NiCad benefits from periodic deep discharge
- May show voltage depression (memory effect)
- Lower efficiency than lithium chemistries

## Scenario 4: Testing Single Li-ion Cell

### Steps
1. Connect single 18650 cell (nominal 3.7V)
2. Use appropriate Li-ion charger
3. Start test:
   ```
   > start 3
   ```
4. System uses 2.8V cutoff for Li-ion
5. Watch for consistent capacity

### Safety Notes
- Never discharge Li-ion below 2.5V
- Use appropriate charge rate (typically 0.5C-1C)
- Monitor temperature if available

## Scenario 5: Batch Testing Multiple Cells

### Workflow
1. Test cell 1, save CSV data
2. Note battery ID from summary
3. Stop test with `stop` command
4. Replace battery
5. Start next test
6. Compare results from CSV logs

### Data Collection
```bash
# In PuTTY, enable logging to file
# All CSV lines will be saved like:
[CSV] 2025-10-19 14:30:00,LeadAcid_20251019_143000,Charging,12.450,2.350,0.000,0.125,0.000,1.512,0.000
[CSV] 2025-10-19 14:30:05,LeadAcid_20251019_143000,Charging,12.680,2.320,0.000,0.130,0.000,1.575,0.000
...
```

### Analysis
1. Extract CSV data:
   ```bash
   grep "^\[CSV\]" battery_test.log > data.csv
   ```
2. Import to spreadsheet
3. Create charts of voltage and current over time
4. Compare capacities between cells
5. Identify matched pairs with similar performance

## Scenario 6: Monitoring Only (No Relay Control)

### Use Case
Monitor an external charger/discharger without relay automation.

### Steps
1. Connect battery to external equipment
2. Monitor with system running:
   ```
   > status
   ```
3. Observe real-time current and voltage
4. Manually control external equipment
5. Log CSV data for record keeping

### Benefits
- Works with any existing charger/discharger
- Provides accurate timestamped data
- No relay hardware needed for testing

## Scenario 7: Emergency Stop

### When to Use
- Battery temperature too high
- Unexpected behavior
- Safety concern
- Test taking too long

### Steps
1. Issue stop command:
   ```
   > stop
   ```
2. System immediately:
   - Disables all relays
   - Stops charging/discharging
   - Saves current data
   - Prints summary
3. Investigate issue before restarting

## Scenario 8: Long-Term Capacity Testing

### Purpose
Verify battery maintains capacity over multiple cycles.

### Procedure
1. Perform initial test, record capacity
2. Let battery rest 24 hours
3. Perform test again
4. Compare results
5. Repeat weekly/monthly
6. Track degradation over time

### Example Log
```
Date       | Battery ID  | Charge (Ah) | Discharge (Ah) | Efficiency
-----------|-------------|-------------|----------------|------------
2025-10-19 | BATT001     | 13.2        | 12.1          | 91.7%
2025-10-26 | BATT001     | 13.0        | 11.9          | 91.5%
2025-11-02 | BATT001     | 12.8        | 11.6          | 90.6%
```

### Analysis
- Gradual capacity loss indicates aging
- Sharp drops may indicate damage
- Track efficiency to detect internal resistance increase

## Tips and Best Practices

### Before Starting Test
- Verify battery voltage is reasonable
- Check all connections are secure
- Ensure adequate ventilation
- Set appropriate charger type
- Verify RTC time is correct

### During Test
- Monitor initial readings for anomalies
- Check CSV logging is working
- Verify relays are switching properly
- Watch for temperature issues
- Don't leave unattended

### After Test
- Review summary data
- Save CSV log file
- Label battery with test results
- Compare to specifications
- Plan for retest if needed

### Data Management
- Create folder structure by date
- Name files with battery ID
- Keep log of all tests
- Back up important data
- Create comparison spreadsheets

### Troubleshooting Tests
- If current reads zero, check shunt connections
- If voltage is wrong, verify INA3221 wiring
- If test won't start, check relay power
- If time is wrong, use `settime` command
- If CSV not logging, check serial output

## Advanced: Automated Testing Script

For PC-based automation, you can script the serial commands:

```python
import serial
import time

ser = serial.Serial('COM3', 115200, timeout=1)
time.sleep(2)

# Start test
ser.write(b'start 0\n')

# Monitor for completion
while True:
    line = ser.readline().decode('utf-8').strip()
    print(line)
    
    # Save CSV lines
    if line.startswith('[CSV]'):
        with open('battery_data.csv', 'a') as f:
            f.write(line[6:] + '\n')
    
    # Check for completion
    if 'Test cycle complete' in line:
        break
    
    time.sleep(0.1)

# Get summary
ser.write(b'summary\n')
time.sleep(2)

summary = ser.read(ser.in_waiting).decode('utf-8')
print(summary)

ser.close()
```

This allows hands-off testing of multiple batteries in sequence.
