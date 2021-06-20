# Flora-ESP8266
Flora is an open source ESP8266 VFD clock, you can make a 4-digit IV-22 variant or 6-digit IV-6 variant. Designed to be low profile and as small as possible, using widely available components. 

**STATUS:**
- **IV-22 variant:** Tested and working, schematic, gerber BOM and 3D model of cover are included. Assembly guide added to wiki.
- **IV-6 variant:** Tested and working. Schematic, gerber and BOM included. 3D printed cover will be added below once some adjustments have been made.
- **IV-12 variant:** Tested and working. Schematic, gerber and BOM and 3D model of cover are included.

**Wiki**
https://github.com/mcer12/Flora-ESP8266/wiki/

**IV-22 enclosure model:**
https://www.thingiverse.com/thing:4744087

**IV-12 enclosure model:**
https://www.thingiverse.com/thing:4890505

**Some things to note:**
1) This design is made with small footprint and ultra low-profile in mind, using only 3mm high components.
2) Using modern and widely available components.
3) Can be completely sourced from LCSC (except for the VFDs of course) and partially assembled using JLCPCB assembly service.
4) Colons made out of WS2812 addressable leds
5) CH340 with NodeMCU style auto-reset built-in for easy programming / debugging
6) Each segment driven directly, not multiplexed
7) NTP based, no battery / RTC. Connect to wifi and you're done.
8) Simple and easy to set up, mobile-friendly web interface
9) diyHue and remote control support
10) 100hz refresh rate using HW ISR timer, zero flicker and not affected by wifi activity.
11) 3 levels of brightness, each with 8 more levels for dimming / crossfade. 48 pwm steps in total for each segment!
12) No buttons design. Simple configuration portal is used for settings.
13) Daylight saving (summer time) support built in and super simple to set up.
14) MicroUSB connector (USB type C on IV-12 version), Below 500mA power consumption on 5V on full brightness (800mA on IV-12 version).


**Images:**

![alt text](https://raw.githubusercontent.com/mcer12/Flora-ESP8266/main/Images/IV22_1.jpg)  

![alt text](https://raw.githubusercontent.com/mcer12/Flora-ESP8266/main/Images/IV6_1.jpg)  

![alt text](https://raw.githubusercontent.com/mcer12/Flora-ESP8266/main/Images/screenshot.png)  

**License:**  
GPL-3.0 License  
LGPL-2.1 License (modified SPI library)  
Mozilla Public License 2.0 (iro.js, https://github.com/jaames/iro.js)
