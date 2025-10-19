/**
 * @file main.cpp
 * @brief Battery Assembly Testing System - Main Entry Point
 * @version 1.0.0
 * @date 2025-10-19
 * 
 * Main firmware for battery assembly testing apparatus.
 * Hardware: ESP32-POE-ISO (Olimex) with WROVER module
 * 
 * Features:
 * - Dual INA3221 current monitoring (charge/discharge shunts)
 * - DS3231 RTC for accurate timekeeping
 * - Relay control for charge/discharge cycling
 * - CSV data logging
 * - Real-time serial monitoring (PuTTY compatible)
 */

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <INA3221.h>
#include "config.h"
#include "battery_data.h"

// ============================================================================
// Global Objects
// ============================================================================
RTC_DS3231 rtc;
INA3221 inaCharge(INA3221_ADDR_CHARGE);
INA3221 inaDischarge(INA3221_ADDR_DISCHARGE);

// Battery test data
BatteryTestData testData;
MovingAverage avgChargeCurrentFilter;
MovingAverage avgDischargeCurrentFilter;

// Timing variables
unsigned long lastUpdateTime = 0;
unsigned long lastDisplayTime = 0;
unsigned long lastLogTime = 0;

// ============================================================================
// Function Prototypes
// ============================================================================
void setup();
void loop();
void initializeHardware();
void initializeI2C();
void initializeRTC();
void initializeAmmeters();
void initializeRelays();
void initializeLEDs();
void printSystemInfo();
void printHelp();
void handleSerialCommands();
void updateMeasurements();
void displayRealTimeData();
void controlRelays();
void setRelayState(int pin, bool state);
void startTest(BatteryType type);
void stopTest();
void runTestStateMachine();
void logDataToCSV();
void printTestSummary();
const char* getBatteryTypeName(BatteryType type);
const char* getStateName(TestState state);

// ============================================================================
// Setup Function
// ============================================================================
void setup() {
    // Initialize serial communication
    Serial.begin(SERIAL_BAUD);
    while (!Serial && millis() < 3000) {
        delay(10);
    }
    
    Serial.println("\n========================================");
    Serial.println("  Battery Assembly Testing System");
    Serial.println("  ESP32-POE-ISO (Olimex)");
    Serial.println("========================================\n");
    
    // Initialize hardware
    initializeHardware();
    
    // Print system information
    printSystemInfo();
    
    // Print help
    printHelp();
    
    Serial.println("\nSystem initialized successfully!");
    Serial.println("Ready for battery testing operations.\n");
}

// ============================================================================
// Main Loop
// ============================================================================
void loop() {
    unsigned long currentTime = millis();
    
    // Handle serial commands
    handleSerialCommands();
    
    // Update measurements at regular intervals
    if (currentTime - lastUpdateTime >= UPDATE_INTERVAL_MS) {
        lastUpdateTime = currentTime;
        updateMeasurements();
    }
    
    // Update display at regular intervals
    if (currentTime - lastDisplayTime >= DISPLAY_UPDATE_MS) {
        lastDisplayTime = currentTime;
        displayRealTimeData();
    }
    
    // Run test state machine
    if (testData.testActive) {
        runTestStateMachine();
        
        // Log data to CSV at regular intervals
        if (currentTime - lastLogTime >= LOG_INTERVAL_MS) {
            lastLogTime = currentTime;
            logDataToCSV();
        }
    }
}

// ============================================================================
// Hardware Initialization Functions
// ============================================================================
void initializeHardware() {
    Serial.println("Initializing hardware...");
    
    initializeI2C();
    initializeRTC();
    initializeAmmeters();
    initializeRelays();
    initializeLEDs();
    
    Serial.println("Hardware initialization complete.\n");
}

void initializeI2C() {
    Serial.print("  Initializing I2C bus... ");
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Wire.setClock(I2C_FREQUENCY);
    Serial.println("OK");
}

