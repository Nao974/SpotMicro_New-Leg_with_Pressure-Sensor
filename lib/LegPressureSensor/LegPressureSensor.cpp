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

// Init -- Constructor
void LegPressureSensor::init(const uint8_t pinPC, const uint8_t pinLed) {
    _pinPC = pinPC;
    _pinLed = pinLed;
    _pressure_value = _pressure_valueNz = _old_pressure_value = _pressure_max = _direction = _state = _calibrateHigh = _calibrateLow = 0;
    pinMode(_pinLed, OUTPUT);
    digitalWrite(_pinLed, HIGH);

}

/*** 
 * Calibration LOW
 * ****************/

// Calculation of the calibration either released or pressed
//    with a measure number defined by the constant CALIBRATE_NB
void LegPressureSensor::calibrateLow() {
    uint16_t value = 0;

    for ( uint16_t i = 0; i < CALIBRATE_NB; i++) {
        delay(10);
        value += analogRead(_pinPC);
    }    
    _calibrateLow = value / CALIBRATE_NB;
    digitalWrite(_pinLed, LOW);
}

// Return the calculated released pressure
uint16_t LegPressureSensor::calibrateLowValue() {
    return _calibrateLow;
}

// Write the calculated released pressure
void LegPressureSensor::calibrateLowWrite(uint16_t value) {
    _calibrateLow = value;
}

/*** 
 * Calibration HIGH
 * ****************/

// Reading of the max pressure by the user to calculate the ratio
void LegPressureSensor::calibrateHigh() {
    int16_t value = 0;

    digitalWrite(_pinLed, HIGH);
    delay(500);
    digitalWrite(_pinLed, LOW);
    delay(500);
    digitalWrite(_pinLed, HIGH);
    delay(500);

    while (analogRead(_pinPC) < 100);

    for ( uint16_t i = 0; i < CALIBRATE_NB; i++) {
        delay(10);
        value = analogRead(_pinPC);
        if ( value > _calibrateHigh )
            _calibrateHigh = value;
    }    

    digitalWrite(_pinLed, LOW);
    delay(500);
    digitalWrite(_pinLed, HIGH);
    delay(500);
    digitalWrite(_pinLed, LOW);
    delay(500);
}

// Return the Calibrate pressure max
uint16_t LegPressureSensor::calibrateHighValue() {
    return _calibrateHigh;
}

// Write the Calibrate pressure max
void LegPressureSensor::calibrateHighWrite(uint16_t value) {
    _calibrateHigh = value;
}

/*** 
 * Calibration Normalized Factor
 * *****************************/

// Calculation of the normlization factor
void LegPressureSensor::calculateNormalizeFactor() {
    _normalizeFactor = (_calibrateHigh - _calibrateLow) / CALIBRATE_STANDARD_DEVIATION;
}

// Return the Calculate normalization factor
float LegPressureSensor::normalizeFactorValue() {
    return _normalizeFactor;
}

// Write the Calibrate normalization factor
void LegPressureSensor::normalizeFactorWrite(float value) {
    _normalizeFactor = value;
}

/*** 
 * Reading pressure and Update State and Direction
 * *****************************/

// Pressure reading and state update
void LegPressureSensor::read() {
    _old_pressure_value = _pressure_value;
    _pressure_value = analogRead(_pinPC);
    if (_pressure_value > _pressure_max) _pressure_max = _pressure_value;

    updateState();
}

// Pressure reading, normalization, and state update
void LegPressureSensor::readNz() {
    _old_pressure_value = _pressure_value;
    _pressure_value = analogRead(_pinPC);
    if (_pressure_value > _pressure_max) _pressure_max = _pressure_value;

    _pressure_valueNz = (_pressure_value - _calibrateLow ) / _normalizeFactor;
    if (_pressure_valueNz > _pressure_maxNz) _pressure_maxNz = _pressure_valueNz;

    updateState();
}

// Update of the leg status according to the measured pressure
void LegPressureSensor::updateState() {
    // State determination
    if (_pressure_value >= _calibrateLow + TRIGGER_PRESSURE )
        _state = STATE_PRESSED;
    else if (_pressure_value <=  _calibrateLow - TRIGGER_PRESSURE ) {
        _state = STATE_RELEASED;
        _pressure_max = 0;
        _pressure_maxNz = 0;
    } 

    // Direction determination
    if (_pressure_value > _old_pressure_value + TRIGGER_DIRECTION)
            _direction = DIR_PRESSURE;
    else if (_pressure_value < _old_pressure_value - TRIGGER_DIRECTION) 
            _direction = DIR_RELEASE;
    else _direction = DIR_FLAT;

    //Update of the leds according to the status
    digitalWrite(_pinLed, _state);
}


/*** 
 * returns the different values
 * *****************************/

// Return the last measured pressure
int16_t LegPressureSensor::pressureValue() {
    return _pressure_value;
}

// Return the last measured pressure normalized
int16_t LegPressureSensor::pressureValueNz() {
    return _pressure_valueNz;
}

// Return the current max pressure
int16_t LegPressureSensor::pressureMax() {
    return _pressure_max;
}

// Return the current max pressure normalized
int16_t LegPressureSensor::pressureMaxNz() {
    return _pressure_maxNz;
}

// Return the direction of movement
int8_t LegPressureSensor::direction() {
    return _direction;
}

// Returns True / False if pressed
bool LegPressureSensor::isPressed() {
    return _state == STATE_PRESSED;
}


/*** 
 * Others
 * *****************************/

// Reset state at Release
void LegPressureSensor::stateReset() {
    _state = STATE_RELEASED;
    _pressure_max = 0;
    _pressure_maxNz = 0;
    _direction = DIR_FLAT;
}