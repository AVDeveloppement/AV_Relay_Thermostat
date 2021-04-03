//__________________________________________________________________________________
// Copyright (c) 2021, Alexandre <contact@avdeveloppement.eu>.  All rights reserved.
// Mozilla Public License Version 2.0
// See the accompanying LICENSE file for terms.
//__________________________________________________________________________________

void relayVerifyState(myTemperature* temp)
{
  if (temp->actual < tempMinMaxFail[TEMP_FAIL_MIN] ||
      temp->actual > tempMinMaxFail[TEMP_FAIL_MAX]) {
    if (temp->relayState == RELAY_ON) {
      digitalWrite(temp->relayPIN, RELAY_PIN_OFF);
      temp->relayState = RELAY_OFF;
      temp->needRedraw = true;
#ifdef SERIAL_OUTPUT
      dtostrf(temp->actual, sizeof(outstr1), 1, outstr1);
      configPrintLnFormat(F("Bad temperature (%s) %s°C set relay off"), temp->title, outstr1);
#endif
    }
    return;
  }

  if (temp->settings.calculationMode == CALCULATION_MODE_HEATING) {
    if ((temp->actual + temp->settings.delta) < temp->settings.target &&
        temp->relayState == RELAY_OFF) {
      digitalWrite(temp->relayPIN, RELAY_PIN_ON);
      temp->relayState = RELAY_ON;
      temp->needRedraw = true;
#ifdef SERIAL_OUTPUT
      dtostrf(temp->actual, sizeof(outstr1), 1, outstr1);
      configPrintLnFormat(F("Temperature (%s) %s°C below target, set on"), temp->title, outstr1);
#endif
    } else if (temp->actual > temp->settings.target &&
               temp->relayState == RELAY_ON) {
      digitalWrite(temp->relayPIN, RELAY_PIN_OFF);
      temp->relayState = RELAY_OFF;
      temp->needRedraw = true;
#ifdef SERIAL_OUTPUT
      dtostrf(temp->actual, sizeof(outstr1), 1, outstr1);
      configPrintLnFormat(F("Temperature (%s) %s°C above target, set off"), temp->title, outstr1);
#endif
    }
  } else if (temp->settings.calculationMode == CALCULATION_MODE_COOLING) {
    if ((temp->actual + temp->settings.delta) > temp->settings.target &&
        temp->relayState == RELAY_OFF) {
      digitalWrite(temp->relayPIN, RELAY_PIN_ON);
      temp->relayState = RELAY_ON;
      temp->needRedraw = true;
#ifdef SERIAL_OUTPUT
      dtostrf(temp->actual, sizeof(outstr1), 1, outstr1);
      configPrintLnFormat(F("Temperature (%s) %s°C above target, set on"), temp->title, outstr1);
#endif
    } else if (temp->actual < temp->settings.target &&
               temp->relayState == RELAY_ON) {
      digitalWrite(temp->relayPIN, RELAY_PIN_OFF);
      temp->relayState = RELAY_OFF;
      temp->needRedraw = true;
#ifdef SERIAL_OUTPUT
      dtostrf(temp->actual, sizeof(outstr1), 1, outstr1);
      configPrintLnFormat(F("Temperature (%s) %s°C below target, set off"), temp->title, outstr1);
#endif
    }
  }
}

void relayCheck() {
  //if need check else exit
  if (!canCheckAndRefresh) return;

  relayVerifyState(&temperature[0]);
  relayVerifyState(&temperature[1]);
}
