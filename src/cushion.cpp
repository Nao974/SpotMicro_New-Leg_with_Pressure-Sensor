#include <Arduino.h>
#include <spotMicro.h>
#include <LegPressureSensor.h> 
#include <SpotEprom.h>

#include <Servo.h>
#include <Oscillator.h>
#include <choreograph.h>

/* You can see the result here :

************
Please, go see my Choreogrpah Software repository 
for details on movement management
https://github.com/Nao974/choreograph-git
*/

#define CALIBRATE false

LegPressureSensor legs[4];

const uint16_t TIME = 150; // Home to Down movement in 1000 ms
const uint16_t FRAME = 10; // Time divider to limit steps every x ms

// Variable for movements
float decrement[NBRE_SERVO];
uint16_t iteration;
int8_t direction;
uint32_t next_time;

void setup() {
    // Init each pressure sensor 
    legs[LEG_FL].init(A8, 13);
    legs[LEG_BL].init(A9, 12);
    legs[LEG_FR].init(A10, 11);
    legs[LEG_BR].init(A11, 10);
    #if CALIBRATE == true 
        for ( uint8_t leg = 0; leg < 4; leg++)
            legs[leg].calibrateLow();

        for ( uint8_t leg = 0; leg < 4; leg++) {
            legs[leg].calibrateHigh();
            legs[leg].calculateNormalizeFactor();
        }    
        epromSave(legs);
    #else
        // Load calibrate and normalized coeff for each pressure sensor 
        epromLoad(legs);
    #endif
   
    for (uint8_t servo=0; servo<NBRE_SERVO; servo++) {
        // initialization of each servo and movement to the initial position
        chore.servoInit(pin[servo]);
        chore.servoOffset(servo, offset[servo]);
        moveSingle_safe(servo, BASE_HOME[servo]); 
        pos_actuelle[servo] = BASE_HOME[servo];

        // Calculates the difference between the 2 positions for each servo 
        decrement[servo] = double(TETE_BASSE[servo] - BASE_HOME[servo]) / (TIME / FRAME);
    }

    iteration = 0;  
    direction = 0;  
    next_time = millis();
}

void loop() {
    // Pressure reading and status update
    legs[LEG_FL].readNz();
    legs[LEG_FR].readNz();

    // If increased pressure on one of the front legs, then shock absorption
    if ( legs[LEG_FL].pressureValueNz()>30 or legs[LEG_FR].pressureValueNz()>30 ) {
        moveServos(350, TETE_BASSE);
        delay(150);
        moveServos(1000, BASE_HOME);
        delay(250);
        legs[LEG_FL].stateReset();
        legs[LEG_FR].stateReset();
    }
    // The next step will be to adapt the movement according to the pressure 
}