void initializeRTC() {
    Serial.print("  Initializing DS3231 RTC... ");
    if (!rtc.begin()) {
        Serial.println("FAILED!");
        Serial.println("    ERROR: Could not find DS3231 RTC!");
        Serial.println("    Check I2C connections (SDA=13, SCL=16)");
    } else {
        Serial.println("OK");
        
        if (rtc.lostPower()) {
            Serial.println("    WARNING: RTC lost power, setting time...");
            // Set to compile time as default
            rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        }
        
        DateTime now = rtc.now();
        char buffer[32];
        sprintf(buffer, "    Current time: %04d-%02d-%02d %02d:%02d:%02d",
                now.year(), now.month(), now.day(),
                now.hour(), now.minute(), now.second());
        Serial.println(buffer);
    }
}

void initializeAmmeters() {
    Serial.print("  Initializing INA3221 ammeters... ");
    
    // Initialize charge ammeter
    inaCharge.begin(&Wire);
    inaCharge.reset();
    inaCharge.setShuntRes(SHUNT_RESISTANCE_CHARGE, SHUNT_RESISTANCE_CHARGE, SHUNT_RESISTANCE_CHARGE);
    
    // Initialize discharge ammeter
    inaDischarge.begin(&Wire);
    inaDischarge.reset();
    inaDischarge.setShuntRes(SHUNT_RESISTANCE_DISCHARGE, SHUNT_RESISTANCE_DISCHARGE, SHUNT_RESISTANCE_DISCHARGE);
    
    Serial.println("OK");
    Serial.println("    Charge shunt: Channel 1");
    Serial.println("    Discharge shunt: Channel 1");
}

void initializeRelays() {
    Serial.print("  Initializing relay control... ");
    
    pinMode(RELAY_MAIN_PIN, OUTPUT);
    pinMode(RELAY_AC_CHARGER_PIN, OUTPUT);
    pinMode(RELAY_AC_DISCHARGE_PIN, OUTPUT);
    pinMode(RELAY_CHARGER_SELECT_PIN, OUTPUT);
    
    // Set all relays to safe state (OFF)
    setRelayState(RELAY_MAIN_PIN, RELAY_DISCHARGE_POSITION);  // Prevent charging
    setRelayState(RELAY_AC_CHARGER_PIN, RELAY_OFF);
    setRelayState(RELAY_AC_DISCHARGE_PIN, RELAY_OFF);
    setRelayState(RELAY_CHARGER_SELECT_PIN, RELAY_OFF);
    
    Serial.println("OK");
    Serial.println("    All relays set to safe state (OFF)");
}

void initializeLEDs() {
    Serial.print("  Initializing status LEDs... ");
    
    pinMode(LED_STATUS_PIN, OUTPUT);
    pinMode(LED_CHARGING_PIN, OUTPUT);
    pinMode(LED_DISCHARGING_PIN, OUTPUT);
    
    digitalWrite(LED_STATUS_PIN, LOW);
    digitalWrite(LED_CHARGING_PIN, LOW);
    digitalWrite(LED_DISCHARGING_PIN, LOW);
    
    Serial.println("OK");
}

// ============================================================================
// System Information
// ============================================================================
void printSystemInfo() {
    Serial.println("System Information:");
    Serial.print("  Version: ");
    Serial.println(VERSION);
    Serial.print("  Project: ");
    Serial.println(PROJECT_NAME);
    
    #ifdef ESP32
    Serial.println("  Platform: ESP32-POE-ISO");
    Serial.print("  Chip Model: ");
    Serial.println(ESP.getChipModel());
    Serial.print("  Chip Revision: ");
    Serial.println(ESP.getChipRevision());
    Serial.print("  CPU Frequency: ");
    Serial.print(ESP.getCpuFreqMHz());
    Serial.println(" MHz");
    Serial.print("  Flash Size: ");
    Serial.print(ESP.getFlashChipSize() / (1024 * 1024));
    Serial.println(" MB");
    #endif
    Serial.println();
}

