//__________________________________________________________________________________
// Copyright (c) 2021, Alexandre <contact@avdeveloppement.eu>.  All rights reserved.
// Mozilla Public License Version 2.0
// See the accompanying LICENSE file for terms.
//__________________________________________________________________________________

const char* menuItems[] =
{
#ifdef languageFrench
  "Effacer stats",
  "Temperature #1",
  "Temperature #2",
  "Reinitialiser",
  "<- Retour",
#elif
  "Clear stats",
  "Temperature #1",
  "Temperature #2",
  "Reset",
  "<- Back",
#endif
};
LcdGfxMenu menu(menuItems, sizeof(menuItems) / sizeof(char *));

const char* menuTempItems[] =
{
#ifdef languageFrench
  "Effacer stats",
  "Declenchement",
  "Difference",
  "Etalonnage",
  "Mode",
  "<- Retour",
#elif
  "Clear stats",
  "Triggering",
  "Difference",
  "Calibration",
  "Mode",
  "<- Back",
#endif
};
LcdGfxMenu menuTemp(menuTempItems, sizeof(menuTempItems) / sizeof(char *));

const char* menuCalculationModeItems[] =
{
#ifdef languageFrench
  "Chauffer",
  "Refroidir",
  "<- Retour",
#elif
  "Heating",
  "Cooling",
  "<- Back",
#endif
};
LcdGfxMenu menuCalculationMode(menuCalculationModeItems, sizeof(menuCalculationModeItems) / sizeof(char *));

const char* menuResetStatsItems[] =
{
#ifdef languageFrench
  "Effacer stats",
  "<- Retour",
#elif
  "Clear stats",
  "<- Back",
#endif
};
LcdGfxMenu menuResetStats(menuResetStatsItems, sizeof(menuResetStatsItems) / sizeof(char *));

const char* menuResetAllItems[] =
{
#ifdef languageFrench
  "REINITIALISER!",
  "<- Retour",
#elif
  "RESET!!!",
  "<- Back",
#endif
};
LcdGfxMenu menuResetAll(menuResetAllItems, sizeof(menuResetAllItems) / sizeof(char *));

void displayPrint(const int* x, const int* y, float* value, EFontStyle fs, const uint8_t factor = 0) {

  /*SERIAL_PRINT("displayPrint value:");
    SERIAL_PRINTLN(*value);*/

  if (*value <= tempMinMaxFail[TEMP_FAIL_MIN] || *value >= tempMinMaxFail[TEMP_FAIL_MAX]) {
    display.printFixedN(*x, *y, "ERROR", fs, factor);
    return;
  }

  char tempStr[4] = "   ";

  dtostrf(*value, sizeof(tempStr), 1, tempStr);
  display.printFixedN(*x, *y, tempStr, fs, factor);

  // now check size text writed and clear space at right to TEXT_SIZE_PIXELS_MAX
  // for cleaning and show small value without rest of hight value behind
  // need add value * factor for correct number of px
  const uint8_t textSizeMax = TEXT_SIZE_PIXELS_MAX + (TEXT_SIZE_PIXELS_MAX * factor);
  uint8_t textSize = nfUtils.getTextSize(reinterpret_cast<const char*>(&tempStr));
  textSize += textSize * factor;
  while (textSize < textSizeMax) {
    display.printFixedN(*x + textSize, *y, " ", fs, factor);
    textSize += TEXT_SIZE_PIXELS_ONE_CARACT + (TEXT_SIZE_PIXELS_ONE_CARACT * factor);
  }

  /*SERIAL_PRINT("displayPrint getTextSize:");
    SERIAL_PRINTLN(nfUtils.getTextSize(reinterpret_cast<const char*>(&tempStr)));*/
}

