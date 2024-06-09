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

const uint16_t TIME = 150; // Home to Down in 1000 ms
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
            Serial.print("Leg "); Serial.print(leg); Serial.print(": ");
            Serial.print("Min "); Serial.print(legs[leg].calibrateLowValue());Serial.print(" => ");
            Serial.print("Max "); Serial.print(legs[leg].calibrateHighValue());Serial.print(" => ");
            Serial.print("Fact Nz "); Serial.println(legs[leg].normalizeFactorValue());
        }    
        Serial.println("Backup Calibration");
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

    if (millis() >= next_time){
        if ( legs[LEG_FL].isPressed() ) direction = 1;
        else direction = -1;

        // Limitation to not exceed the min / max positions
        if ( ( (iteration + direction) < (TIME / FRAME) ) && ( (iteration + direction) >= 0) ) iteration += direction;

        // Displacement of each servo of LEG_FL according to calculated iteration
        for (uint8_t i=0; i< 3; i++)
            moveSingle_safe(i, BASE_HOME[i] + ( decrement[i] * iteration) );
        next_time = millis() + FRAME; // every FRAME ms
    }
    
   delay(50);
}