/*
 * -------------------------------------------------------------------------
 * Class for the management of pressure sensors at the end of a robot's legs
 * © 2021 Nao974
 *  https://github.com/Nao974/LegPressureSensor
 * -------------------------------------------------------------------------
 * The pressure sensor at the end of the leg will allow you to know:
 *    If it hits the ground (Boolean True / False)
 *    The force exerted on it
 *    And at an instant "t" the direction of the force exerted (pressure // release)
 * -------------------------------------------------------------------------
 */

#pragma once

#include <Arduino.h>

// The different possible states
const uint8_t STATE_RELEASED = 0;
const uint8_t STATE_PRESSED = 1;

// The different possible directions
const int8_t DIR_RELEASE = -1;
const int8_t DIR_FLAT = 0;
const int8_t DIR_PRESSURE = +1;

// Number of measurements for calculating the average
const uint16_t CALIBRATE_NB = 100;

// Deviation from the calibration validating a change of state
const uint8_t TRIGGER_PRESSURE = 10;

// Deviation from the calibration validating a change of direction
const uint8_t TRIGGER_DIRECTION = 5;


class LegPressureSensor {

    private:

        uint8_t _pin; // Pin of the µC board where the sensor is connected
        int16_t _calibrate; // Storage of the measured calibration
        uint8_t _state; // Current state of the leg
        int8_t _direction; // Current direction of movement
        int16_t _pressure_value; // Current pressure value
        int16_t  _pressure_max; // Max pressure measured as long as the tab is pressed
        int16_t _old_pressure_value; // variable to calculate the direction

        // Update of the leg status according to the measured pressure
        void updateState(); 

    public:
        //Constructor
        LegPressureSensor(const uint8_t pin);

        // Calculation of the calibration
        void calibrate();   

        // Pressure reading and state update
        void read();
 
        // Return the last measured pressure
        uint16_t pressureValue();

        // Return the calculated released pressure
        uint16_t calibrateValue();

        // Return the current max pressure
        uint16_t pressureMax();

        // Return the direction of movement
        int8_t   direction();

        // Returns True / False if pressed
        bool isPressed();
};