void displayScreenStats() {
  if (!temperature[0].stats.needRedraw && !temperature[1].stats.needRedraw) return;

  temperature[0].stats.needRedraw = false;
  temperature[1].stats.needRedraw = false;

  /* SERIAL_PRINTLN("showScreenStats");
    SERIAL_PRINTLN(temperature[0].stats.maximum);
    SERIAL_PRINTLN(temperature[0].stats.lowest);
    SERIAL_PRINTLN(temperature[0].maxPosXY[0]);
    SERIAL_PRINTLN(temperature[0].lowPosXY[0]);*/

  display.printFixed(0, temperature[0].maxPosXY[1], "Max:");

  if (temperature[0].stats.maximum > tempMinMaxFail[TEMP_FAIL_MIN] && temperature[0].stats.maximum < tempMinMaxFail[TEMP_FAIL_MAX]) {
    displayPrint(&temperature[0].maxPosXY[0], &temperature[1].maxPosXY[1], &temperature[0].stats.maximum, STYLE_NORMAL);
  } else {
    display.printFixedN(temperature[0].maxPosXY[0], temperature[0].maxPosXY[1], " -- ", STYLE_NORMAL, 0);
  }

  if (temperature[1].stats.maximum > tempMinMaxFail[TEMP_FAIL_MIN] && temperature[1].stats.maximum < tempMinMaxFail[TEMP_FAIL_MAX]) {
    displayPrint(&temperature[1].maxPosXY[0], &temperature[1].maxPosXY[1], &temperature[1].stats.maximum, STYLE_NORMAL);
  } else {
    display.printFixedN(temperature[1].maxPosXY[0], temperature[1].maxPosXY[1], " -- ", STYLE_NORMAL, 0);
  }

  display.printFixed(0, temperature[0].lowPosXY[1], "Min:");

  if (temperature[0].stats.lowest > tempMinMaxFail[TEMP_FAIL_MIN] && temperature[0].stats.lowest < tempMinMaxFail[TEMP_FAIL_MAX]) {
    displayPrint(&temperature[0].lowPosXY[0], &temperature[0].lowPosXY[1], &temperature[0].stats.lowest, STYLE_NORMAL);
  } else {
    display.printFixedN(temperature[0].lowPosXY[0], temperature[0].lowPosXY[1], " -- ", STYLE_NORMAL, 0);
  }

  if (temperature[1].stats.lowest > tempMinMaxFail[TEMP_FAIL_MIN] && temperature[1].stats.lowest < tempMinMaxFail[TEMP_FAIL_MAX]) {
    displayPrint(&temperature[1].lowPosXY[0], &temperature[1].lowPosXY[1], &temperature[1].stats.lowest, STYLE_NORMAL);
  } else {
    display.printFixedN(temperature[1].lowPosXY[0], temperature[1].lowPosXY[1], " -- ", STYLE_NORMAL, 0);
  }
}

void displayScreenActual(struct myTemperature* temp) {
  if (!temp->needRedraw) return;

  temp->needRedraw = false;

  //TODO show if relay on/off
  /*uint16_t color = display.getColor();
    if (temp->relayState == RELAY_OFF) {
    display.invertColors();
    display.setBackground(color);
    }
    display.drawHLine(temp->relayOnHLine[DRAW_X1], temp->relayOnHLine[DRAW_Y1], temp->relayOnHLine[DRAW_X2]);
    //display.fillRect(temp->relayOnHLine[DRAW_X1], temp->relayOnHLine[DRAW_Y1], temp->relayOnHLine[DRAW_X2], temp->relayOnHLine[DRAW_Y2]);
    if (temp->relayState == RELAY_OFF) {
    display.invertColors();
    display.setColor(color);
    }*/

  displayPrint(&temp->actualPosXY[0], &temp->actualPosXY[1], &temp->actual, STYLE_NORMAL, 1);
}

void displayScreenTarget() {
  if (!temperature[0].settings.needRedraw && !temperature[1].settings.needRedraw) return;

  temperature[0].settings.needRedraw = false;
  temperature[1].settings.needRedraw = false;

  // create rect white for background
  NanoRect rect;
  rect.setRect(0, temperature[0].targetPosXY[1] - 1,  127, temperature[0].targetPosXY[1] + 15);
  display.fillRect(rect);

  // for write black in white rect
  display.invertColors();
  displayPrint(&temperature[0].targetPosXY[0], &temperature[0].targetPosXY[1], &temperature[0].settings.target, STYLE_NORMAL, 1);
  displayPrint(&temperature[1].targetPosXY[0], &temperature[1].targetPosXY[1], &temperature[1].settings.target, STYLE_NORMAL, 1);
  // set color normal again
  display.invertColors();
}

void displayScreenMenuTemperatureChange() {
  displayPrint(&actualTempChangeX, &actualTempChangeY, &actualTempChange, STYLE_NORMAL, 1);
}

void displayDataTemperature() {
  //if need repaint else exit
  if (!canCheckAndRefresh) return;

  displayScreenStats();

  displayScreenActual(&temperature[0]);
  displayScreenActual(&temperature[1]);

  displayScreenTarget();
}

void displayFullDataTemperature() {
  canCheckAndRefresh = true; // we force repaint

  temperature[0].stats.needRedraw = true;
  temperature[0].settings.needRedraw = true;
  temperature[0].needRedraw = true;
  temperature[1].stats.needRedraw = true;
  temperature[1].settings.needRedraw = true;
  temperature[1].needRedraw = true;

  displayDataTemperature();
}
