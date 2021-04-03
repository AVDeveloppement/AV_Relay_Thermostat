//__________________________________________________________________________________
// Copyright (c) 2021, Alexandre <contact@avdeveloppement.eu>.  All rights reserved.
// Mozilla Public License Version 2.0
// See the accompanying LICENSE file for terms.
//__________________________________________________________________________________

void menuIsCanCheckAndRefresh() {
  // if menu are temperature (check when in settings menu result in slow change menu)
  // and we have no time or last time is over time out
  if (actualMenuShow == ActualMenu::AM_DATA_TEMPERATURE &&
      (lastCheckAndRefresh == 0 || (millis() - lastCheckAndRefresh) >= timeOutCheckAndRefresh)) {
    canCheckAndRefresh = true;
    lastCheckAndRefresh = millis(); // store last time we allow get, check and repaint data screen
  } else {
    canCheckAndRefresh = false;
  }
}

//reset stats given in param and ask save it
void menuResetStatsTemp(myTemperature* temp) {
  temp->stats.lowest = temp->actual;
  temp->stats.maximum = temp->actual;
  temp->stats.needSave = true;
  temp->stats.needRedraw = true;//not needed since after reset we go back to temp screen and it redraw all
}

// when user press up button
void menuUp() {
  switch ((ActualMenu)actualMenuShow) {
    case ActualMenu::AM_SETTINGS_MAIN:
      menu.up();
      break;
    case ActualMenu::AM_SETTINGS_TEMPERATURE:
      menuTemp.up();
      break;
    case ActualMenu::AM_SETTINGS_TEMP_TARGET:
    case ActualMenu::AM_SETTINGS_TEMP_DELTA:
    case ActualMenu::AM_SETTINGS_TEMP_CORRECTION:
      if (actualTempChange < tempMinMaxFail[TEMP_FAIL_MAX]) actualTempChange += 0.1;
      break;
    case ActualMenu::AM_SETTINGS_TEMP_CALCULATION_MODE:
      menuCalculationMode.up();
      break;
    case ActualMenu::AM_SETTINGS_RESET_STATS:
      menuResetStats.up();
      break;
    case ActualMenu::AM_SETTINGS_RESET_ALL:
      menuResetAll.up();
      break;
    default:
      break;
  }
}

// when user press select in menu reset stats of both temperature
void menuSelectionStatsBoth() {
  switch ((SelectionReset)menuResetStats.selection()) {
    case SelectionReset::SR_CONFIRM:
      menuResetStatsTemp(&temperature[0]);
      menuResetStatsTemp(&temperature[1]);
      newActualMenuShow = ActualMenu::AM_DATA_TEMPERATURE;
      break;
    case SelectionReset::SR_BACK:
      newActualMenuShow = ActualMenu::AM_SETTINGS_MAIN;
      break;
    default:
      break;
  }
}

// when user press select in menu reset stats of one temperature
void menuSelectionStatsTemp(myTemperature* temp) {
  switch ((SelectionReset)menuResetStats.selection()) {
    case SelectionReset::SR_CONFIRM:
      menuResetStatsTemp(temp);
      newActualMenuShow = ActualMenu::AM_DATA_TEMPERATURE;
      break;
    case SelectionReset::SR_BACK:
      newActualMenuShow = ActualMenu::AM_SETTINGS_TEMPERATURE;
      break;
    default:
      break;
  }
}

// when user press select button in menu of calculation mode
void menuSelectionCalculationMode(myTemperature* temp) {
  switch ((SelectionCalculationMode)menuCalculationMode.selection()) {
    case SelectionCalculationMode::SCM_HEATING: {
        temp->settings.calculationMode = CALCULATION_MODE_HEATING;
        settingsSave();
        break;
      }
    case SelectionCalculationMode::SCM_COOLING: {
        temp->settings.calculationMode = CALCULATION_MODE_COOLING;
        settingsSave();
        break;
      }
    case SelectionCalculationMode::SCM_BACK:
      // nothing to do, get back below
      break;
    default:
      break;
  }
  newActualMenuShow = ActualMenu::AM_SETTINGS_TEMPERATURE;
}

