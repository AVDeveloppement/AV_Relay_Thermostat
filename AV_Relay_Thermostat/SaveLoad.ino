//__________________________________________________________________________________
// Copyright (c) 2021, Alexandre <contact@avdeveloppement.eu>.  All rights reserved.
// Mozilla Public License Version 2.0
// See the accompanying LICENSE file for terms.
//__________________________________________________________________________________

void settingsLoadFromSave(struct myTempSettings* tempSettings, struct myTempSettingsSave* tempSettingsSave) {
  tempSettings->target = tempSettingsSave->target;
  tempSettings->delta = tempSettingsSave->delta;
  tempSettings->correction = tempSettingsSave->correction;
  tempSettings->calculationMode = tempSettingsSave->calculationMode;
}

bool settingsLoad() {
  byte state = 0;
  EEPROM.get(EEPROM_ADRESS_SETTINGS_STATE, state);
  if (state == EEPROM_STATE_OK) {
    SERIAL_PRINTLN(F("Config in EEPROM, try load it"));

    myTempSettingsSave tempSettingsSave;

    EEPROM.get(EEPROM_ADRESS_SETTINGS, tempSettingsSave);
    settingsLoadFromSave(&temperature[0].settings, &tempSettingsSave);

    EEPROM.get(EEPROM_ADRESS_SETTINGS + EEPROM_SIZE_SETTINGS, tempSettingsSave);
    settingsLoadFromSave(&temperature[1].settings, &tempSettingsSave);

    return true;
  } else {
    SERIAL_PRINTLN(F("No config found in EEPROM"));
    return false;
  }
}

void statsLoadFromSave(struct myTempStats* tempStats, struct myTempStatsSave* tempStatsSave) {
  tempStats->lowest = tempStatsSave->lowest ;
  tempStats->maximum = tempStatsSave->maximum;
}

void statsLoad() {
  byte state = 0;
  myTempStatsSave tempStatsSave;

  EEPROM.get(EEPROM_ADRESS_STATS_STATE, state);
  if (state == EEPROM_STATE_OK) {
    SERIAL_PRINT(F("Stats "));
    SERIAL_PRINT(temperature[0].title);
    SERIAL_PRINTLN(F(" found in EEPROM, try load it"));

    EEPROM.get(EEPROM_ADRESS_STATS, tempStatsSave);
    statsLoadFromSave(&temperature[0].stats, &tempStatsSave);
  } else {
    SERIAL_PRINT(F("Stats "));
    SERIAL_PRINT(temperature[0].title);
    SERIAL_PRINTLN(F(" not found in EEPROM"));
  }
  /*SERIAL_PRINT(F("statsLoad() EEPROM_ADRESS_STATS_STATE: "));
    SERIAL_PRINTLN(EEPROM_ADRESS_STATS_STATE);

    SERIAL_PRINT(F("statsLoad() EEPROM_ADRESS_STATS: "));
    SERIAL_PRINTLN(EEPROM_ADRESS_STATS);

    SERIAL_PRINT(F("statsLoad() state: "));
    SERIAL_PRINTLN(state);*/

  EEPROM.get(EEPROM_ADRESS_STATS_STATE + EEPROM_SIZE_STATE + EEPROM_SIZE_STATS, state);
  if (state == EEPROM_STATE_OK) {
    SERIAL_PRINT(F("Stats "));
    SERIAL_PRINT(temperature[1].title);
    SERIAL_PRINTLN(F(" found in EEPROM, try load it"));

    EEPROM.get(EEPROM_ADRESS_STATS + EEPROM_SIZE_STATE + EEPROM_SIZE_STATS, tempStatsSave);
    statsLoadFromSave(&temperature[1].stats, &tempStatsSave);
  } else {
    SERIAL_PRINT(F("Stats "));
    SERIAL_PRINT(temperature[1].title);
    SERIAL_PRINTLN(F(" not found in EEPROM"));
  }
  /*SERIAL_PRINT(F("statsLoad() EEPROM_ADRESS_STATS_STATE + EEPROM_SIZE_STATE + EEPROM_SIZE_STATS: "));
    SERIAL_PRINTLN(EEPROM_ADRESS_STATS_STATE + EEPROM_SIZE_STATE + EEPROM_SIZE_STATS);

    SERIAL_PRINT(F("statsLoad() EEPROM_ADRESS_STATS + EEPROM_SIZE_STATE + EEPROM_SIZE_STATS: "));
    SERIAL_PRINTLN(EEPROM_ADRESS_STATS + EEPROM_SIZE_STATE + EEPROM_SIZE_STATS);

    SERIAL_PRINT(F("statsLoad() state: "));
    SERIAL_PRINTLN(state);*/
}

