//__________________________________________________________________________________
// Copyright (c) 2021, Alexandre <contact@avdeveloppement.eu>.  All rights reserved.
// Mozilla Public License Version 2.0
// See the accompanying LICENSE file for terms.
//__________________________________________________________________________________

#pragma once

#include "stdint.h"

// witch button user press
enum class Key : uint8_t {
  K_NONE   = 0,
  K_UP     = 1,
  K_DOWN   = 2,
  K_SELECT = 3,
};

//for know if we apply change on both temp or only one temp
enum class SettingsTemperature : uint8_t {
  ST_BOTH   = 0,
  ST_TEMP_1 = 1,
  ST_TEMP_2 = 2,
};

// for know menu item user press in main settings
enum class SelectionSettingsMain : uint8_t {
  SSM_RESET_STATS = 0,
  SSM_TEMP_1      = 1,
  SSM_TEMP_2      = 2,
  SSM_RESET_ALL   = 3,
  SSM_BACK        = 4,
};

// for know menu item user press in temperature settings
enum class SelectionSettingsTemp : uint8_t {
  SST_RESET_STATS      = 0,
  SST_TEMP_TARGET      = 1,
  SST_TEMP_DELTA       = 2,
  SST_TEMP_CORRECTION  = 3,
  SST_CALCULATION_MODE = 4,
  SST_BACK             = 5,
};

// for know menu item user press in calculation mode settings
enum class SelectionCalculationMode : uint8_t {
  SCM_HEATING = 0,
  SCM_COOLING = 1,
  SCM_BACK    = 2,
};

// for know menu item user press in reset settings
enum class SelectionReset : uint8_t {
  SR_CONFIRM = 0,
  SR_BACK    = 1,
};

// for know menu to show to user
enum class ActualMenu : uint8_t {
  AM_DATA_TEMPERATURE               = 0,
  AM_SETTINGS_MAIN                  = 1,
  AM_SETTINGS_TEMPERATURE           = 2,
  AM_SETTINGS_TEMP_TARGET           = 3,
  AM_SETTINGS_TEMP_DELTA            = 4,
  AM_SETTINGS_TEMP_CORRECTION       = 5,
  AM_SETTINGS_TEMP_CALCULATION_MODE = 6,
  AM_SETTINGS_RESET_STATS           = 7,
  AM_SETTINGS_RESET_ALL             = 8,
  AM_SAME                           = 9,
};

Key getPressedButton(uint8_t analogPin);
