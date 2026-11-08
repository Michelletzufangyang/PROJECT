# Wearable Light Controller

A responsive wearable prototype that allows body movement and gesture to control the colour of the Vespera lighting installation.

![Final wearable light controller](images/final-product.jpg)

**Course:** CASA0014 Prototyping the Internet of Things  
**Institution:** UCL Bartlett, Connected Environments  
**Board:** Arduino MKR WiFi 1010  
**Vespera luminaire:** 31  
**Communication:** Wi-Fi and MQTT

---

## 1. Project Overview

This project explores how a garment can become an interactive lighting interface.

I developed a wearable light controller using three photoresistors attached to a black jacket. When a performer covers one of the sensors, the Arduino detects the change in light and sends a new colour gradient to the Vespera lighting installation.

The garment transforms a simple physical action—covering a sensor—into a change in the surrounding light. This creates a connection between the performer's body, movement and the atmosphere of the space.

Rather than controlling light through a conventional button or screen, the performer interacts with the system through the garment itself.

![Initial sketch](images/sketch.jpg)

---

## 2. Design Concept

The original idea was to use photoresistors on a stage. A performer could move around the stage and cover different sensors to create changes in the lighting.

I then developed the idea into a wearable controller. The three photoresistors were positioned on different parts of the jacket:

- One sensor on the left sleeve
- One sensor on the right sleeve
- One sensor on the chest

This arrangement allows the performer to use gestures to control the light. For example:

- Covering the left sleeve sensor changes the lighting
- Covering the right sleeve sensor changes it again
- Covering the chest sensor creates another colour transition

The garment therefore becomes a spatial and bodily interface.

![Sensor placement on the jacket](images/sensor-placement.jpg)

---

## 3. Interaction

Each photoresistor works as a light-sensitive trigger.

When the sensor is uncovered, it receives light and produces a higher analogue reading. When the performer covers it with their hand or another part of the garment, the reading falls below a threshold.

The Arduino recognises this change and advances to the next lighting state.

| Performer action | System response |
|---|---|
| Cover a photoresistor | The next colour gradient is activated |
| Uncover the photoresistor | The sensor is reset and ready for another interaction |
| Cover another photoresistor | The lighting changes again |
| Repeat the gesture | The colour sequence continues |

The prototype cycles through four gradients:

1. Red to purple
2. Green to blue
3. Blue to pink
4. Yellow to red

![Red to purple gradient](images/gradient-1.png)

![Green to blue gradient](images/gradient-2.png)

![Blue to pink gradient](images/gradient-3.png)

![Yellow to red gradient](images/gradient-4.png)

---

## 4. Components

### Hardware

- Arduino MKR WiFi 1010
- Three photoresistors / LDRs
- Three 10 kΩ resistors
- Breadboard
- Jumper wires
- 3.7 V rechargeable lithium battery
- Battery connector and power switch
- Black jacket
- Tape and foil tape for securing and routing wires
- Vespera 72-pixel luminaire
- Laptop for monitoring the MQTT lighting grid

![Arduino and breadboard](images/arduino-breadboard.jpg)

![Three photoresistors installed](images/three-sensors-installed.jpg)

![Battery and power switch](images/wireless-battery.jpg)

---

## 5. Circuit and Wiring

The three photoresistors are connected to separate analogue pins:

| Component | Arduino pin |
|---|---|
| Photoresistor 1 | A0 |
| Photoresistor 2 | A1 |
| Photoresistor 3 | A2 |
| Photoresistor power | VCC |
| Resistor connection | GND |

Each photoresistor forms a voltage divider with a 10 kΩ resistor.

```text
VCC
 │
Photoresistor
 │
Analogue pin A0, A1 or A2
 │
10 kΩ resistor
 │
GND# PROJECT
