# Wearable Light Controller

A responsive wearable prototype that allows body movement and gesture to control the colour of the Vespera lighting installation.

![Final wearable light controller](<img width="3472" height="4624" alt="Final product" src="https://github.com/user-attachments/assets/320a2c00-90a8-4665-853a-40229472dfff" />

)

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

![Initial sketch](<img width="3472" height="4624" alt="Sketch" src="https://github.com/user-attachments/assets/10220dc1-7313-4d89-8edb-b533472cdd64" />
)

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

![Sensor placement on the jacket](<img width="3472" height="4624" alt="Installing" src="https://github.com/user-attachments/assets/d3a6c89d-1403-4933-8050-e440be3dd0f6" />

)

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

![Red to purple gradient](<img width="1575" height="824" alt="gradient 4" src="https://github.com/user-attachments/assets/fd8dbdf7-f86a-4b2a-9f25-56215bdb582f" />
)

![Green to blue gradient](<img width="1523" height="763" alt="Gradient 1" src="https://github.com/user-attachments/assets/e343fd75-5cce-4666-984f-31da6bb36323" />
)

![Blue to pink gradient](<img width="1510" height="759" alt="gradient 2" src="https://github.com/user-attachments/assets/b3394026-5598-487d-b1f7-684773ba891f" />
)

![Yellow to red gradient](<img width="1554" height="830" alt="gradient 3" src="https://github.com/user-attachments/assets/c13c476b-7d6a-4032-8669-7243b839f38e" />
)

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

![Arduino and breadboard](<img width="4624" height="3472" alt="1 sensor" src="https://github.com/user-attachments/assets/71f8b16c-2ff7-4c7f-b4f3-48dd8ef3526b" />
)

![Three photoresistors installed](<img width="3472" height="4624" alt="3 sensors installed" src="https://github.com/user-attachments/assets/35193244-f341-4e7c-a5bc-7c6abd9a5169" />
)

![Battery and power switch](<img width="3472" height="4624" alt="Wireless battery installed" src="https://github.com/user-attachments/assets/a3f4449a-596e-41b7-8a0b-a8b1730f149f" />
)

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
