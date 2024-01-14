
## Latest update


``
Assembling the board, and working on the firmware...
``


# Blender

The Blender project's purpose is to extract digitalized sound characteristics like amplitude and frequency, which are transferring through the S/PDIF protocol, and display the amplitude of different frequencies by the dominant color of the input VGA received signal. So the base color will be determined by VGA input, and a spectrum of that color will be displayed over/on the DMX/WS2812 chip. Also, the specification of the sound will be displayed on an OLED, and the sound will be played by a chip that has an integrated DAC in it. The data to DAC will be transferred via the I2S protocol.

## Features

- Battery Powered
- Can be installed in-line.
- On board speaker and WS2812 for troubleshooting.

## Tech Stack

### Hardware

- **MCU:** STM32F446RCT6
- **Connection:** S/PDIF, VGA, DMX, I2S
- **PCB Layers:** 4Layer/s (None-controlled Impedance)
- **CAD:** KiCAD

### Firmware

- **OS:** FreeRTOS
- **Programming Language:** C
- **IDE:** STM32CubeIDE

### Application

- **Application:** N/A



## Block Diagrams

![SystemDesign](https://github.com/mend0z0/Blender/blob/main/Document/Block%20Diagrams/_FBD_SYS_Blender_v1.0.svg)

![Hardware](https://github.com/mend0z0/Blender/blob/main/Document/Block%20Diagrams/_FBD_HW_Blender_v1.0.svg)

![Firmware](https://github.com/mend0z0/Blender/blob/main/Document/Block%20Diagrams/_FBD_FW_Blender_v1.0.svg)


## Screenshots

### Hardware

#### CAD View

![_CADView](https://github.com/mend0z0/Blender/blob/main/Document/Media%20Content/Hardware%20Pictures/CAD%20View/_CADView_Blender_v1.0.svg)

#### 3D_View

![_3DView_Top](https://github.com/mend0z0/Blender/blob/main/Document/Media%20Content/Hardware%20Pictures/3D%20View/_3DView_Top_Blender_v1.0.png)

![_3DView_Bottom](https://github.com/mend0z0/Blender/blob/main/Document/Media%20Content/Hardware%20Pictures/3D%20View/_3DView_Bottom_Blender_v1.0.png)
 
#### Assembled Hardware

![_ASMView_Top](https://github.com/mend0z0)

![_ASMView_Bottom](https://github.com/mend0z0)

### Application
![App Screenshot](https://github.com/mend0z0)


## Demo

Insert gif or link to demo, it'll be my youtube channel

## LOGO

![Logo](https://github.com/mend0z0/Blender/blob/main/LOGO.png)


## Author

- [@Siavash Taher Parvar](https://www.linkedin.com/in/mend0z0)


## Support

[Support Link](https://github.com/sponsors/mend0z0)