void settingsSaveToSave(myTempSettingsSave* tempSettingsSave, myTempSettings* tempSettings) {
  tempSettingsSave->target = tempSettings->target;
  tempSettingsSave->delta = tempSettings->delta;
  tempSettingsSave->correction = tempSettings->correction;
  tempSettingsSave->calculationMode = tempSettings->calculationMode;
}

void settingsSave() {
  byte state = 0;
  // check EEPROM was already write else need set ok
  EEPROM.get(EEPROM_ADRESS_SETTINGS_STATE, state);
  if (state != EEPROM_STATE_OK) {
    SERIAL_PRINTLN(F("No config in EEPROM, try save it"));
    state = EEPROM_STATE_OK;
    EEPROM.put(EEPROM_ADRESS_SETTINGS_STATE, state);
  } else {
    SERIAL_PRINTLN(F("Config in EEPROM, try overwrite it"));
  }

  myTempSettingsSave tempSettingsSave;

  settingsSaveToSave(&tempSettingsSave, &temperature[0].settings);
  EEPROM.put(EEPROM_ADRESS_SETTINGS, tempSettingsSave);

  settingsSaveToSave(&tempSettingsSave, &temperature[1].settings);
  EEPROM.put(EEPROM_ADRESS_SETTINGS + EEPROM_SIZE_SETTINGS, tempSettingsSave);
}


void statsSaveToSave(myTempStatsSave* tempStatsSave, myTempStats* tempStats) {
  tempStatsSave->lowest = tempStats->lowest;
  tempStatsSave->maximum = tempStats->maximum;
}

// save stats only timed and separatly for preserv EEPROM life
void statsSave(myTemperature* temp) {
  if (temp->stats.needSave) {

    /*SERIAL_PRINT(F("Save stats, millis(): "));
      SERIAL_PRINT(millis());
      SERIAL_PRINT(F(" lastTimeSave: "));
      SERIAL_PRINT(temp->stats.lastTimeSave);
      SERIAL_PRINT(F(" timeOutSave: "));
      SERIAL_PRINTLN(temp->stats.timeOutSave);*/

    // we check if last time is over time out
    if ((millis() - temp->stats.lastTimeSave) >= temp->stats.timeOutSave) {
      byte state = 0;
      int posState = (temp->id * EEPROM_SIZE_STATE) + (temp->id * EEPROM_SIZE_STATS);
      /*SERIAL_PRINT(F("statsSave() posState: "));
        SERIAL_PRINTLN(posState);

        SERIAL_PRINT(F("statsSave() EEPROM_ADRESS_STATS_STATE: "));
        SERIAL_PRINTLN(EEPROM_ADRESS_STATS_STATE);

        SERIAL_PRINT(F("statsSave() EEPROM_ADRESS_STATS: "));
        SERIAL_PRINTLN(EEPROM_ADRESS_STATS);*/

      // check EEPROM was already write else need set ok
      EEPROM.get(EEPROM_ADRESS_STATS_STATE + posState, state);
      if (state != EEPROM_STATE_OK) {
        SERIAL_PRINT(F("Stats "));
        SERIAL_PRINT(temp->title);
        SERIAL_PRINTLN(F(" not found in EEPROM, try save it"));
        state = EEPROM_STATE_OK;
        EEPROM.put(EEPROM_ADRESS_STATS_STATE + posState, state);
      } else {
        SERIAL_PRINT(F("Stats "));
        SERIAL_PRINT(temp->title);
        SERIAL_PRINTLN(F(" found in EEPROM, try overwrite it"));
      }
      /*SERIAL_PRINT(F("statsSave() state: "));
        SERIAL_PRINTLN(state);*/

      myTempStatsSave tempStatsSave;
      statsSaveToSave(&tempStatsSave, &temp->stats);

      //adress are pos of stats and add size of state + stats if id greater 0
      EEPROM.put(EEPROM_ADRESS_STATS + posState, tempStatsSave);

      temp->stats.needSave = false;
      temp->stats.lastTimeSave = millis(); // we keep time last save for check time out
    }
  }
}