void printHelp() {
    Serial.println("Available Commands:");
    Serial.println("  help        - Show this help message");
    Serial.println("  status      - Show current system status");
    Serial.println("  start <n>   - Start test (0=Lead Acid, 1=LiFePO4, 2=NiCad, 3=Li-ion)");
    Serial.println("  stop        - Stop current test");
    Serial.println("  summary     - Print test summary");
    Serial.println("  time        - Show current RTC time");
    Serial.println("  settime     - Set RTC time to compile time");
    Serial.println();
}

// ============================================================================
// Serial Command Handler
// ============================================================================
void handleSerialCommands() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        if (command.length() == 0) return;
        
        Serial.print("\n> ");
        Serial.println(command);
        
        if (command.equals("help")) {
            printHelp();
        }
        else if (command.equals("status")) {
            Serial.println("\nCurrent Status:");
            Serial.print("  State: ");
            Serial.println(getStateName(testData.currentState));
            Serial.print("  Test Active: ");
            Serial.println(testData.testActive ? "Yes" : "No");
            Serial.print("  Battery Type: ");
            Serial.println(getBatteryTypeName(testData.batteryType));
            Serial.println();
        }
        else if (command.startsWith("start ")) {
            int typeNum = command.substring(6).toInt();
            if (typeNum >= 0 && typeNum <= 3) {
                startTest((BatteryType)typeNum);
            } else {
                Serial.println("ERROR: Invalid battery type. Use 0-3.");
            }
        }
        else if (command.equals("stop")) {
            stopTest();
        }
        else if (command.equals("summary")) {
            printTestSummary();
        }
        else if (command.equals("time")) {
            DateTime now = rtc.now();
            char buffer[32];
            sprintf(buffer, "RTC Time: %04d-%02d-%02d %02d:%02d:%02d",
                    now.year(), now.month(), now.day(),
                    now.hour(), now.minute(), now.second());
            Serial.println(buffer);
        }
        else if (command.equals("settime")) {
            rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
            Serial.println("RTC time set to compile time.");
        }
        else {
            Serial.println("ERROR: Unknown command. Type 'help' for available commands.");
        }
    }
}

// ============================================================================
// Measurement Functions
// ============================================================================
void updateMeasurements() {
    // Read charge current (Channel 1 of charge INA3221)
    float chargeCurrent = inaCharge.getCurrent_mA(INA3221_CH1) / 1000.0;  // Convert mA to A
    if (chargeCurrent < 0) chargeCurrent = 0;  // Prevent negative readings
    
    // Read discharge current (Channel 1 of discharge INA3221)
    float dischargeCurrent = inaDischarge.getCurrent_mA(INA3221_CH1) / 1000.0;
    if (dischargeCurrent < 0) dischargeCurrent = 0;
    
    // Read voltage (from charge INA3221)
    float voltage = inaCharge.getBusVoltage_V(INA3221_CH1);
    
    // Update test data
    testData.currentCharge = chargeCurrent;
    testData.currentDischarge = dischargeCurrent;
    testData.voltage = voltage;
    
    // Update averages
    avgChargeCurrentFilter.addSample(chargeCurrent);
    avgDischargeCurrentFilter.addSample(dischargeCurrent);
    testData.currentChargeAvg = avgChargeCurrentFilter.getAverage();
    testData.currentDischargeAvg = avgDischargeCurrentFilter.getAverage();
    
    // Update peaks
    if (chargeCurrent > testData.currentChargePeak) {
        testData.currentChargePeak = chargeCurrent;
    }
    if (dischargeCurrent > testData.currentDischargePeak) {
        testData.currentDischargePeak = dischargeCurrent;
    }
    if (voltage > testData.voltagePeak) {
        testData.voltagePeak = voltage;
    }
    
    // Accumulate Ah and Wh
    float deltaTimeHours = UPDATE_INTERVAL_MS / 3600000.0;  // Convert ms to hours
    
    if (testData.currentState == STATE_CHARGING || testData.currentState == STATE_PRECHARGE) {
        testData.totalChargeAh += chargeCurrent * deltaTimeHours;
        testData.totalChargeWh += chargeCurrent * voltage * deltaTimeHours;
    }
    
    if (testData.currentState == STATE_DISCHARGING) {
        testData.totalDischargeAh += dischargeCurrent * deltaTimeHours;
        testData.totalDischargeWh += dischargeCurrent * voltage * deltaTimeHours;
    }
}

