#include <Arduino.h>

#include <Servo.h>
#include <Oscillator.h>
#include <choreograph.h>

const uint8_t LEG_FL=0, LEG_BL=1, LEG_FR=2, LEG_BR=3;

const uint8_t  NBRE_SERVO = 12;
const uint8_t  MAX_POS = 180; // Servo Max 180°


uint8_t servo_id;
uint8_t pos_actuelle[NBRE_SERVO];
choreograph chore;

uint8_t pin[]={48, 50, 52, 40, 42, 44, 32, 34, 36, 24, 26, 28};
int8_t offset[]={-3, 0, -2, 0, 0, 0, -4, 2, -8, -3, 2, -12};
uint8_t orientation[]={'d', 'd', 'd', 'i', 'i', 'i', 'd', 'd', 'd', 'i', 'i', 'i'};
uint8_t angle_min[]={40, 0, 65, 40, 0, 65, 40, 0, 65, 40, 0, 65};
uint8_t angle_max[]={135, 180, 175, 135, 180, 175, 135, 180, 175, 135, 180, 175};
uint8_t BASE_HOME[]={90,60,120,90,60,120,90,50,120,90,50,120};

/*** Positions */
uint8_t BASE_DEPART[]={90,45,175,90,45,175,90,45,175,90,45,175};
uint8_t LEVER_LEVER_1[]={90,45,175,90,45,175,90,45,150,90,45,150};
uint8_t TETE_BASSE[]={90,35,165,90,35,165,90,50,120,90,50,120};



// Servo movement according to its orientation and min / max limit
void moveSingle_safe(uint8_t servo_id, uint8_t pos) {
    if (pos <= angle_max[servo_id] && pos >= angle_min[servo_id]) {
        if (orientation[servo_id] == 'i') 
            pos= MAX_POS -pos;
        chore.moveSingle(servo_id, pos);
    }    
}

// Move to pos in time ms
void moveServos(int16_t time, uint8_t pos[]) 
  {
   unsigned long partial_time, final_time;
   float increment[NBRE_SERVO];

   for (uint8_t i=0; i< NBRE_SERVO; i++) 
     increment[i] = (pos[i] - pos_actuelle[i]) / (time / 10.0);

   final_time =  millis() + time;
   for (int iteration = 1; millis() < final_time; iteration++) 
     {
      partial_time = millis() + 10;
      for (int i=0; i< NBRE_SERVO; i++) 
         moveSingle_safe(i,pos_actuelle[i] + (iteration * increment[i]));
      while (millis() < partial_time);
     }
   for (int i=0; i< NBRE_SERVO; i++) pos_actuelle[i]=pos[i];
  }