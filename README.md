# 6Sens: inSitu Seismic System for Slope Stability Sensing

## Desciption
The goal is to develop a warning system on the analysis of the evolution of the micro-seismic activity generated before the instability is triggered. The method uses a multitude of low-cost seismic sensors deployed in-situ in a network over the possibly unstable zone. The goal is to characterize the temporal evolution of seismic signals detected simultaneously by several sensors (i .e., co-detection). This method uses to its advantage the heterogeneity of the materials and the attenuation of the waves during their propagation in the material.

The design of these sensors is subject to certain technical constraints essential to good
operation of the project:

    - Be autonomous in energy, over a period of several months (so as not to have to return to site, often dangerous).
    - Be robust, reliable and adapted to extreme climatic conditions (cold, hot, humidity).
    - All sensors in the network must be synchronized precisely enough to perform the analysis (about 1/10 second).
    - Be able to communicate information in real time – or near – measurements to a gateway that will retransmit the data to a dedicated website.
    - Be produced at low cost (to install a large-scale network, and have the possibility of to install sensors in hazardous areas with the possibility of losing them).

## Hardware description
### Geophone Sensor
The choice fell on omni geometric sensors from Geospace Technologies such Omni-X-LT or GS-ONE. Seismic geophone is a type of sensor that converts ground movement or a kind vibration into voltage, which could be acquired by an acquisition system. The deviation of this mesured voltage from the base line is called the geophone response and is analysed and for monitoring vibration generated by nature in our case.

The frequencyresponse of the geophone is a kind of harmonic oscillator, which is deternined by corner frequency, around 15Hz for the chosen sensors and damping (typically 0.7 for our case).
Our application require the full wave to be sued and the three-component 3C geophones is used (Omni-X-LT or GS-ONE).
For more information, please to refer to : https://www.geospace.com/products/sensors/

### Amplifier/Filter board


### CPU/Radio board for test
The choice fell on Cicerone MKR main board. Move-X Cicerone LoRa/GNSS Board is a high-performance, low-power, Arduino MKR-compatible development board based on the MAMWLE LoRa Module and the u-blox MAX-M10S GNSS Module. The Cicerone Board delivers best-in-class GNSS, long-range wireless connection, and high-performance MCU processing in a low-power solution for optimal battery life. The Cicerone Board allows users to build tracking applications worldwide with meter-level accuracy and to communicate long-range, low-power data via LoRaWAN. The integrated Li-Po charging circuit enables the Cicerone Board to manage battery charging through the USB port.

### CPU/Radio board for POC
