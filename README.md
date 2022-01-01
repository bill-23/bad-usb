# Bad USB v1.1
---
![Bad USB Installed](/Bad-usb-Assem.jpg)

Ducky Script intrepter/executer for Arduino boards. 

Designed as a non-stealth version of a usb rubber ducky [Hak5](https://hak5.org/products/usb-rubber-ducky-deluxe) for times when stealth doens't matter and you want the ability to run any number of scripts.

The design incorporates an Arduino Pro Micro, Adafruit Micro-SD breakout, OLED Display and Mini Rotating Momentary Button. 

Use this for LEGAL - NON NEFARIOUS uses - like automatically setting passwords or other IT tasks.

***Released under MIT License***

---

## External Libraries

The script makes use of the following external libraries (All available from the Arduino library manager):
- Keyboard
- SPI
- SD
- Wire
- ACROBOTIC_SSD1306

**Note: We need to make a couple of changes to the ACROBOTIC_SSD1306 library. As of publishing time this project uses v1.0.1 which doens't seem to have native 128x32 support.** 

**ACROBOTIC_SSD1306.h**
Change the `SSD1306_Max_Y` variable to `SSD1306_Max_Y = 31`. 

**ACROBOTIC_SSD1306.cpp**
Add `sendCommand(SSD1306_Max_Y);` to line 32 in `init()`
Remove `sendCommand(0x3F);` from `init()`

Adjust the inner loop of `clearDisplay()` from `i=16` -> `i=26`. This will clear the while screen. If not changed it will leave pixels lit even when cleared.

---

## Hardware

- Arduino Pro Micro - [Amazon](https://www.amazon.com/ARCELI-Atmega32U4-Bootloadered-Development-Microcontroller/dp/B07J2Q3ZD5/ref=sr_1_12_sspa?dchild=1&keywords=Arduino+Pro+Micro&qid=1631213734&sr=8-12-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyTldSSlpVNDdHSVY5JmVuY3J5cHRlZElkPUEwMjUzNzE2MTdEUTI0VUlYOTRZUCZlbmNyeXB0ZWRBZElkPUEwNTgzMDMxVllKNDBaUldENjVUJndpZGdldE5hbWU9c3BfbXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==)
- Adafruit Micro-SD Breakout - [Adafruit](https://www.adafruit.com/product/254)
- OLED Display Module (128x32) - [Amazon](https://www.amazon.com/dp/B079BN2J8V/?coliid=I3BBVOU5FFFGML&colid=3Q4TZ7VNGY5YL&psc=1&ref_=lv_ov_lig_dp_it)
- 5 Way Momentary Push Button Switch - [Amazon](https://www.amazon.com/dp/B0147XLUVS/?coliid=I3BRLOD3YZ3PQI&colid=3Q4TZ7VNGY5YL&psc=1&ref_=lv_ov_lig_dp_it)
- OTG Adapter - [Amazon](https://www.amazon.com/dp/B015GZOHKW/?coliid=I2I3R53QJ40H8L&colid=3Q4TZ7VNGY5YL&psc=1&ref_=lv_ov_lig_dp_it)
- Micro-SD Card - [Amazon](https://www.amazon.com/SanDisk-128GB-MicroSDXC-Ultra-Memory/dp/B07XDCZ9J3/ref=sr_1_11?dchild=1&keywords=micro+sd+card&qid=1631213922&sr=8-11)

#### Wiring
- Adafruit Micro-SD Breakout
    - CLK    - Pin 15
    - D0     - Pin 14
    - D1     - Pin 16
    - CS     - Pin 10

- SSD1306 Display
    - SDA    - Pin 2
    - SCL    - Pin 3

- Momentary Switch
    - Right  - Pin 7
    - Center - Pin 8
    - Left   - Pin 9
    - Side   - GND

---

## Sample Usage

To use, simply add Ducky Script `.txt` files to the micro-sd card and insert in the reader. 

Once plugged in the root dir can be scrolled using the momentary switch. Once on the desired file press the momentary switch and the script will execute. 

More info on Ducky Script syntax can be found [here](https://docs.hak5.org/hc/en-us/articles/360010555153-Ducky-Script-the-USB-Rubber-Ducky-language)

##### HelloWorld.txt
```
REM This script will open notepad and type the string Hello, World!!!
DELAY 1000
REM Press the windows key, then type r 
GUI r
DELAY 500
ENTER
DELAY 500
REM Open Notepad
STRING notepad
DELAY 500
ENTER
DELAY 500
STRING Hello World!!!
ENTER
```