// ============================================================================
// Display Functions
// ============================================================================
void displayRealTimeData() {
    if (!testData.testActive) return;
    
    // Clear screen (ANSI escape code)
    Serial.print("\033[2J\033[H");
    
    // Header
    Serial.println("========================================");
    Serial.println("  BATTERY TEST MONITOR");
    Serial.println("========================================");
    
    // RTC Time
    DateTime now = rtc.now();
    char timeBuffer[32];
    sprintf(timeBuffer, "Time: %04d-%02d-%02d %02d:%02d:%02d",
            now.year(), now.month(), now.day(),
            now.hour(), now.minute(), now.second());
    Serial.println(timeBuffer);
    
    // State and battery info
    Serial.print("State: ");
    Serial.println(getStateName(testData.currentState));
    Serial.print("Battery: ");
    Serial.println(getBatteryTypeName(testData.batteryType));
    
    // Running time
    char durationBuffer[16];
    testData.formatDuration(testData.getCurrentDuration(), durationBuffer);
    Serial.print("Running Time: ");
    Serial.println(durationBuffer);
    
    Serial.println("----------------------------------------");
    
    // Voltage
    Serial.print("Voltage:     ");
    Serial.print(testData.voltage, 3);
    Serial.print(" V  (Peak: ");
    Serial.print(testData.voltagePeak, 3);
    Serial.println(" V)");
    
    // Charge current
    Serial.print("Charge:      ");
    Serial.print(testData.currentCharge, 3);
    Serial.print(" A  (Avg: ");
    Serial.print(testData.currentChargeAvg, 3);
    Serial.print(" A, Peak: ");
    Serial.print(testData.currentChargePeak, 3);
    Serial.println(" A)");
    
    // Discharge current
    Serial.print("Discharge:   ");
    Serial.print(testData.currentDischarge, 3);
    Serial.print(" A  (Avg: ");
    Serial.print(testData.currentDischargeAvg, 3);
    Serial.print(" A, Peak: ");
    Serial.print(testData.currentDischargePeak, 3);
    Serial.println(" A)");
    
    Serial.println("----------------------------------------");
    
    // Totals
    Serial.print("Total Charge:    ");
    Serial.print(testData.totalChargeAh, 3);
    Serial.print(" Ah  (");
    Serial.print(testData.totalChargeWh, 2);
    Serial.println(" Wh)");
    
    Serial.print("Total Discharge: ");
    Serial.print(testData.totalDischargeAh, 3);
    Serial.print(" Ah  (");
    Serial.print(testData.totalDischargeWh, 2);
    Serial.println(" Wh)");
    
    Serial.println("========================================");
    Serial.println("Commands: help, status, stop, summary");
    Serial.println();
}

// ============================================================================
// Relay Control Functions
// ============================================================================
void setRelayState(int pin, bool state) {
    digitalWrite(pin, state ? RELAY_ON : RELAY_OFF);
}

