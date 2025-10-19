# Header Files

This directory contains header files for the Battery Testing System.

## Current Files

### config.h
Configuration and pin definitions for the ESP32-POE-ISO hardware:
- I2C pin assignments (SDA=13, SCL=16)
- INA3221 ammeter addresses and shunt configurations
- DS3231 RTC settings
- Relay control pin definitions
- Battery type specifications and cutoff voltages
- Test cycle timing parameters
- Safety limits and thresholds

### battery_data.h
Data structures and classes for battery testing:
- `BatteryTestData` structure - Stores all test measurements and results
- `MovingAverage` class - Implements moving average filter for current smoothing
- Helper functions for formatting time and durations
- Test state enumeration and battery type definitions

## Usage

Headers in this directory are automatically available to your source files:

```cpp
#include "config.h"
#include "battery_data.h"
```

## Customization

### Adding New Battery Types
1. Add to `BatteryType` enum in config.h
2. Define voltage and capacity constants
3. Update state machine cutoff logic in main.cpp
4. Update `getBatteryTypeName()` function

### Changing Pin Assignments
1. Edit pin definitions in config.h
2. Verify hardware compatibility
3. Update WIRING_GUIDE.md to reflect changes

### Adjusting Measurement Parameters
- Modify `AVERAGING_SAMPLES` for smoother/faster response (currently 10)
- Adjust `UPDATE_INTERVAL_MS` for data collection rate (currently 1000ms)
- Change `LOG_INTERVAL_MS` for CSV logging frequency (currently 5000ms)
- Update `DISPLAY_UPDATE_MS` for screen refresh rate (currently 500ms)

## Best Practices

1. **Include Guards** - All headers use include guards:
```cpp
#ifndef CONFIG_H
#define CONFIG_H

// Header content here

#endif // CONFIG_H
```

2. **Documentation** - Functions and constants are documented with Doxygen-style comments

3. **Minimal Dependencies** - Headers only include what's necessary

4. **Type Safety** - Use enums for state machines and battery types instead of magic numbers
