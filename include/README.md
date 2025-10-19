# Header Files

This directory is for project header files.

## Purpose

Place your custom header (.h) files here for inclusion in your source code.

## Usage

Headers in this directory are automatically available to your source files:

```cpp
#include "MyHeader.h"
```

## Structure

Organize headers logically:

```
include/
├── config.h              # Configuration constants
├── battery_monitor.h     # Battery monitoring functions
├── safety.h             # Safety system functions
└── hardware/            # Hardware-specific headers
    ├── adc.h
    └── sensors.h
```

## Best Practices

1. **Include Guards** - Always use include guards:
```cpp
#ifndef MY_HEADER_H
#define MY_HEADER_H

// Header content here

#endif // MY_HEADER_H
```

2. **Documentation** - Document functions and constants:
```cpp
/**
 * @brief Measure battery voltage
 * @param channel Battery channel number (0-3)
 * @return Voltage in volts
 */
float measureBatteryVoltage(uint8_t channel);
```

3. **Minimal Dependencies** - Only include what's necessary

4. **Platform-Specific Code** - Use conditional compilation:
```cpp
#ifdef ESP32
    // ESP32-specific code
#elif defined(STM32H7)
    // STM32-specific code
#endif
```
