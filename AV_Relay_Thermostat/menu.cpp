//__________________________________________________________________________________
// Copyright (c) 2021, Alexandre <contact@avdeveloppement.eu>.  All rights reserved.
// Mozilla Public License Version 2.0
// See the accompanying LICENSE file for terms.
//__________________________________________________________________________________

#include "menu.h"
#include <lcdgfx.h>

Key getPressedButton(uint8_t analogPin) {
  int buttonValue = lcd_adcRead(analogPin);

  //Serial.println(buttonValue);

  if (buttonValue > 90 && buttonValue < 330) {
    if (buttonValue < 170) {
      return Key::K_DOWN;
    }
    else if (buttonValue < 250) {
      return Key::K_SELECT;
    }
    else if (buttonValue < 330) {
      return Key::K_UP;
    }
  }

  return Key::K_NONE;
}
