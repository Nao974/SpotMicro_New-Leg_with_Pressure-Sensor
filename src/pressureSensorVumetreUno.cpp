#include <Arduino.h>
#include <LegPressureSensor.h> 

/*  You can see the result here :
 https://youtube.com/shorts/5eInNg3Okts?si=phbLiTOYaWVGga2u
*/

// Pin of the differents leds On Arduino Uno
const uint8_t pin_vumetre[]={2,3,4,5,6,7,8,9};
const uint8_t pin_pressed = 10;
const uint8_t pin_released = 11;
const uint8_t pin_leg = 13;

// Variable for the Vumeter display
int16_t vumetre = 0;
uint8_t led = 0;

//Declaration of a 1st leg
LegPressureSensor leg1;

void setup() {
    // Initialize the Leg with the corresponding pins
    leg1.init(A0, pin_leg);
    // Release calibration of the leg
    delay(500);
    leg1.calibrateLow();
    leg1.calibrateHigh();
    leg1.calculateNormalizeFactor(); 

    // Initializes pins for Leds
    for (uint8_t i =0; i<8; i++) {
        pinMode(pin_vumetre[i], OUTPUT);
        digitalWrite(pin_vumetre[i], HIGH);
    }    
    pinMode(pin_pressed, OUTPUT);
        digitalWrite(pin_pressed, HIGH);
    pinMode(pin_released, OUTPUT);
        digitalWrite(pin_released, HIGH);
}

void loop() {
    // Pressure reading and status update
    leg1.readNz();

    // LED management according to status
    digitalWrite(pin_released, !leg1.isPressed());
    digitalWrite(pin_pressed, leg1.isPressed());
    
    //VU-meter update
    if ( vumetre > (led*32) && leg1.isPressed() ) digitalWrite(pin_vumetre[led], HIGH);
    else digitalWrite(pin_vumetre[led], LOW);
    // Once the last LED has been processed, a new value is displayed
    if ( led >= 7 ) {
        led=0;
        vumetre = leg1.pressureValueNz();
    }
    // Otherwise processing of the next led
    else led++;  
}

