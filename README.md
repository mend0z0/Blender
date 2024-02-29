
## Latest update


``
Assembling the board, and working on the firmware...
``


# Blender

The Blender project's purpose is to extract digitalized sound characteristics like amplitude and frequency, which are transferring through the S/PDIF protocol, and display the amplitude of different frequencies by the dominant color of the input VGA received signal. So the base color will be determined by VGA input, and a spectrum of that color will be displayed over/on the DMX/WS2812 chip. Also, the specification of the sound will be displayed on an OLED, and the sound will be played by a chip that has an integrated DAC in it. The data to DAC will be transferred via the I2S protocol.

## Category

__Digital Signal Processing (DSP)__

## Features

- Battery Powered
- Can be installed in-line.
- On board speaker and WS2812 for troubleshooting.

## Tech Stack

| <H3>Hardware</H3> | <H3>Firmware</H3> | <H3>Mechanical</H3> |
|:--| --| --:| 
| <br><P align="left">**MCU:** STM32F446RCT6</br><br>**Connection:** S/PDIF, VGA, DMX, I2S</br><br>**PCB Layers:** 4Layer/s (None-controlled Impedance)</br><br>**CAD:** KiCAD</br> | <br><P align="left">**OS:** FreeRTOS</br><br>**Programming Language:** C</br><br>**IDE:** STM32CubeIDE</br><br></br> | <br><P align="left">**CAD:** Solid Edge</br><br>**Manufacturing Method:** 3D Printing</br><br></br><br></br> |

## Block Diagram

![SystemDesign](https://github.com/mend0z0/Blender/blob/main/Document/Block%20Diagrams/_FBD_SYS_Blender_v1.0.svg)


![Firmware](https://github.com/mend0z0/Blender/blob/main/Document/Block%20Diagrams/_FBD_FW_Blender_v1.0.svg)


## Screenshots

### Assembled Hardware

![_ASMView_Top](https://github.com/mend0z0/Blender/blob/main/Document/Tests/Assembled%20Hardware/Assembled%20PCB/_ASMView_Top_Blender_v1.0.jpeg)

![_ASMView_Bottom](https://github.com/mend0z0/Blender/blob/main/Document/Tests/Assembled%20Hardware/Assembled%20PCB/_ASMView_Bottom_Blender_v1.0.jpeg)

### Final Device

![_BLENDER](https://github.com/mend0z0)


## Demo

Insert gif or link to demo, it'll be my youtube channel

## LOGO

![Logo](https://github.com/mend0z0/Blender/blob/main/LOGO.png)


## Author

- [@Siavash Taher Parvar](https://www.linkedin.com/in/mend0z0)


## Support

[Support Link](https://github.com/sponsors/mend0z0)

