# Simple Arduino IR Remote Control for XBMC/Kodi

This project uses an [Arduino Pro Mini (a little Leonardo clone)](https://www.amazon.com/gp/product/B012FOV17O/) to pick up IR signals from my TV remote and emulate a keyboard. Fortunately for me, my remote has several extra buttons that aren't used during normal operation. This lets me intercept the commands.

I'm using [Ken Shirriff's IRRemote library](https://github.com/Arduino-IRremote/Arduino-IRremote) to decode the IR signals. This project is specifically set up for my oCOSMO CE4031 TV's remote, but the code should be easily adapted to any supported codes.


## Wiring
The Leonardo version's wiring is as simple as:

![Wiring Diagram](https://raw.githubusercontent.com/corbanmailloux/ArduinoKodiIRRemote/master/LeonardoKodiIRRemote_bb.png)
