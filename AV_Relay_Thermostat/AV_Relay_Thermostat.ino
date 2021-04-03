//__________________________________________________________________________________
// Relay Thermostat
// For control relay based on temperature
// -2 relay control.
// -mode heating, cooling.
// -correction for set correct actual temperature.
// -delta for start again (ex restart 2 degree below target).
// -statistics with min max temperature.
// -menu for change settings.
// -screen for display: temperature, statistics, settings.
// -languages: French, English (default)
//
// Copyright (c) 2021, Alexandre <contact@avdeveloppement.eu>.  All rights reserved.
// Mozilla Public License Version 2.0
// See the accompanying LICENSE file for terms.
//__________________________________________________________________________________

/**
   PIN Arduino NANO with OLED 128X64
   A4                    SDA
   A5                    SCK
   GND                   GND
   3.3V                  VDD

   PIN Arduino NANO with NTC Thermistor 10K 1% 3950 Temperature Sensor Probe Waterproof
   GND                   Thermistor PIN 1 (Both PIN are same)
   3.3V to 10k resistance to Thermistor PIN 2
                           |_ to NANO A2 and A3
   3.3V to NANO AREF (for NANO know we use 3.3V as reference)

   PIN Arduino NANO with Relay Shield
   D5                    IN1
   D6                    IN2
   GND                   DC-
   5V                    DC+

   PIN Arduino NANO with Button shield
   5V to resistor 10k to resistor 1k to button 1 to gnd
                       |              |_ to resistor 220ohms to button 2 to gnd
                       |_ Arduino NANO A6                     |_ to resistor 220ohms to button 3 to gnd
*/

#include <Arduino.h>
#include <avr/wdt.h>
// Settings save/load
#include <EEPROM.h>
// OLED Screen
#include <lcdgfx.h>
#include <lcdgfx_gui.h>
#include "menu.h"

void setup() {
  configInit();
}

void loop() {
  menuIsCanCheckAndRefresh(); // verify timer if we allow for temperature and relay check

  temperatureCheck(); // try get temperature

  relayCheck(); // if temperature changed set state relay

  menuCheck();

  //Serial.println(configFreeRam());

  configLoopDelay();
}
