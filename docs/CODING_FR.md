# Programmation
Les exemples donnés utilisent les librairies du projet [Choreograph](https://github.com/Nao974/choreograph-git) pour la gestion des mouvements et des servomoteurs. Je vous laisse prendre connaissance de ce projet connexe.

Librairies pour la gestion des servos non documentées dans ce projet :
- lib/choreograph
- lib/Oscillator
- lib/Servo

## Librairie LegPressureSensor

Ecrite pour VSCode il suffit de la copier dans le dossier *./lib* de votre projet Plateform-IO.

### Les Constantes

```C
const uint16_t CALIBRATE_NB = 100;
const float CALIBRATE_STANDARD_DEVIATION = 350.0;
const uint8_t TRIGGER_PRESSURE = 10;
const uint8_t TRIGGER_DIRECTION = 5;
```

- CALIBRATE_NB : Nombre de mesure lors du calcul de la valeur de calibration
- CALIBRATE_STANDARD_DEVIATION: Ecart type utilisé dans le calcul du coefficient de normalisation
- TRIGGER_PRESSURE : Ecart par rapport à la calibration validant un changement d'état.
- TRIGGER_PRESSURE : Ecart par rapport à la calibration validant un changement de sens.

### Les Méthodes Publics (fonctions)

```C
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
```

### Déclaration et Initialisation
Suivant l'exemple du programme *1xleg_FrontLeft.cpp*

- Déclaration d'un tableau de type LegPressureSensor pour les 4 pattes  
```C
LegPressureSensor legs[4];
```

- Initialisation des 4 pattes avec la broche d'entrée analogique pour la lecture du capteur et la sortie digitale pour la LED :    
```C
legs[LEG_FL].init(A8, 13);
legs[LEG_BL].init(A9, 12);
legs[LEG_FR].init(A10, 11);
legs[LEG_BR].init(A11, 10);
```

### Calibration
Le robot doit être placé sur un support avec les pattes qui ne touchent pas le sol :  
<img alt="Robot sur support" src="assets/coding_calibration.png"/>

**La calibration doit se faire *à vide* puis par compression du coussinet**

- Calibration *à vide* pour les 4 pattes :  
```C
for ( uint8_t leg = 0; leg < 4; leg++)
    legs[leg].calibrateLow();
```

- Puis calibration avec pression max :  
```C
for ( uint8_t leg = 0; leg < 4; leg++) {
    legs[leg].calibrateHigh();
    legs[leg].calculateNormalizeFactor();
}
```
Pour chaque patte l'une après l'autre, la LED va clignotée afin d'indiquer la patte en cours de calibration, il convient à ce moment de presser plusieurs fois le coussinet afin d'avoir la valeur max de pression. Une fois la mesure effectuée, le programme passe à la patte suivante en faisant clignoter sa LED.

La calibration n'est pas à faire à chaque démarrage, vous pouvez sauvegarder le résultat en mémoire non volatile avec la fonction :  
```C
epromSave(legs);
```

Puis lors des démarrages suivant, recharger la calibration précédente :  
```C
epromLoad(legs);
```C

La constante permet de lancer ou non la calibration :  
```C
#define CALIBRATE false
```

## Code Source Exemple

- **pressureSensorVumetreUno.cpp** : Permet de tester la patte réalisée avec un retour visuel sur un vumètre à 8 LEDs. Montage réalisé sur une carte Arduino Uno, il convient de modifier l'environnement  dans *platformio.ini*  
[Vidéo Youtube](https://youtu.be/5eInNg3Okts)

- **1xLeg_FrontLeft.cpp** : Lors de l'appui sur le coussinet, la patte se rétracte, elle reprend sa position initiale une fois le coussinet relaché  
[Vidéo Youtube](https://youtu.be/5eInNg3Okts)

- **cushion.cpp** : Amortissement des pattes avant quand il tombe ou lors d'un coup sur la tête provoquant une pression sur les capteurs avants :   
[Vidéo Youtube t=9s](https://youtu.be/5eInNg3Okts)

---

[retour au menu](../README_FR.md)