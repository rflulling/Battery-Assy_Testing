# Changelog

All notable changes to the Battery Assembly Testing System will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- Implement battery voltage measurement
- Implement current sensing and monitoring
- Add temperature monitoring
- Implement safety shutdown logic
- Add LCD/OLED display support
- Create web interface for remote monitoring (ESP32)
- Add data logging to SD card or flash memory
- Implement multiple battery channel support
- Add battery charge/discharge cycling capabilities

## [1.0.0] - 2025-10-19

### Added
- Initial PlatformIO project structure
- Support for ESP32 as primary microcontroller platform
- Support for STM32H7 as alternative platform
- Experimental support for Milk-V Duo 64
- Basic firmware skeleton with hardware initialization
- Assembly documentation with mechanical build instructions
- Safety guidelines and considerations
- Bill of Materials template
- Maintenance schedule documentation
- Multi-platform configuration in platformio.ini

### Project Structure
- `src/` - Source code directory with main.cpp
- `include/` - Header files directory
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
