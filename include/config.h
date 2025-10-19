/**
 * @file config.h
 * @brief Configuration and pin definitions for Battery Testing System
 * @version 1.0.0
 * @date 2025-10-19
 * 
 * Hardware: ESP32-POE-ISO (Olimex) with WROVER module
 * Storage: 8MB or 16MB variants supported
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// I2C Configuration (ESP32-POE-ISO)
// ============================================================================
#define I2C_SDA_PIN 13
#define I2C_SCL_PIN 16
#define I2C_FREQUENCY 100000  // 100kHz standard mode

// ============================================================================
// INA3221 Ammeter Configuration
// ============================================================================
// Two INA3221 chips for monitoring charge and discharge paths
#define INA3221_ADDR_CHARGE 0x40    // First INA3221 for charge monitoring
#define INA3221_ADDR_DISCHARGE 0x41 // Second INA3221 for discharge monitoring

// Shunt resistor values (in Ohms)
#define SHUNT_RESISTANCE_CHARGE 0.01    // 10mΩ shunt for charge path
#define SHUNT_RESISTANCE_DISCHARGE 0.01 // 10mΩ shunt for discharge path

// Current measurement ranges
#define MAX_CURRENT_CHARGE 10.0     // Maximum charge current (A)
#define MAX_CURRENT_DISCHARGE 10.0  // Maximum discharge current (A)

// ============================================================================
// DS3231 RTC Configuration
// ============================================================================
#define RTC_I2C_ADDRESS 0x68  // DS3231 I2C address

// ============================================================================
// Relay Control Pins
// ============================================================================
#define RELAY_MAIN_PIN 32           // Main relay - charge/discharge selector
#define RELAY_AC_CHARGER_PIN 33     // AC power relay for charger
#define RELAY_AC_DISCHARGE_PIN 25   // AC power relay for discharge station
#define RELAY_CHARGER_SELECT_PIN 26 // Relay to switch between Lead Acid/Lithium chargers

// Relay states
#define RELAY_ON HIGH
#define RELAY_OFF LOW
#define RELAY_CHARGE_POSITION LOW
#define RELAY_DISCHARGE_POSITION HIGH

// ============================================================================
// Status LED Pins
// ============================================================================
#define LED_STATUS_PIN 2      // General status LED (built-in on many ESP32)
#define LED_CHARGING_PIN 27   // Charging indicator LED
#define LED_DISCHARGING_PIN 14 // Discharging indicator LED

// ============================================================================
// Battery Types
// ============================================================================
enum BatteryType {
    BATTERY_LEAD_ACID,      // Small lead acid motorcycle battery (~13Ah)
    BATTERY_LIFEPO4,        // Small LiFePO4 battery (~90Ah)
    BATTERY_NICAD_4xAA,     // Pack of 4x AA NiCad cells
    BATTERY_LIION_CELL      // Various sizes of Lithium Ion cells
};

// ============================================================================
// Battery Specifications
// ============================================================================
// Lead Acid Motorcycle Battery
#define LEAD_ACID_NOMINAL_VOLTAGE 12.0
#define LEAD_ACID_CAPACITY_AH 13.0
#define LEAD_ACID_CHARGE_VOLTAGE 14.4
#define LEAD_ACID_DISCHARGE_CUTOFF 10.5

// LiFePO4 Battery
#define LIFEPO4_NOMINAL_VOLTAGE 12.8
#define LIFEPO4_CAPACITY_AH 90.0
#define LIFEPO4_CHARGE_VOLTAGE 14.6
#define LIFEPO4_DISCHARGE_CUTOFF 10.0

// NiCad 4xAA Pack
#define NICAD_NOMINAL_VOLTAGE 4.8
#define NICAD_CAPACITY_AH 2.5
#define NICAD_CHARGE_VOLTAGE 6.0
#define NICAD_DISCHARGE_CUTOFF 4.0

// Li-ion Cell (generic)
#define LIION_NOMINAL_VOLTAGE 3.7
#define LIION_CAPACITY_AH 3.0
#define LIION_CHARGE_VOLTAGE 4.2
#define LIION_DISCHARGE_CUTOFF 2.8

// ============================================================================
// Test Cycle Configuration
// ============================================================================
#define UPDATE_INTERVAL_MS 1000     // Data update interval (1 second)
#define DISPLAY_UPDATE_MS 500       // Display refresh interval
#define LOG_INTERVAL_MS 5000        // CSV logging interval (5 seconds)

// Safety limits
#define MAX_TEST_DURATION_HOURS 48  // Maximum test duration
#define MAX_TEMPERATURE_C 60.0      // Maximum safe temperature
#define MIN_VOLTAGE_CUTOFF 0.5      // Minimum voltage before emergency stop

// ============================================================================
// Data Logging
// ============================================================================
#define CSV_BUFFER_SIZE 256
#define MAX_FILENAME_LENGTH 64

// ============================================================================
// Serial Configuration
// ============================================================================
#define SERIAL_BAUD 115200
#define SERIAL_TIMEOUT_MS 100

// ============================================================================
// Test State Machine
// ============================================================================
enum TestState {
    STATE_IDLE,
    STATE_PRECHARGE,
    STATE_CHARGING,
    STATE_CHARGE_COMPLETE,
    STATE_DISCHARGE_READY,
    STATE_DISCHARGING,
    STATE_DISCHARGE_COMPLETE,
    STATE_TEST_COMPLETE,
    STATE_ERROR
};

// ============================================================================
// Measurement Averaging
// ============================================================================
#define AVERAGING_SAMPLES 10        // Number of samples for moving average
#define PEAK_HOLD_TIME_MS 5000      // Time to hold peak values (5 seconds)

#endif // CONFIG_H
