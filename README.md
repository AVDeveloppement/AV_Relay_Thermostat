# AV Relay Thermostat
## Written with [Arduino](https://www.arduino.cc/ "Arduino") 1.8.13, and library [lcdgfx](https://github.com/lexus2k/lcdgfx "lcdgfx") thanks to their author and community for their work.

### For control relay based on temperature:
* 2 relay control.
* mode heating, cooling.
* correction for set correct actual temperature.
* delta for start again (ex restart 2 degree below target).
* statistics with min max temperature.
* menu for change settings.
* screen for display: temperature, statistics, settings.
* languages: French, English (default).


### Hardware needed:
* 1 x Arduino Nano (with latest bootloader, else reset cannot use soft restart)
* 1 x OLED 128x64 I2C Monochrome Display
* 2 x Temperature Sensor (Thermistorr type NTC resistance at 25° 10k)
* 3 x 10K Resistor
* 3 x 1k Resistor
* 3 x Pushbutton
* 1 x 2-Channel 5v Relay Shield


![Screenshot](/images/1617379370131.png?raw=true "Screenshot")
![Screenshot](/images/1617379370134.png?raw=true "Screenshot")
![Screenshot](/images/1617379370123.png?raw=true "Screenshot")
![Screenshot](/images/1617379370109.png?raw=true "Screenshot")
![Screenshot](/images/AV_Relay_Thermostat_bb.png?raw=true "Screenshot")
![Screenshot](/images/AV_Relay_Thermostat_schema.png?raw=true "Screenshot")

Any idea :point_up: Questions :question: Go to :point_right: [Discussions](https://github.com/AVDeveloppement/AV_Relay_Thermostat/discussions "Discussions") :point_left:

License: [Mozilla Public License Version 2.0](https://github.com/AVDeveloppement/AV_Relay_Thermostat/raw/main/license.txt "Mozilla Public License Version 2.0")

Enjoy.