void controlRelays() {
    // Control relays based on current state
    switch (testData.currentState) {
        case STATE_IDLE:
        case STATE_ERROR:
            // All relays off
            setRelayState(RELAY_MAIN_PIN, RELAY_DISCHARGE_POSITION);
            setRelayState(RELAY_AC_CHARGER_PIN, RELAY_OFF);
            setRelayState(RELAY_AC_DISCHARGE_PIN, RELAY_OFF);
            digitalWrite(LED_CHARGING_PIN, LOW);
            digitalWrite(LED_DISCHARGING_PIN, LOW);
            digitalWrite(LED_STATUS_PIN, LOW);
            break;
            
        case STATE_PRECHARGE:
        case STATE_CHARGING:
            // Enable charger
            setRelayState(RELAY_MAIN_PIN, RELAY_CHARGE_POSITION);
            setRelayState(RELAY_AC_CHARGER_PIN, RELAY_ON);
            setRelayState(RELAY_AC_DISCHARGE_PIN, RELAY_OFF);
            digitalWrite(LED_CHARGING_PIN, HIGH);
            digitalWrite(LED_DISCHARGING_PIN, LOW);
            digitalWrite(LED_STATUS_PIN, HIGH);
            break;
            
        case STATE_DISCHARGING:
            // Enable discharge
            setRelayState(RELAY_MAIN_PIN, RELAY_DISCHARGE_POSITION);
            setRelayState(RELAY_AC_CHARGER_PIN, RELAY_OFF);
            setRelayState(RELAY_AC_DISCHARGE_PIN, RELAY_ON);
            digitalWrite(LED_CHARGING_PIN, LOW);
            digitalWrite(LED_DISCHARGING_PIN, HIGH);
            digitalWrite(LED_STATUS_PIN, HIGH);
            break;
            
        default:
            // Safe state during transitions
            setRelayState(RELAY_AC_CHARGER_PIN, RELAY_OFF);
            setRelayState(RELAY_AC_DISCHARGE_PIN, RELAY_OFF);
            digitalWrite(LED_STATUS_PIN, HIGH);
            break;
    }
}

// ============================================================================
// Test Control Functions
// ============================================================================
void startTest(BatteryType type) {
    Serial.println("\n========================================");
    Serial.println("STARTING BATTERY TEST");
    Serial.println("========================================");
    
    // Reset test data
    testData.reset();
    testData.batteryType = type;
    testData.testActive = true;
    testData.currentState = STATE_PRECHARGE;
    testData.testStartTime = millis();
    testData.voltageStart = testData.voltage;
    
    // Reset filters
    avgChargeCurrentFilter.reset();
    avgDischargeCurrentFilter.reset();
    
    // Set battery ID
    DateTime now = rtc.now();
    sprintf(testData.batteryId, "%s_%04d%02d%02d_%02d%02d%02d",
            getBatteryTypeName(type),
            now.year(), now.month(), now.day(),
            now.hour(), now.minute(), now.second());
    
    Serial.print("Battery Type: ");
    Serial.println(getBatteryTypeName(type));
    Serial.print("Battery ID: ");
    Serial.println(testData.batteryId);
    Serial.println("========================================\n");
    
    // Start with precharge state
    testData.chargeStartTime = millis();
    controlRelays();
}

void stopTest() {
    Serial.println("\n========================================");
    Serial.println("STOPPING BATTERY TEST");
    Serial.println("========================================");
    
    testData.testActive = false;
    testData.currentState = STATE_IDLE;
    testData.voltageEnd = testData.voltage;
    
    // Turn off all relays
    controlRelays();
    
    Serial.println("Test stopped.");
    Serial.println("========================================\n");
    
    // Print summary
    printTestSummary();
}

