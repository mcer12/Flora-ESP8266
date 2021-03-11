# Flora-ESP8266
Flora is an open source ESP8266 VFD clock, you can make a 4-digit IV-22 variant or 6-digit IV-6 variant. Designed to be low profile and as small as possible, using widely available components. 

- Updated IV-22 V2 variant is tested and working, schematic, gerber and BOM are included.
- IV-6 variant should be working, schematic is included but pcb needs to be tested before I can publish gerber.
- IV-12 variant TBA

**Wiki**
https://github.com/mcer12/Flora-ESP8266/wiki/

**IV-22 enclosure model:**
https://www.thingiverse.com/thing:4744087

I will try to add some notes to the essembly but it's actually fairly straight forward. Can be completely sourced from LCSC and partially assembled using JLCPCB assembly service.

**Some things to note:**
1) This design is made with low-profile and small footprint in mind using only 3mm high components. Feel free to adopt it for your project!
2) Colons made out of WS2812 addressable leds
3) CH340 with NodeMCU style auto-reset built-in for easy programming / debugging
4) Each segment driven directly, not multiplexed
5) NTP based, no battery / RTC. Connect to wifi and you're done.
6) 150hz refresh rate using HW ISR timer, zero flicker and not affected by wifi activity.
7) 3 levels of brightness, each with 8 more levels for dimming / crossfade. 32 pwm steps in total for each segment!
8) No buttons design. Simple configuration portal is used for settings.
9) Daylight saving (summer time) support built in and super simple to set up.
10) MicroUSB connector, Below 500mA power consumption on 5V on full brightness.

**Images:**

![alt text](https://raw.githubusercontent.com/mcer12/Flora-ESP8266/main/Images/clock.jpg)  

![alt text](https://raw.githubusercontent.com/mcer12/Flora-ESP8266/main/Images/screenshot.png)  

**License:**  
GPL-3.0 License  
LGPL-2.1 License (modified SPI library)
