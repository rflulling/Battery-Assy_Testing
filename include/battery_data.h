/**
 * @file battery_data.h
 * @brief Data structures for battery testing
 * @version 1.0.0
 * @date 2025-10-19
 */

#ifndef BATTERY_DATA_H
#define BATTERY_DATA_H

#include <Arduino.h>
#include "config.h"

// ============================================================================
// Battery Test Data Structure
// ============================================================================
struct BatteryTestData {
    // Battery identification
    BatteryType batteryType;
    char batteryId[32];
    
    // Voltage measurements
    float voltage;              // Current voltage (V)
    float voltageStart;         // Starting voltage (V)
    float voltageEnd;           // Ending voltage (V)
    float voltagePeak;          // Peak voltage during test (V)
    
    // Current measurements (Charge)
    float currentCharge;        // Real-time charge current (A)
    float currentChargeAvg;     // Average charge current (A)
    float currentChargePeak;    // Peak charge current (A)
    float totalChargeAh;        // Total charge accumulated (Ah)
    float totalChargeWh;        // Total charge energy (Wh)
    
    // Current measurements (Discharge)
    float currentDischarge;     // Real-time discharge current (A)
    float currentDischargeAvg;  // Average discharge current (A)
    float currentDischargePeak; // Peak discharge current (A)
    float totalDischargeAh;     // Total discharge accumulated (Ah)
    float totalDischargeWh;     // Total discharge energy (Wh)
    
    // Time tracking
    unsigned long testStartTime;    // Test start timestamp (ms)
    unsigned long chargeStartTime;  // Charge start timestamp (ms)
    unsigned long chargeEndTime;    // Charge end timestamp (ms)
    unsigned long dischargeStartTime; // Discharge start timestamp (ms)
    unsigned long dischargeEndTime;   // Discharge end timestamp (ms)
    
    // Duration (seconds)
    unsigned long chargeDuration;
    unsigned long dischargeDuration;
    unsigned long totalDuration;
    
    // State information
    TestState currentState;
    bool testActive;
    bool errorFlag;
    char errorMessage[64];
    
    // Temperature (if available)
    float temperature;
    
    // Constructor
    BatteryTestData() {
        reset();
    }
    
    // Reset all data
    void reset() {
        batteryType = BATTERY_LEAD_ACID;
        strcpy(batteryId, "UNKNOWN");
        
        voltage = 0.0;
        voltageStart = 0.0;
        voltageEnd = 0.0;
        voltagePeak = 0.0;
        
        currentCharge = 0.0;
        currentChargeAvg = 0.0;
        currentChargePeak = 0.0;
        totalChargeAh = 0.0;
        totalChargeWh = 0.0;
        
        currentDischarge = 0.0;
        currentDischargeAvg = 0.0;
        currentDischargePeak = 0.0;
        totalDischargeAh = 0.0;
        totalDischargeWh = 0.0;
        
        testStartTime = 0;
        chargeStartTime = 0;
        chargeEndTime = 0;
        dischargeStartTime = 0;
        dischargeEndTime = 0;
        
        chargeDuration = 0;
        dischargeDuration = 0;
        totalDuration = 0;
        
        currentState = STATE_IDLE;
        testActive = false;
        errorFlag = false;
        strcpy(errorMessage, "");
        
        temperature = 0.0;
    }
    
    // Format elapsed time as HH:MM:SS
    void formatDuration(unsigned long seconds, char* buffer) {
        unsigned long hours = seconds / 3600;
        unsigned long minutes = (seconds % 3600) / 60;
        unsigned long secs = seconds % 60;
        sprintf(buffer, "%02lu:%02lu:%02lu", hours, minutes, secs);
    }
    
    // Get current test duration in seconds
    unsigned long getCurrentDuration() {
        if (testStartTime == 0) return 0;
        return (millis() - testStartTime) / 1000;
    }
};

// ============================================================================
// Moving Average Filter
// ============================================================================
class MovingAverage {
private:
    float samples[AVERAGING_SAMPLES];
    int index;
    int count;
    float sum;
    
public:
    MovingAverage() : index(0), count(0), sum(0.0) {
        for (int i = 0; i < AVERAGING_SAMPLES; i++) {
            samples[i] = 0.0;
        }
    }
    
    void addSample(float value) {
        sum -= samples[index];
        samples[index] = value;
        sum += value;
        index = (index + 1) % AVERAGING_SAMPLES;
        if (count < AVERAGING_SAMPLES) count++;
    }
    
    float getAverage() {
        if (count == 0) return 0.0;
        return sum / count;
    }
    
    void reset() {
        for (int i = 0; i < AVERAGING_SAMPLES; i++) {
            samples[i] = 0.0;
        }
        index = 0;
        count = 0;
        sum = 0.0;
    }
};

#endif // BATTERY_DATA_H
