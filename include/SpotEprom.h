#include <Arduino.h>
#include <LegPressureSensor.h> 
#include <EEPROM.H>

struct eprom_t {
    uint16_t FLcalibrateLow, FLcalibrateHigh;
    float FLnormalizeFactor;

    uint16_t BLcalibrateLow, BLcalibrateHigh;
    float BLnormalizeFactor;

    uint16_t FRcalibrateLow, FRcalibrateHigh;
    float FRnormalizeFactor;

    uint16_t BRcalibrateLow, BRcalibrateHigh;
    float BRnormalizeFactor;
};

void epromSave(LegPressureSensor *legs ) {
    eprom_t eprom;

    eprom = { 
        legs[LEG_FL].calibrateLowValue(), legs[LEG_FL].calibrateHighValue(), legs[LEG_FL].normalizeFactorValue(),
        legs[LEG_BL].calibrateLowValue(), legs[LEG_BL].calibrateHighValue(), legs[LEG_BL].normalizeFactorValue(),
        legs[LEG_FR].calibrateLowValue(), legs[LEG_FR].calibrateHighValue(), legs[LEG_FR].normalizeFactorValue(),
        legs[LEG_BR].calibrateLowValue(), legs[LEG_BR].calibrateHighValue(), legs[LEG_BR].normalizeFactorValue(),
    };

    EEPROM.put(0, eprom);
}

void epromLoad(LegPressureSensor *legs ) {
    eprom_t eprom;

    EEPROM.get(0, eprom);
    
    legs[LEG_FL].calibrateLowWrite(eprom.FLcalibrateLow); legs[LEG_FL].calibrateHighWrite(eprom.FLcalibrateHigh);
        legs[LEG_FL].normalizeFactorWrite(eprom.FLnormalizeFactor); legs[LEG_FL].readNz();
    legs[LEG_BL].calibrateLowWrite(eprom.BLcalibrateLow); legs[LEG_BL].calibrateHighWrite(eprom.BLcalibrateHigh);
        legs[LEG_BL].normalizeFactorWrite(eprom.BLnormalizeFactor); legs[LEG_BL].readNz();
    legs[LEG_FR].calibrateLowWrite(eprom.FRcalibrateLow); legs[LEG_FR].calibrateHighWrite(eprom.FRcalibrateHigh);
        legs[LEG_FR].normalizeFactorWrite(eprom.FRnormalizeFactor); legs[LEG_FR].readNz();
    legs[LEG_BR].calibrateLowWrite(eprom.BRcalibrateLow); legs[LEG_BR].calibrateHighWrite(eprom.BRcalibrateHigh);
        legs[LEG_BR].normalizeFactorWrite(eprom.BRnormalizeFactor); legs[LEG_BR].readNz();

}