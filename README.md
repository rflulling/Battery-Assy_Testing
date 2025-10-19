# Battery Assembly Testing System

A comprehensive battery testing apparatus and software system designed to test multiple large batteries with safety monitoring and data collection capabilities.

## Overview

This project provides firmware and documentation for building a battery assembly testing system capable of:
- Testing multiple large batteries simultaneously
- Monitoring voltage, current, and temperature
- Implementing safety shutdown mechanisms
- Logging test data for analysis
- Supporting multiple microcontroller platforms

## Supported Platforms

### Primary Platform
- **ESP32** - ESP32-DevKitC or similar (recommended)
  - Built-in Wi-Fi/Bluetooth for remote monitoring
  - Sufficient GPIO pins for multiple battery channels
  - Good ADC performance for measurements

### Alternative Platforms
- **STM32H7** - High-performance ARM Cortex-M7
  - Superior processing power for complex algorithms
  - Precision ADCs for accurate measurements
  - Suitable for demanding real-time applications

- **Milk-V Duo 64** - RISC-V based SBC (experimental)
  - Linux-based for advanced data processing
  - Good for logging and analysis
  - May require additional setup and drivers

## Project Structure

```
Battery-Assy_Testing/
├── platformio.ini          # PlatformIO configuration
├── src/                    # Source code
│   └── main.cpp           # Main application entry point
├── include/               # Header files
├── lib/                   # Custom libraries
├── test/                  # Unit tests
├── README.md              # This file
├── ASSEMBLY_NOTES.md      # Mechanical assembly instructions
├── CHANGELOG.md           # Version history
└── .gitignore            # Git ignore rules
```

## Getting Started

### Prerequisites

1. **PlatformIO IDE** (recommended) or **PlatformIO CLI**
   - Install via VS Code extension or command line
   - Documentation: https://docs.platformio.org/

2. **Hardware**
   - ESP32 development board (or alternative platform)
   - USB cable for programming
   - See ASSEMBLY_NOTES.md for complete hardware requirements

### Building the Project

#### Using VS Code with PlatformIO Extension

1. Open the project folder in VS Code
2. PlatformIO will automatically detect the project
3. Select your target environment (esp32dev, stm32h7, etc.)
4. Click "Build" in the PlatformIO toolbar

#### Using PlatformIO CLI

```bash
# Build for ESP32 (default)
pio run

# Build for specific environment
pio run -e esp32dev      # ESP32
pio run -e esp32-s3      # ESP32-S3
pio run -e stm32h7       # STM32H7

# Build all environments
pio run -e all
```

### Uploading Firmware

#### Using VS Code
1. Connect your board via USB
2. Click "Upload" in the PlatformIO toolbar

#### Using PlatformIO CLI
```bash
# Upload to default environment (ESP32)
pio run --target upload

# Upload to specific environment
pio run -e esp32dev --target upload
```

### Serial Monitor

#### Using VS Code
- Click "Serial Monitor" in PlatformIO toolbar

#### Using PlatformIO CLI
```bash
pio device monitor -b 115200
```

## Configuration

### Changing Target Platform

Edit `platformio.ini` and set the default environment:

```ini
[platformio]
default_envs = esp32dev  # Change to esp32-s3, stm32h7, etc.
```

### Adjusting Build Flags

Modify build flags in `platformio.ini`:

```ini
[env]
build_flags = 
    -D VERSION="1.0.0"
    -D PROJECT_NAME="Battery Assembly Testing"
    -D CUSTOM_FLAG=value
```

## Testing

### Running Unit Tests

```bash
# Run all tests
pio test

# Run tests for specific environment
pio test -e esp32dev
```

### Creating Tests

Place test files in the `test/` directory. See PlatformIO documentation for test framework usage.

## Mechanical Assembly

Refer to [ASSEMBLY_NOTES.md](ASSEMBLY_NOTES.md) for detailed:
- Hardware component lists
- Step-by-step assembly instructions
- Safety considerations and guidelines
- Wiring diagrams and schematics
- Calibration procedures
- Maintenance schedules

## Safety Warnings

⚠️ **DANGER: HIGH CURRENT AND VOLTAGE**

This system is designed to work with large batteries that can deliver extremely high currents. Improper assembly or operation can result in:
- Electric shock
- Fire or explosion
- Chemical burns
- Equipment damage

**Only qualified personnel should build and operate this equipment.**

Key safety requirements:
- Install emergency stop mechanisms
- Use appropriate current ratings for all components
- Implement multiple protection layers (fuses, relays, shutoffs)
- Ensure proper ventilation for battery gases
- Never leave system unattended during operation
- Follow all local electrical codes and regulations

See ASSEMBLY_NOTES.md for complete safety guidelines.

## Features

### Current Features (v1.0.0)
- Multi-platform support (ESP32, STM32H7, Milk-V Duo 64)
- Basic firmware skeleton with initialization
- Serial communication for monitoring
- System information reporting

### Planned Features
- Battery voltage measurement and monitoring
- Current sensing and logging
- Temperature monitoring with safety limits
- Automatic safety shutdown logic
- Display support (LCD/OLED)
- Web interface for remote monitoring
- Data logging (SD card or flash)
- Multiple independent battery channels
- Charge/discharge cycling capabilities

## Version History

See [CHANGELOG.md](CHANGELOG.md) for detailed version history and release notes.

## Development

### Code Style
- Use clear, descriptive variable and function names
- Comment complex logic and algorithms
- Follow Arduino/PlatformIO best practices
- Document functions with Doxygen-style comments

### Contributing
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly on target hardware
5. Submit a pull request

### Adding Support for New Platforms
1. Add new environment in `platformio.ini`
2. Test compilation and basic functionality
3. Update documentation
4. Add platform-specific notes to ASSEMBLY_NOTES.md

## Troubleshooting

### Build Issues
- Ensure PlatformIO is up to date: `pio upgrade`
- Clean build files: `pio run --target clean`
- Check platform and framework versions in platformio.ini

### Upload Issues
- Verify correct USB port selection
- Check board is in bootloader mode (if required)
- Try different USB cable or port
- Install/update USB drivers for your platform

### Runtime Issues
- Check serial monitor for error messages
- Verify hardware connections
- Ensure proper power supply
- Review calibration settings

## Resources

- [PlatformIO Documentation](https://docs.platformio.org/)
- [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [STM32 Documentation](https://www.st.com/en/microcontrollers-microprocessors/stm32h7-series.html)
- [Arduino Framework Reference](https://www.arduino.cc/reference/en/)

## License

[Specify your license here]

## Contact

[Specify contact information or repository maintainer]

## Acknowledgments

[Add acknowledgments, credits, or references]

---

**Last Updated:** 2025-10-19  
**Version:** 1.0.0
