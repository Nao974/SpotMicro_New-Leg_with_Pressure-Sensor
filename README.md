# SpotMicro: New leg with pressure sensor

[Version française](./README_fr.md)

<a id="up"> </a>

The pressure sensor at the end of the leg will allow you to know:
- If it hits the ground (Boolean True / False)
- The force exerted on it
- And at an instant "t" the direction of the force exerted (pressure // release)

[![Video Presentation](assets/video_presentation.png)](https://youtu.be/9EAe0ReYfHI)

---

## Creation of "pear"

<a id="pear"> </a>

Just like the leg of Unitree's A1 robot, I set out to create a flexible "Pear".

First by 3d printing in filament ** FLEX ** (TPU), but the exercise is not easy because it is flexible and it is difficult to have a constant flow at the output extruder.
After tests of different thicknesses, it remains too rigid and it tears very quickly:

[![Test TPU](assets/flex-pear.png)](https://youtu.be/qxboo4HXB_k)

I have not tested the TPE filament which is even more flexible because the experience with the FLEX was already complicated.

---

## New Option : **Silicone**

<a id="silicone"> </a>

The principle is to 3D print molds in order to "model" Silicone of bathroom joints:

<div align="center"><img alt="silicone-mold-A" width="45%" src="assets/silicone-mold_A.png" />&nbsp;<img alt="silicone-mold-B" width="45%" src="assets/silicone-mold_B.png" /></div>  

The Silicone is first kneaded in water saturated with dishwashing liquid:

** VIDEOOOOOOOOOOOOO **

A few hours of drying and our part is ready, several thickness tests have been carried out:

[![Result Silicone Mold](assets/result_silicone-mold.png)](https://youtu.be/qxboo4HXB_k)

---

## Pressure sensor

<a id="sensor"> </a>

Having no idea of the pressure differential in the "pear" between a pressed or released state, I tested 2 sensors with different accuracy.

I chose analog sensors because all of the I2Cs found had a fixed address, therefore incompatible with the SportMicro which has 4 legs, therefore 4 pressure sensors.

### **MPX4250AP**
With an accuracy of 20 mV / kPa, this sensor is not sensitive enough, even when I blow into it to the maximum, the value varies only very slightly.

### **MPX5010DP**
With an accuracy of 450mv / kPa, this sensor is great, I capture light pressures as well as large crushes:

[![Result Sensor](assets/result_sensor.png)](https://youtu.be/x-UZMXbWamg)

---

## Modification of the leg

*Work in progress*