// when user press select button in menu of one temperature
void menuSelectionSettingsTemperature() {
  switch ((SelectionSettingsTemp)menuTemp.selection()) {
    case SelectionSettingsTemp::SST_RESET_STATS:
      newActualMenuShow = ActualMenu::AM_SETTINGS_RESET_STATS;
      break;
    case SelectionSettingsTemp::SST_TEMP_TARGET:
      newActualMenuShow = ActualMenu::AM_SETTINGS_TEMP_TARGET;
      break;
    case SelectionSettingsTemp::SST_TEMP_DELTA:
      newActualMenuShow = ActualMenu::AM_SETTINGS_TEMP_DELTA;
      break;
    case SelectionSettingsTemp::SST_TEMP_CORRECTION:
      newActualMenuShow = ActualMenu::AM_SETTINGS_TEMP_CORRECTION;
      break;
    case SelectionSettingsTemp::SST_CALCULATION_MODE:
      newActualMenuShow = ActualMenu::AM_SETTINGS_TEMP_CALCULATION_MODE;
      break;
    case SelectionSettingsTemp::SST_BACK:
      newActualMenuShow = ActualMenu::AM_SETTINGS_MAIN;
      break;
    default:
      break;
  }
}

// when user press select in menu reset app
void menuSelectionResetAll() {
  switch ((SelectionReset)menuResetAll.selection()) {
    case SelectionReset::SR_CONFIRM:
      configResetApp();//write EEPROM for prevent load and restart with default value
      break;
    case SelectionReset::SR_BACK:
      newActualMenuShow = ActualMenu::AM_SETTINGS_MAIN;
      break;
    default:
      break;
  }
}

//when user press select in main menu
void menuSelectionSettings() {
  switch ((SelectionSettingsMain)menu.selection()) {
    case SelectionSettingsMain::SSM_RESET_STATS:
      actualSettingsTemperature = SettingsTemperature::ST_BOTH;
      newActualMenuShow = ActualMenu::AM_SETTINGS_RESET_STATS;
      break;
    case SelectionSettingsMain::SSM_TEMP_1:
      actualSettingsTemperature = SettingsTemperature::ST_TEMP_1;
      newActualMenuShow = ActualMenu::AM_SETTINGS_TEMPERATURE;
      break;
    case SelectionSettingsMain::SSM_TEMP_2:
      actualSettingsTemperature = SettingsTemperature::ST_TEMP_2;
      newActualMenuShow = ActualMenu::AM_SETTINGS_TEMPERATURE;
      break;
    case SelectionSettingsMain::SSM_RESET_ALL:
      newActualMenuShow = ActualMenu::AM_SETTINGS_RESET_ALL;
      break;
    case SelectionSettingsMain::SSM_BACK:
      newActualMenuShow = ActualMenu::AM_DATA_TEMPERATURE;
      break;
    default:
      break;
  }
}

// when user press select button in a menu
void menuSelection() {
  if (menuChanged) return;

  switch ((ActualMenu)actualMenuShow) {
    case ActualMenu::AM_SETTINGS_MAIN:
      menuSelectionSettings();
      break;
    case ActualMenu::AM_SETTINGS_TEMPERATURE:

      menuSelectionSettingsTemperature();
      break;
    case ActualMenu::AM_SETTINGS_TEMP_TARGET: {
        if (actualSettingsTemperature == SettingsTemperature::ST_TEMP_1)
          temperature[0].settings.target = actualTempChange;
        else if (actualSettingsTemperature == SettingsTemperature::ST_TEMP_2)
          temperature[1].settings.target = actualTempChange;

        settingsSave();
        newActualMenuShow = ActualMenu::AM_SETTINGS_TEMPERATURE;
        break;
      }
    case ActualMenu::AM_SETTINGS_TEMP_DELTA: {
        if (actualSettingsTemperature == SettingsTemperature::ST_TEMP_1)
          temperature[0].settings.delta = actualTempChange;
        else if (actualSettingsTemperature == SettingsTemperature::ST_TEMP_2)
          temperature[1].settings.delta = actualTempChange;

        settingsSave();

        newActualMenuShow = ActualMenu::AM_SETTINGS_TEMPERATURE;
        break;
      }
    case ActualMenu::AM_SETTINGS_TEMP_CORRECTION: {
        if (actualSettingsTemperature == SettingsTemperature::ST_TEMP_1)
          temperature[0].settings.correction = actualTempChange;
        else if (actualSettingsTemperature == SettingsTemperature::ST_TEMP_2)
          temperature[1].settings.correction = actualTempChange;

        settingsSave();

        newActualMenuShow = ActualMenu::AM_SETTINGS_TEMPERATURE;
        break;
      }
    case ActualMenu::AM_SETTINGS_TEMP_CALCULATION_MODE: {
        if (actualSettingsTemperature == SettingsTemperature::ST_TEMP_1) menuSelectionCalculationMode(&temperature[0]);
        if (actualSettingsTemperature == SettingsTemperature::ST_TEMP_2) menuSelectionCalculationMode(&temperature[1]);
        break;
      }
    case ActualMenu::AM_SETTINGS_RESET_STATS: {
        if (actualSettingsTemperature == SettingsTemperature::ST_BOTH) menuSelectionStatsBoth();
        if (actualSettingsTemperature == SettingsTemperature::ST_TEMP_1) menuSelectionStatsTemp(&temperature[0]);
        if (actualSettingsTemperature == SettingsTemperature::ST_TEMP_2) menuSelectionStatsTemp(&temperature[1]);
        break;
      }
    case ActualMenu::AM_SETTINGS_RESET_ALL:
      menuSelectionResetAll();
      break;
    default:
      break;
  }

  menuChanged = true; //prevent change menu quickly, user must release button before change menu again
}

