#include <Arduino.h>
#include <LegPressureSensor.h> 

// Pin of the differents leds
const uint8_t pin_vumetre[]={2,3,4,5,6,7,8,9};
const uint8_t pin_pressed = 10;
const uint8_t pin_released = 11;

// Variable for the VUmeter display
uint8_t vumetre = 0;
uint8_t i = 0;

//Declaration of a 1st leg
LegPressureSensor leg1(A0);

// Displays the vu-meter LED by LED at each main loop for better visibility 
//    and no blocking function
void vumeterOneByOne() {
    // Determines whether to turn on or extend the LED according to the row of the LED
    if ( vumetre > (i*16) && leg1.isPressed() ) digitalWrite(pin_vumetre[i], HIGH);
    else digitalWrite(pin_vumetre[i], LOW);

    // Once the last LED has been processed, a new value is displayed
    if ( i >= 7 ) {
        i=0;
        vumetre = leg1.pressureMax() - leg1.calibrateValue();
    }
    // Otherwise processing of the next led
    else i++;   
}


void setup() {

    // Release calibration of the leg
    delay(500);
    leg1.calibrate();

    // Initializes pins in digital output 
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
    leg1.read();

    // LED management according to status
    digitalWrite(pin_released, !leg1.isPressed());
    digitalWrite(pin_pressed, leg1.isPressed());
    
    //VU-meter management
    vumeterOneByOne();
}

