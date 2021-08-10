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

#include "LegPressureSensor.h"

// Constructor
LegPressureSensor::LegPressureSensor(const uint8_t pin) {
    _pin = pin;
    _pressure_value = _old_pressure_value = _pressure_max = _direction = _state = 0;
}

// Calculation of the calibration either released or pressed
//    with a measure number defined by the constant CALIBRATE_NB
void LegPressureSensor::calibrate() {
    uint16_t value = 0;

    for ( uint16_t i = 0; i < CALIBRATE_NB; i++) {
        delay(10);
        value += analogRead(_pin);
    }    
    _calibrate = value / CALIBRATE_NB;
}

// Update of the leg status according to the measured pressure
void LegPressureSensor::updateState() {
    // State determination
    if (_pressure_value >= _calibrate + TRIGGER_PRESSURE )
        _state = STATE_PRESSED;
    else if (_pressure_value <=  _calibrate - TRIGGER_PRESSURE ) {
        _state = STATE_RELEASED;
        _pressure_max = 0;
    } 

    // Direction determination
    if (_pressure_value > _old_pressure_value + TRIGGER_DIRECTION)
            _direction = DIR_PRESSURE;
    else if (_pressure_value < _old_pressure_value - TRIGGER_DIRECTION) 
            _direction = DIR_RELEASE;
    else _direction = DIR_FLAT;
    
}

// Pressure reading and state update
void LegPressureSensor::read() {
    _old_pressure_value = _pressure_value;
    _pressure_value = analogRead(_pin);
    if (_pressure_value > _pressure_max) _pressure_max = _pressure_value;

    updateState();
}

// Return the last measured pressure
uint16_t LegPressureSensor::pressureValue() {
    return _pressure_value;
}

// Return the direction of movement
int8_t LegPressureSensor::direction() {
    return _direction;
}

// Return the calculated released pressure
uint16_t LegPressureSensor::calibrateValue() {
    return _calibrate;
}

// Return the current max pressure
uint16_t LegPressureSensor::pressureMax() {
    return _pressure_max;
}

// Returns True / False if pressed
bool LegPressureSensor::isPressed() {
    return _state == STATE_PRESSED;
}