# Smart Garden Irrigation - Circuit Connections & Wiring Topology

**Organization:** Kernelwise Labs  

---

## 1. Schematic Circuit Interconnects

```
[12V 2A SMPS]
   |
   +-----> [LM2596 Buck Converter (12V -> 5V)] -----> [ESP32 VIN] & [Relay VCC]
   |
   +-----> [Relay COM Terminal]
                |
           [Relay NO Terminal] -----> [12V DC Water Pump (+)]
                                           |
                                    [12V DC Water Pump (-)] -----> [GND Star Point]
                                           ^
                                           | (Reverse-biased 1N4007 across pump +/-)
                                           +

[Capacitive Moisture Sensor]
   VCC  -----> ESP32 3V3
   GND  -----> ESP32 GND
   AOUT -----> ESP32 GPIO 34 (ADC1_CH6)

[DHT22 Sensor]
   VCC  -----> ESP32 3V3
   GND  -----> ESP32 GND
   DATA -----> ESP32 GPIO 4 (4.7kΩ Pull-Up to 3V3)

[5V Opto Relay]
   VCC  -----> 5V Rail
   GND  -----> Common GND
   IN   -----> ESP32 GPIO 23 (Active-LOW trigger)
```

---

## 2. Electrical Noise Suppression & Inductive Protection

1. **Flyback Snubber Diode:** When the 12V DC water pump motor turns off, its collapsing electromagnetic field generates back-EMF spikes up to $-100\text{V}$. A 1N4007 diode connected in reverse bias across the motor terminals absorbs this spike, preventing relay contact arcing and logic resets.
2. **Capacitive Filtering:** A 470µF electrolytic capacitor is placed across the 12V supply bus, and a 10µF tantalum capacitor is placed across the 5V logic bus to absorb transient surges.
