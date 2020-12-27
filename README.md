# Flora-ESP8266
Flora is an open source 6-digit WiFi VFD clock, based on ESP8266. Designed to be low profile and as small as possible, using widely available components. 

Schematic and BOM included. I will also add gerbers and some notes for the assembly. Can be completely sourced from LCSC and partially assembled using JLCPCB assembly service.

Some things to note:
1) Designed for IV-6 VFD tubes
2) Colons made out of WS2812 addressable leds
3) CH340 with NodeMCU style auto-reset built-in for easy programming / debugging
4) Each segment driven directly, not multiplexed
5) NTP based, no battery / RTC. Connect to wifi and you're done.
6) 150hz refresh rate using HW interrupt, not affected by wifi activity.
7) 3 levels of brightness, each with 8 more levels for dimming / crossfade.
8) No buttons design. Simple configuration portal is used for settings.
9) Daylight saving (summer time) support built in and super simple to set up.
10) MicroUSB connector, Below 500mA power consumption on 5V on full brightness.