// ============================================================================
// Test State Machine
// ============================================================================
void runTestStateMachine() {
    // Check for safety conditions
    if (testData.voltage < MIN_VOLTAGE_CUTOFF && testData.currentState != STATE_IDLE) {
        Serial.println("\nERROR: Voltage below minimum cutoff!");
        testData.errorFlag = true;
        strcpy(testData.errorMessage, "Voltage too low");
        testData.currentState = STATE_ERROR;
        stopTest();
        return;
    }
    
    // Check maximum test duration
    if (testData.getCurrentDuration() > (MAX_TEST_DURATION_HOURS * 3600)) {
        Serial.println("\nWARNING: Maximum test duration exceeded!");
        testData.currentState = STATE_TEST_COMPLETE;
        stopTest();
        return;
    }
    
    // State machine logic
    switch (testData.currentState) {
        case STATE_PRECHARGE:
            // Check if charge current drops below threshold (charging complete)
            if (testData.currentCharge < 0.1 && (millis() - testData.chargeStartTime) > 60000) {
                // Charging appears complete
                testData.chargeEndTime = millis();
                testData.chargeDuration = (testData.chargeEndTime - testData.chargeStartTime) / 1000;
                testData.currentState = STATE_CHARGE_COMPLETE;
                Serial.println("\nPrecharge complete.");
                controlRelays();
            }
            break;
            
        case STATE_CHARGE_COMPLETE:
            // Transition to discharge ready
            delay(2000);  // Brief delay
            testData.currentState = STATE_DISCHARGE_READY;
            Serial.println("\nPreparing for discharge...");
            controlRelays();
            break;
            
        case STATE_DISCHARGE_READY:
            // Start discharge
            testData.dischargeStartTime = millis();
            testData.currentState = STATE_DISCHARGING;
            Serial.println("\nDischarge started.");
            controlRelays();
            break;
            
        case STATE_DISCHARGING:
            // Check discharge voltage cutoff based on battery type
            float cutoffVoltage = LEAD_ACID_DISCHARGE_CUTOFF;
            switch (testData.batteryType) {
                case BATTERY_LIFEPO4:
                    cutoffVoltage = LIFEPO4_DISCHARGE_CUTOFF;
                    break;
                case BATTERY_NICAD_4xAA:
                    cutoffVoltage = NICAD_DISCHARGE_CUTOFF;
                    break;
                case BATTERY_LIION_CELL:
                    cutoffVoltage = LIION_DISCHARGE_CUTOFF;
                    break;
                default:
                    cutoffVoltage = LEAD_ACID_DISCHARGE_CUTOFF;
                    break;
            }
            
            if (testData.voltage <= cutoffVoltage) {
                testData.dischargeEndTime = millis();
                testData.dischargeDuration = (testData.dischargeEndTime - testData.dischargeStartTime) / 1000;
                testData.currentState = STATE_DISCHARGE_COMPLETE;
                Serial.println("\nDischarge complete (cutoff voltage reached).");
                controlRelays();
            }
            break;
            
        case STATE_DISCHARGE_COMPLETE:
            // Test complete
            testData.totalDuration = testData.getCurrentDuration();
            testData.currentState = STATE_TEST_COMPLETE;
            Serial.println("\nTest cycle complete!");
            stopTest();
            break;
            
        default:
            break;
    }
    
    // Update relay states
    controlRelays();
}

// ============================================================================
// Data Logging
// ============================================================================
void logDataToCSV() {
    // Print CSV-formatted data to serial
    // Header format: Timestamp, State, Voltage, ChargeCurrent, DischargeCurrent, TotalChargeAh, TotalDischargeAh
    
    DateTime now = rtc.now();
    char timestamp[32];
    sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d",
            now.year(), now.month(), now.day(),
            now.hour(), now.minute(), now.second());
    
    Serial.print("[CSV] ");
    Serial.print(timestamp);
    Serial.print(",");
    Serial.print(testData.batteryId);
    Serial.print(",");
    Serial.print(getStateName(testData.currentState));
    Serial.print(",");
    Serial.print(testData.voltage, 3);
    Serial.print(",");
    Serial.print(testData.currentCharge, 3);
    Serial.print(",");
    Serial.print(testData.currentDischarge, 3);
    Serial.print(",");
    Serial.print(testData.totalChargeAh, 3);
    Serial.print(",");
    Serial.print(testData.totalDischargeAh, 3);
    Serial.print(",");
    Serial.print(testData.totalChargeWh, 2);
    Serial.print(",");
    Serial.print(testData.totalDischargeWh, 2);
    Serial.println();
}

