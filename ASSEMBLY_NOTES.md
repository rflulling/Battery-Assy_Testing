# Assembly Notes - Battery Assembly Testing System

## Overview
This document provides mechanical assembly instructions and considerations for the Battery Assembly Testing System. The system is designed to test multiple large batteries simultaneously with appropriate safety measures.

## Hardware Components

### Required Components
1. **Microcontroller Board**
   - Primary: ESP32 Development Board (ESP32-DevKitC or similar)
   - Alternative 1: STM32H743ZI Nucleo Board
   - Alternative 2: Milk-V Duo 64 (experimental)

2. **Power Supply**
   - Isolated DC power supply (12V/5A minimum)
   - Voltage regulator (if using batteries for system power)

3. **Battery Interface Components**
   - Battery holders/connectors (appropriate for battery type and size)
   - High-current relays or contactors (rated for battery current)
   - Emergency stop switches (NC configuration recommended)
   - Fuses or circuit breakers (rated 150% of maximum battery current)

4. **Measurement Components**
   - Precision voltage dividers or isolated voltage sensors
   - Hall-effect current sensors or shunt resistors with isolated amplifiers
   - Temperature sensors (thermocouples or digital sensors like DS18B20)

5. **Safety Components**
   - Reverse polarity protection diodes
   - Overcurrent protection
   - Thermal fuses
   - Fire-resistant enclosure
   - Ventilation fans (for heat and gas dissipation)

6. **Display and User Interface** (Optional)
   - LCD/OLED display
   - Status LEDs
   - Control buttons or keypad
   - Emergency stop button (large, red, mushroom-style)

## Mechanical Assembly Instructions

### Step 1: Enclosure Preparation
1. Select a fire-resistant enclosure (metal or flame-rated plastic)
2. Install ventilation fans at top and bottom for proper airflow
3. Mount emergency stop button on front panel, easily accessible
4. Create cable entries with appropriate grommets for battery connections

### Step 2: Mounting the Microcontroller
1. Install standoffs or DIN rail mounts inside enclosure
2. Mount the ESP32/STM32H7 board securely with proper spacing for cooling
3. Ensure the board is isolated from the enclosure to prevent shorts
4. Position for easy access to USB/programming port

### Step 3: Power Supply Installation
1. Mount the isolated DC power supply on DIN rail or standoffs
2. Connect AC input through a fused switch
3. Install voltage regulator if needed for MCU power
4. Wire power supply ground to enclosure ground for safety

### Step 4: Battery Connection Interface
1. Install battery terminals/connectors rated for expected current
2. Mount high-current relays between battery terminals and test circuitry
   - Use appropriate heatsinks for relays
   - Ensure adequate spacing (minimum 10mm between terminals)
3. Install reverse polarity protection on each battery channel
4. Add emergency disconnect relays controlled by emergency stop

### Step 5: Measurement Circuit Installation
1. **Voltage Sensing:**
   - Install voltage divider networks or isolated voltage sensors
   - Connect to ADC inputs on microcontroller
   - Use TVS diodes for overvoltage protection
   - Ensure isolation if measuring high voltages

2. **Current Sensing:**
   - Mount Hall-effect sensors or install shunt resistors
   - Position sensors in series with battery load path
   - Connect sensor outputs to ADC inputs
   - Calibrate with known current loads

3. **Temperature Monitoring:**
   - Attach temperature sensors to battery terminals
   - Install ambient temperature sensor inside enclosure
   - Route sensor cables away from high-current paths

### Step 6: Safety System Wiring
1. Wire emergency stop button to interrupt relay coils (NC configuration)
2. Install fuses or circuit breakers in battery current paths
3. Add overcurrent detection circuits with automatic shutoff
4. Install thermal cutoffs on critical components
5. Connect status LEDs to indicate system state

### Step 7: User Interface Assembly (if applicable)
1. Mount display on front panel
2. Install control buttons in ergonomic layout
3. Connect interface components to microcontroller GPIO
4. Ensure all controls are clearly labeled

### Step 8: Cable Management
1. Route power cables separately from signal cables
2. Use cable ties or cable ducts for organization
3. Label all cables with source and destination
4. Ensure no cables can contact sharp edges or moving parts
5. Leave service loops for maintenance access

## Safety Considerations

