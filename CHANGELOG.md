# Changelog

All notable changes to the Battery Assembly Testing System will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- Add temperature monitoring with DS18B20 sensors
- Implement SD card data storage
- Add 4.5" SPI LCD display support
- Create physical button/dial input interface
- Add web interface for remote monitoring via Ethernet
- Implement multiple independent battery channel support
- Add enhanced fault detection algorithms
- Implement battery pairing/matching based on test results
- Add thermal receipt printing capability

## [1.0.0] - 2025-10-19

### Added
- Initial PlatformIO project structure
- ESP32-POE-ISO (Olimex) as primary microcontroller platform
- Dual INA3221 current sensor integration for charge/discharge monitoring
- DS3231 RTC integration for accurate timekeeping
- Relay control system for automated charge/discharge switching
- Real-time voltage and current measurement
- Average and peak current tracking with moving average filter
- Accumulated Ah and Wh measurement calculations
- CSV data logging with RTC timestamps
- Serial terminal interface compatible with PuTTY
- Support for four battery types:
  - Lead Acid motorcycle battery (~13Ah)
  - LiFePO4 battery (~90Ah)
  - NiCad 4xAA pack
  - Lithium Ion cells
- Automated test cycle state machine
  - Precharge state
  - Charging state
  - Discharge state
  - Completion and error states
- Interactive serial commands:
  - help - Show available commands
  - status - Display current system status
  - start <n> - Start battery test
  - stop - Stop current test
  - summary - Print test summary report
  - time - Show RTC time
  - settime - Set RTC to compile time
- Real-time display with voltage, current, averages, peaks, and totals
- Safety features:
  - Voltage cutoff monitoring per battery type
  - Maximum test duration limit
  - Emergency relay shutdown
- Test summary reporting with complete statistics
- Configuration system with pin definitions
- Battery data structures with reset and formatting functions
- Moving average filter for smooth current readings

### Hardware Configuration
- I2C bus on pins 13 (SDA) and 16 (SCL)
- Relay control on pins 32, 33, 25, 26
- Status LEDs on pins 2, 27, 14
- Support for Schottky diode reverse bias protection
- Dual INA3221 ammeter chips (0x40 and 0x41)

### Project Structure
- `src/` - Source code directory with main.cpp
- `include/` - Header files (config.h, battery_data.h)
- `lib/` - Custom libraries directory
- `test/` - Unit tests directory
- `platformio.ini` - PlatformIO configuration file
- `README.md` - Project overview and build instructions
- `ASSEMBLY_NOTES.md` - Mechanical assembly documentation
- `CHANGELOG.md` - Version history and changes

## Version Numbering

This project uses Semantic Versioning (SemVer):
- **MAJOR** version: Incompatible API changes or major feature rewrites
- **MINOR** version: New functionality added in a backwards compatible manner
- **PATCH** version: Backwards compatible bug fixes

## Release Notes Format

Each release includes:
- **Added**: New features
- **Changed**: Changes in existing functionality
- **Deprecated**: Soon-to-be removed features
- **Removed**: Removed features
- **Fixed**: Bug fixes
- **Security**: Security vulnerability fixes

---

For older versions or detailed commit history, please refer to the Git repository.
