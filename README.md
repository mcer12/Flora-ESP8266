# Flora-ESP8266
Flora is an open source ESP8266 VFD clock, you can make a 4-digit IV-22 variant or 6-digit IV-6 variant. Designed to be low profile and as small as possible, using widely available components. 

**NOTE: Improved IV-22 pcb is in the works, I suggest to wait for it.**

- IV-22 version is tested and working, schematic, gerber and BOM are included.
- IV-6 version should be working, schematic is included but pcb needs to be tested before I can publish gerber.
- IV-12 version TBA

**IV-22 enclosure model:**
https://www.thingiverse.com/thing:4744087

I will try to add some notes to the essembly but it's actually fairly straight forward. Can be completely sourced from LCSC and partially assembled using JLCPCB assembly service.

**Some things to note:**
1) Colons made out of WS2812 addressable leds
2) CH340 with NodeMCU style auto-reset built-in for easy programming / debugging
3) Each segment driven directly, not multiplexed
4) NTP based, no battery / RTC. Connect to wifi and you're done.
5) 150hz refresh rate using HW interrupt, not affected by wifi activity.
6) 3 levels of brightness, each with 8 more levels for dimming / crossfade.
7) No buttons design. Simple configuration portal is used for settings.
8) Daylight saving (summer time) support built in and super simple to set up.
9) MicroUSB connector, Below 500mA power consumption on 5V on full brightness.

**Images:**

![alt text](https://raw.githubusercontent.com/mcer12/Flora-ESP8266/main/Images/screenshot.png)


**License:**  
GPL-3.0 License  
LGPL-2.1 License (modified SPI library)
