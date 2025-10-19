# Custom Libraries

This directory is for custom project libraries.

## Adding Libraries

Place your custom libraries in this directory. Each library should have its own subdirectory with the following structure:

```
lib/
└── MyLibrary/
    ├── src/
    │   ├── MyLibrary.cpp
    │   └── MyLibrary.h
    ├── examples/
    │   └── MyLibraryExample.ino
    ├── keywords.txt
    ├── library.properties
    └── README.md
```

## Library Dependencies

PlatformIO will automatically handle library dependencies specified in `platformio.ini`. For example:

```ini
lib_deps =
    Wire
    SPI
    adafruit/Adafruit GFX Library
```

## Platform-Specific Libraries

You can specify libraries for specific platforms:

```ini
[env:esp32dev]
lib_deps =
    ${env.lib_deps}
    ESP32-specific-library

[env:stm32h7]
lib_deps =
    ${env.lib_deps}
    STM32-specific-library
```

## Documentation

For more information about PlatformIO library management:
- https://docs.platformio.org/en/latest/librarymanager/
- https://docs.platformio.org/en/latest/projectconf/section_env_library.html
