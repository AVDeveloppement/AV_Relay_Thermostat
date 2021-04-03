//__________________________________________________________________________________
// Copyright (c) 2021, Alexandre <contact@avdeveloppement.eu>.  All rights reserved.
// Mozilla Public License Version 2.0
// See the accompanying LICENSE file for terms.
//__________________________________________________________________________________

void temperatureUpdate(myTemperature* temp) {
  uint8_t i;
  uint16_t samples[NUMSAMPLES];
  float newValue = 0;

  // take N samples in a row, with a slight delay
  for (i = 0; i < NUMSAMPLES; i++) {
    samples[i] = analogRead(temp->thermistorPIN);
    delay(25);
  }

  // average all the samples out
  for (i = 0; i < NUMSAMPLES; i++) newValue += samples[i];
  newValue /= NUMSAMPLES;

  // convert the value to resistance
  newValue = 1023 / newValue - 1;
  newValue = SERIESRESISTOR / newValue;

  // convert to degree celcius
  newValue = newValue / THERMISTORNOMINAL;     // (R/Ro)
  newValue = log(newValue);                  // ln(R/Ro)
  newValue /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  newValue += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  newValue = 1.0 / newValue;                 // Invert
  newValue -= 273.15;                         // convert to Celcius

  // apply correction
  newValue += temp->settings.correction;

  // get rounded with one digit after .
  newValue = round(newValue * 10) / 10.0;

  if (newValue != temp->actual) {
    temp->actual = newValue;
    temp->needRedraw = true;
  }

  /*SERIAL_PRINTLN("getSondeTemp");
    SERIAL_PRINTLN(temperature[0].stats.maximum);
    SERIAL_PRINTLN(temperature[0].stats.lowest);
    SERIAL_PRINTLN(temperature[0].maxPosXY[0]);
    SERIAL_PRINTLN(temperature[0].lowPosXY[0]);*/

  // prevent change stats if new value not in range min max
  // maybe disconnected?
  if (newValue < tempMinMaxFail[TEMP_FAIL_MIN] || newValue > tempMinMaxFail[TEMP_FAIL_MAX])
    return;

  if (temp->actual < temp->stats.lowest) {
    temp->stats.lowest = temp->actual;
    temp->stats.needSave = true;
    temp->stats.needRedraw = true;
#ifdef SERIAL_OUTPUT
    dtostrf(temp->stats.lowest, sizeof(outstr1), 1, outstr1);
    configPrintLnFormat(F("Lowest Temperature (%s) changed: %s°C"), temp->title, outstr1);
#endif
  }

  if (temp->actual > temp->stats.maximum) {
    temp->stats.maximum = temp->actual;
    temp->stats.needSave = true;
    temp->stats.needRedraw = true;
#ifdef SERIAL_OUTPUT
    dtostrf(temp->stats.maximum, sizeof(outstr1), 1, outstr1);
    configPrintLnFormat(F("Maximum Temperature (%s) changed: %s°C"), temp->title, outstr1);
#endif
  }

  /*SERIAL_PRINT("getSondeTemp exit, title: ");
    SERIAL_PRINT(temp->title);
    SERIAL_PRINT(", stats max: ");
    SERIAL_PRINT(temp->stats.maximum);
    SERIAL_PRINT(", stats min: ");
    SERIAL_PRINT(temp->stats.lowest);
    SERIAL_PRINT(", actual: ");
    SERIAL_PRINTLN(temp->actual);*/

  statsSave(temp);
}

void temperatureCheck() {
  //if need check else exit
  if (!canCheckAndRefresh) return;

  temperatureUpdate(&temperature[0]);
  temperatureUpdate(&temperature[1]);
}
