# Flora-ESP8266
Flora is an open source ESP8266 VFD clock, you can make a 4-digit IV-22 variant or 6-digit IV-6 variant. Designed to be low profile and as small as possible, using widely available components. 

Schematic and BOM included. I will also some notes for the assembly and 3d models to print shortly. Can be completely sourced from LCSC and partially assembled using JLCPCB assembly service.

Some things to note:
1) Colons made out of WS2812 addressable leds
2) CH340 with NodeMCU style auto-reset built-in for easy programming / debugging
3) Each segment driven directly, not multiplexed
4) NTP based, no battery / RTC. Connect to wifi and you're done.
5) 150hz refresh rate using HW interrupt, not affected by wifi activity.
6) 3 levels of brightness, each with 8 more levels for dimming / crossfade.
7) No buttons design. Simple configuration portal is used for settings.
8) Daylight saving (summer time) support built in and super simple to set up.
9) MicroUSB connector, Below 500mA power consumption on 5V on full brightness.