// ============================================================================
// Reporting Functions
// ============================================================================
void printTestSummary() {
    Serial.println("\n========================================");
    Serial.println("  BATTERY TEST SUMMARY");
    Serial.println("========================================");
    
    Serial.print("Battery ID: ");
    Serial.println(testData.batteryId);
    Serial.print("Battery Type: ");
    Serial.println(getBatteryTypeName(testData.batteryType));
    
    Serial.println("\n--- Voltage ---");
    Serial.print("Start: ");
    Serial.print(testData.voltageStart, 3);
    Serial.println(" V");
    Serial.print("End:   ");
    Serial.print(testData.voltageEnd, 3);
    Serial.println(" V");
    Serial.print("Peak:  ");
    Serial.print(testData.voltagePeak, 3);
    Serial.println(" V");
    
    Serial.println("\n--- Charge Data ---");
    char chargeDurationStr[16];
    testData.formatDuration(testData.chargeDuration, chargeDurationStr);
    Serial.print("Duration:      ");
    Serial.println(chargeDurationStr);
    Serial.print("Average:       ");
    Serial.print(testData.currentChargeAvg, 3);
    Serial.println(" A");
    Serial.print("Peak:          ");
    Serial.print(testData.currentChargePeak, 3);
    Serial.println(" A");
    Serial.print("Total Charge:  ");
    Serial.print(testData.totalChargeAh, 3);
    Serial.print(" Ah (");
    Serial.print(testData.totalChargeWh, 2);
    Serial.println(" Wh)");
    
    Serial.println("\n--- Discharge Data ---");
    char dischargeDurationStr[16];
    testData.formatDuration(testData.dischargeDuration, dischargeDurationStr);
    Serial.print("Duration:      ");
    Serial.println(dischargeDurationStr);
    Serial.print("Average:       ");
    Serial.print(testData.currentDischargeAvg, 3);
    Serial.println(" A");
    Serial.print("Peak:          ");
    Serial.print(testData.currentDischargePeak, 3);
    Serial.println(" A");
    Serial.print("Total Charge:  ");
    Serial.print(testData.totalDischargeAh, 3);
    Serial.print(" Ah (");
    Serial.print(testData.totalDischargeWh, 2);
    Serial.println(" Wh)");
    
    Serial.println("\n--- Overall ---");
    char totalDurationStr[16];
    testData.formatDuration(testData.totalDuration, totalDurationStr);
    Serial.print("Total Duration: ");
    Serial.println(totalDurationStr);
    
    if (testData.errorFlag) {
        Serial.print("Error: ");
        Serial.println(testData.errorMessage);
    }
    
    Serial.println("========================================\n");
}

// ============================================================================
// Helper Functions
// ============================================================================
const char* getBatteryTypeName(BatteryType type) {
    switch (type) {
        case BATTERY_LEAD_ACID:    return "Lead Acid";
        case BATTERY_LIFEPO4:      return "LiFePO4";
        case BATTERY_NICAD_4xAA:   return "NiCad 4xAA";
        case BATTERY_LIION_CELL:   return "Li-ion Cell";
        default:                   return "Unknown";
    }
}

const char* getStateName(TestState state) {
    switch (state) {
        case STATE_IDLE:              return "Idle";
        case STATE_PRECHARGE:         return "Precharge";
        case STATE_CHARGING:          return "Charging";
        case STATE_CHARGE_COMPLETE:   return "Charge Complete";
        case STATE_DISCHARGE_READY:   return "Discharge Ready";
        case STATE_DISCHARGING:       return "Discharging";
        case STATE_DISCHARGE_COMPLETE: return "Discharge Complete";
        case STATE_TEST_COMPLETE:     return "Test Complete";
        case STATE_ERROR:             return "Error";
        default:                      return "Unknown";
    }
}
