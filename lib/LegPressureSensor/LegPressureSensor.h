/*
 * -------------------------------------------------------------------------
 * Class for the management of pressure sensors at the end of a robot's legs
 * © 2022 Nao974
 *  https://github.com/Nao974/LegPressureSensor
 * -------------------------------------------------------------------------
 * The pressure sensor at the end of the leg will allow you to know:
 *    If it hits the ground (Boolean True / False) and 
 *    The force exerted on it
 *    And at an instant "t" the direction of the force exerted (pressure // release)
 * -------------------------------------------------------------------------
 */

#pragma once

#include <Arduino.h>

// Number of measurements for calculating the average
const uint16_t CALIBRATE_NB = 100;
const float CALIBRATE_STANDARD_DEVIATION = 350.0; // Max standard value - Min standard value
// Deviation from the calibration validating a change of state
const uint8_t TRIGGER_PRESSURE = 10;
// Deviation from the calibration validating a change of direction
const uint8_t TRIGGER_DIRECTION = 5;

// The different possible states
const uint8_t STATE_RELEASED = 0;
const uint8_t STATE_PRESSED = 1;

// The different possible directions
const int8_t DIR_RELEASE = -1;
const int8_t DIR_FLAT = 0;
const int8_t DIR_PRESSURE = +1;

class LegPressureSensor {

    private:

        uint8_t _pinPC; // Pin of the µC board where the Presure Sensor is connected
        uint8_t _pinLed; // Pin of the µC board where the Leds is connected
        int16_t _calibrateLow, _calibrateHigh; // Storage of the measured calibration
        float _normalizeFactor; // normalization factor 
        uint8_t _state; // Current state of the leg
        int8_t _direction; // Current direction of movement
        int16_t _pressure_value, _pressure_valueNz; // Current pressure value and normalized
        int16_t  _pressure_max, _pressure_maxNz; // Max pressure and Max pressure normalized measured as long as the tab is pressed
        int16_t _old_pressure_value; // variable to calculate the direction

        // Update of the leg status according to the measured pressure
        void updateState(); 

    public:
        //LegPressureSensor(const uint8_t pinPC, const uint8_t pinLed); // Constructor

        void init(const uint8_t pinPC, const uint8_t pinLed);  // Initialization
        
        void calibrateLow(); // Calculation of the calibration in the air  
        uint16_t calibrateLowValue(); // Return the calculated released pressure     
        void calibrateLowWrite(uint16_t value); // Set value released pressure
                
        void calibrateHigh(); // Calculation of the calibration press max         
        uint16_t calibrateHighValue(); // Return the calculated released pressure      
        void calibrateHighWrite(uint16_t value); // Set value max pressure

        void calculateNormalizeFactor(); // Calculation of the normalization factor  
        float normalizeFactorValue(); // Return the calculated released pressure                
        void normalizeFactorWrite(float value); // Set Ratio Calibration
        
        void read(); // Pressure reading and state update
        void readNz(); // Pressure reading, NormaliZation and state update
        
        int16_t pressureValue(); // Return the last measured pressure      
        int16_t pressureValueNz(); // Return the last measured pressure normalized
        
        int16_t pressureMax(); // Return the current max pressure
        int16_t pressureMaxNz(); // Return the current max pressure normalized
        
        int8_t direction(); // Return the direction of movement      
        bool isPressed(); // Returns True / False if pressed

        void stateReset(); // Reset state at Release
};