### Electrical Safety
- **DANGER:** Large batteries can deliver extremely high currents
- Always use appropriate current ratings for all components
- Install multiple levels of protection (fuses, relays, emergency stop)
- Ensure proper grounding of all metal components
- Use insulated tools when working with live circuits
- Never bypass safety interlocks or emergency stop

### Chemical Safety
- Batteries may vent gases during testing
- Ensure adequate ventilation
- Keep fire extinguisher (Class D for lithium) nearby
- Have spill containment materials available
- Follow battery manufacturer's safety guidelines

### Thermal Safety
- Monitor temperatures continuously during testing
- Install thermal protection on all high-power components
- Ensure adequate cooling airflow
- Keep flammable materials away from testing area

### Operational Safety
- Test emergency stop function before each use
- Never leave testing system unattended during operation
- Establish maximum test duration and automatic shutoff
- Document all safety procedures and train operators

## Testing and Calibration

### Pre-Operation Checklist
1. Verify all connections are secure
2. Test emergency stop function
3. Calibrate voltage measurements with known reference
4. Calibrate current sensors with known loads
5. Verify temperature sensors read correctly
6. Check all safety interlocks
7. Perform dry run without batteries

### Calibration Procedure
1. **Voltage Calibration:**
   - Apply known voltage to inputs
   - Record ADC readings
   - Calculate calibration factors
   - Store in firmware

2. **Current Calibration:**
   - Pass known current through sensors
   - Record sensor outputs
   - Calculate calibration factors
   - Verify linearity across range

3. **Temperature Calibration:**
   - Compare readings to reference thermometer
   - Apply offset corrections if needed

## Maintenance Schedule

### Daily (if in use)
- Visual inspection for damage or loose connections
- Emergency stop function test
- Check ventilation fans
- Verify temperature readings

### Weekly
- Clean dust and debris from enclosure
- Check all wire connections
- Test all safety relays
- Inspect battery terminals for corrosion

### Monthly
- Recalibrate voltage and current measurements
- Test all fuses and circuit breakers
- Inspect for signs of overheating
- Update firmware if needed

### Annually
- Replace ventilation fan filters
- Test all safety components
- Professional electrical inspection (recommended)
- Update documentation and procedures

## Platform-Specific Notes

### ESP32
- 3.3V logic levels - ensure level shifters for 5V components
- Use isolated sensors for high-voltage measurements
- Wi-Fi/Bluetooth available for remote monitoring
- Adequate GPIO pins for multiple battery channels

### STM32H7
- 3.3V logic levels
- High-performance ADCs for precision measurements
- More processing power for complex algorithms
- May require external Wi-Fi module for connectivity

### Milk-V Duo 64
- Experimental support
- Linux-based system allows for advanced logging
- May require custom drivers for hardware interfaces
- Consider using for data processing rather than real-time control

## Bill of Materials (BOM) Template

| Item | Description | Quantity | Specifications | Notes |
|------|-------------|----------|----------------|-------|
| MCU | ESP32 Dev Board | 1 | ESP32-DevKitC | Primary controller |
| PSU | DC Power Supply | 1 | 12V, 5A, isolated | System power |
| Relay | Contactor/Relay | 4+ | Rated for battery current | One per battery channel |
| Sensor | Voltage Sensor | 4+ | Isolated, range appropriate | One per channel |
| Sensor | Current Sensor | 4+ | Hall-effect or shunt | One per channel |
| Sensor | Temperature Sensor | 4+ | DS18B20 or thermocouple | One per battery |
| Safety | Emergency Stop | 1 | NC, mushroom button | Front panel |
| Safety | Fuses | 4+ | 150% of max current | One per channel |
| Enclosure | Metal Box | 1 | Fire-resistant, adequate size | NEMA rated |
| Cooling | Ventilation Fans | 2 | 12V, adequate CFM | Top and bottom |

## Revision History
- Version 1.0.0 (2025-10-19): Initial assembly documentation

## Additional Resources
- PlatformIO Documentation: https://docs.platformio.org/
- ESP32 Datasheet: https://www.espressif.com/
- Battery Safety Guidelines: Consult manufacturer
- Electrical Safety Standards: IEC 60950, UL 60950

---
**WARNING:** This system handles high currents and potentially dangerous batteries. Only qualified personnel should assemble, maintain, or operate this equipment. Follow all safety guidelines and local electrical codes.
