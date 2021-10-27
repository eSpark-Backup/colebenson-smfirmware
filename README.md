# ColeBenson-SMFirmware
 To develop and test Smart Motors Firmware on Development Kits, Breakout Board and then on custom hardware

## STM32DuinoCore-Github
 https://github.com/stm32duino/Arduino_Core_STM32
 
## Project Dependencies

### STM32F4 Software Development Kit
1.  https://www.st.com/en/embedded-software/stm32cubef4.html   (STM32Cube MCU Package with Patch)
2.  https://www.st.com/en/embedded-software/stsw-stm32068.html (STM32F4DISCOVERY Firmware Package)

### STLINK-V2 / STLINK-V3 Drivers
1. https://www.st.com/en/development-tools/stsw-link009.html (USB Drivers for Windows)
2. https://www.st.com/en/development-tools/stsw-link007.html (Firmware Update for Debugger)

### Arduino IDE Development Environment for Custom Board

![image](https://user-images.githubusercontent.com/76909080/139075820-1104380a-4e52-4e43-a465-e5b54c6c8d06.png)

### STM32 Boot Mode Selection
1. Two Boot Modes of STM32 will be focused upon: System Memory and User Flash Memory modes.
2. To select between these two modes, keep BOOT1 connected to (GND <0>), and switch between (VCC <1>) / (GND <0>) for BOOT0.
3. 
![image](https://user-images.githubusercontent.com/76909080/139076461-e29edceb-ec9c-40e3-9cbc-7d05221edb2c.png)