// when user press down button
void menuDown() {
  switch ((ActualMenu)actualMenuShow) {
    case ActualMenu::AM_SETTINGS_MAIN:
      menu.down();
      break;
    case ActualMenu::AM_SETTINGS_TEMPERATURE:
      menuTemp.down();
      break;
    case ActualMenu::AM_SETTINGS_TEMP_TARGET:
    case ActualMenu::AM_SETTINGS_TEMP_DELTA:
    case ActualMenu::AM_SETTINGS_TEMP_CORRECTION:
      if (actualTempChange > tempMinMaxFail[TEMP_FAIL_MIN]) actualTempChange -= 0.1;
      break;
    case ActualMenu::AM_SETTINGS_TEMP_CALCULATION_MODE:
      menuCalculationMode.down();
      break;
    case ActualMenu::AM_SETTINGS_RESET_STATS:
      menuResetStats.down();
      break;
    case ActualMenu::AM_SETTINGS_RESET_ALL:
      menuResetAll.down();
      break;
    default:
      break;
  }
}

void menuCheck() {
  Key newButton = getPressedButton(BUTTON_PIN);

  /*SERIAL_PRINT("newButton: ");
    SERIAL_PRINTLN((int)newButton);*/
  //if user pres a button we updade last time var for auto get out settings function
  switch (newButton) {
    case Key::K_UP:
      startTimeLastButtonPressed = millis();
      SERIAL_PRINTLN("menu up");
      break;
    case Key::K_DOWN:
      startTimeLastButtonPressed = millis();
      SERIAL_PRINTLN("menu down");
      break;
    case Key::K_SELECT:
      startTimeLastButtonPressed = millis();
      SERIAL_PRINTLN("menu select");
      break;
    case Key::K_NONE:
      menuChanged = false;//user not press button we can set this to false for reactive select button for change menu
      break;
    default:
      break;
  }

  // by default we stay on same screen
  newActualMenuShow = ActualMenu::AM_SAME;

  // not in settings and user continue press same button
  if (actualMenuShow == ActualMenu::AM_DATA_TEMPERATURE) {
    // if continue press select
    if (newButton == button && newButton == Key::K_SELECT) {
      // if begin, store now for know when he start press button
      if (startTimeButtonPressed == 0) startTimeButtonPressed = millis();

      // if we have a time we check if last time is over time out
      if (startTimeButtonPressed != 0 && (millis() - startTimeButtonPressed) >= timeOutButtionSelect) {
        newActualMenuShow = ActualMenu::AM_SETTINGS_MAIN; //show main menu
        newButton == Key::K_NONE; // reset actual button
        startTimeButtonPressed = 0; // reset timer
      }
    } else {
      startTimeButtonPressed = 0; // reset timer
    }
  }

  // if in a settings menu and not press key
  if (actualMenuShow != ActualMenu::AM_DATA_TEMPERATURE && newButton == Key::K_NONE) {
    // if we have a time we check if last time is over time out
    if (startTimeLastButtonPressed != 0 && (millis() - startTimeLastButtonPressed) >= timeOutGetOutSettings) {
      newActualMenuShow = ActualMenu::AM_DATA_TEMPERATURE; // show data temperature
      newButton == Key::K_NONE; // reset actual button
      startTimeLastButtonPressed = 0; // reset timer
    }
  }

  if (newButton != Key::K_NONE) {
    button = newButton;
    switch (button) {
      case Key::K_UP:
        menuUp();
        break;
      case Key::K_DOWN:
        menuDown();
        break;
      case Key::K_SELECT:
        menuSelection();
        break;
      default:
        break;
    }
  }

#ifdef SERIAL_OUTPUT
  /*SERIAL_PRINT("menu actualMenuShow ");
    SERIAL_PRINTLN((int)actualMenuShow);*/

  if (newActualMenuShow !=  ActualMenu::AM_SAME && newActualMenuShow != actualMenuShow) {
    SERIAL_PRINT("menu newActualMenuShow ");
    SERIAL_PRINTLN((int)newActualMenuShow);
  }
  //SERIAL_PRINTLN(analogRead(BUTTON_PIN));
#endif

  // if menu are changed we need to do more action, clear screen, set correct font, force refresh
  if (newActualMenuShow != ActualMenu::AM_SAME && newActualMenuShow != actualMenuShow) {
    actualMenuShow = newActualMenuShow;//store menu showed for next loop

    //remove all for wite properly new data on screen
    display.clear();

    // set different font for menu with text selection or data temp and menu float selection
    switch ((ActualMenu)actualMenuShow) {
      case ActualMenu::AM_DATA_TEMPERATURE:
      case ActualMenu::AM_SETTINGS_TEMP_TARGET:
      case ActualMenu::AM_SETTINGS_TEMP_DELTA:
      case ActualMenu::AM_SETTINGS_TEMP_CORRECTION: {
          display.setFixedFont(ssd1306xled_font6x8);
          if (actualMenuShow == ActualMenu::AM_DATA_TEMPERATURE) {
            displayFullDataTemperature();//force repaint show stats, actual, target
          }
          break;
        }
      default:
        display.setFixedFont(ssd1306xled_font8x16);//set bigger font for settings menu
        break;
    }
  }

  //if menu are temperature
  if (actualMenuShow == ActualMenu::AM_DATA_TEMPERATURE) {
    displayDataTemperature();//show data if something changed
  } else if (actualMenuShow == ActualMenu::AM_SETTINGS_MAIN) {
    menu.show(display);
  } else if (actualMenuShow == ActualMenu::AM_SETTINGS_TEMPERATURE) {
    menuTemp.show(display);
  } else if (actualMenuShow == ActualMenu::AM_SETTINGS_TEMP_TARGET) {
    //if we start show menu
    if (newActualMenuShow != ActualMenu::AM_SAME) {
      //we get actual value from setting
      if (actualSettingsTemperature == SettingsTemperature::ST_TEMP_1)
        actualTempChange =  temperature[0].settings.target;
      else if (actualSettingsTemperature == SettingsTemperature::ST_TEMP_2)
        actualTempChange =  temperature[1].settings.target;
    }
    displayScreenMenuTemperatureChange();
  } else if (actualMenuShow == ActualMenu::AM_SETTINGS_TEMP_DELTA) {
    //if we start show menu
    if (newActualMenuShow != ActualMenu::AM_SAME) {
      //we get actual value from setting
      if (actualSettingsTemperature == SettingsTemperature::ST_TEMP_1)
        actualTempChange =  temperature[0].settings.delta;
      else if (actualSettingsTemperature == SettingsTemperature::ST_TEMP_2)
        actualTempChange =  temperature[1].settings.delta;
    }
    displayScreenMenuTemperatureChange();
  } else if (actualMenuShow == ActualMenu::AM_SETTINGS_TEMP_CORRECTION) {
    //if we start show menu
    if (newActualMenuShow != ActualMenu::AM_SAME) {
      //we get actual value from setting
      if (actualSettingsTemperature == SettingsTemperature::ST_TEMP_1)
        actualTempChange =  temperature[0].settings.correction;
      else if (actualSettingsTemperature == SettingsTemperature::ST_TEMP_2)
        actualTempChange =  temperature[1].settings.correction;
    }
    displayScreenMenuTemperatureChange();
  } else if (actualMenuShow == ActualMenu::AM_SETTINGS_TEMP_CALCULATION_MODE) {
    menuCalculationMode.show(display);
  } else if (actualMenuShow == ActualMenu::AM_SETTINGS_RESET_STATS) {
    // if we start show menu
    if (newActualMenuShow != ActualMenu::AM_SAME) {
      // set focus on back button
      while ((SelectionReset)menuResetStats.selection() != SelectionReset::SR_BACK) menuResetStats.down();
    }
    menuResetStats.show(display);
  } else if (actualMenuShow == ActualMenu::AM_SETTINGS_RESET_ALL) {
    // if we start show menu
    if (newActualMenuShow != ActualMenu::AM_SAME) {
      // set focus on back button
      while ((SelectionReset)menuResetAll.selection() != SelectionReset::SR_BACK) menuResetAll.down();
    }
    menuResetAll.show(display);
  }
}
