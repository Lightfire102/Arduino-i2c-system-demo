# Arduino-i2c-system-demo

## Overview

https://github.com/user-attachments/assets/6f830aec-d6af-4344-8bd7-ca83c07705bc

This project shows a **multi-peripheral I2C system** using an Arduino Uno R3 as the master and it communicates with the following:

- **DS1307 Real-time Clock (RTC)** for timestmping
- **MPU-9250 / MPU-6050 IMU** for raw accelerometer, gyroscope, and temperature data

The main purpose of this project was to strengthen both my familiarity and understanding of the following:
- I2C addressing and bus sharing
- Register pointer writes + repeated-start reads
- Raw sensor data reading and converting
- Hardware protocol verifiction

---

## System Architecture


### High-level Flow
![System Flowchart](docs/overview/Flowchart.md)

### Physical Build
![Breadboard Build](docs/overview/I2CArduinoSystemBuild.md)

---

## Hardware


### Components
- Arduino Uno R3
- DS1307 RTC module
- MPU-9250 or MPU-6050 IMU breakout
- Breadboard and jumper wires
- Pull-up resistors on SDA/SCL (10 k)
- Decoupling capacitors (0.1 µF)

### Power Notes
- **MPU powered at 3.3 V** (just to be safe)
- RTC powered at 5 V
- Common ground

### I2C Addressing
DS1307 RTC | '0x68 | Fixed address |
MPU-9250 | '0x69 | AD0 set to High |

---

## Things to take away

- It's extremely helpful to have an I2C scanner to figure out which devices are actually running I2C
- Drawing system diagrams is a helpful way to visualize bigger systems
- IMPORTANT FOR I2C project firmware. Even if you use setClock(), word of advice is to always check using the oscilloscope to make sure the clock is running at the time you want. A mentor told me that firmware coders written the speed they want the clock ran and it just doesn't run at that speed so a good practice is to test it. 
- Writing code that is "too clever" can actually backfire because then when you need to debug and you're looking at clever code it will be hard to understand. It's better to write clean, readable and understandable code so that when it comes to debugging, you need to make sure you draw a fine line there. 

---

## Author

**Justus Downing**
Computer Engineering
Embedded Systems & Hardware Debugging

