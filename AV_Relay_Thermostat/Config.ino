//__________________________________________________________________________________
// Copyright (c) 2021, Alexandre <contact@avdeveloppement.eu>.  All rights reserved.
// Mozilla Public License Version 2.0
// See the accompanying LICENSE file for terms.
//__________________________________________________________________________________

//#define SERIAL_OUTPUT // for get informations in serial

#define DISABLE_ONBOARD_LED // if board have led and want set led off

// if not def default are ENGLISH
#define languageFrench

// OLED screen on
// A4 SDA
// A5 SCK

// which analog pin button connected
#define BUTTON_PIN A6 // A6

// wich dgital pin relay are connected
#define RELAY_1_PIN 5 // D5
#define RELAY_2_PIN 6 // D6

// which analog pin termistor connected
#define THERMISTOR_1_PIN A2 // A2
#define THERMISTOR_2_PIN A3 // A3

#define THERMISTOR_1 0
#define THERMISTOR_2 1

// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000 // 10k

// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25

// how many samples to take and average, more takes longer but is more 'smooth'
#define NUMSAMPLES 10

// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950

// the value of the 'other' resistor
#define SERIESRESISTOR 10000 // 10k

//for led usage
#define LED_PIN_ON  HIGH
#define LED_PIN_OFF LOW

//for relay usage
#define RELAY_PIN_ON  HIGH
#define RELAY_PIN_OFF LOW

#define RELAY_OFF 0
#define RELAY_ON  1

#define RELAY_1 0
#define RELAY_2 1

// number of temperature
#define TEMPERATURE_NUMBER 2

// EEPROM
#define EEPROM_SIZE_STATE            sizeof(byte)
#define EEPROM_SIZE_SETTINGS         sizeof(myTempSettingsSave)
#define EEPROM_SIZE_STATS            sizeof(myTempStatsSave)

#define EEPROM_STATE_OK              161 // byte (2-250) must be greater that 2 because for reset we write EEPROM_STATE_OK - 1

#define EEPROM_ADRESS_SETTINGS_STATE 0
#define EEPROM_ADRESS_SETTINGS       EEPROM_ADRESS_SETTINGS_STATE + EEPROM_SIZE_STATE

#define EEPROM_ADRESS_STATS_STATE    EEPROM_ADRESS_SETTINGS + (EEPROM_SIZE_SETTINGS * 2)
#define EEPROM_ADRESS_STATS          EEPROM_ADRESS_STATS_STATE + EEPROM_SIZE_STATE

//value with factor 0 are at max around -20.0 (see tempMinMaxFail) so 5 * 6px = 30px
//factor 1 * by 2 = 60px
#define TEXT_SIZE_PIXELS_MAX 30
#define TEXT_SIZE_PIXELS_ONE_CARACT 6

// calculation mode, hot, cold
#define CALCULATION_MODE_HEATING 0
#define CALCULATION_MODE_COOLING 1

//serial speed
#define BAUD 115200

#define INITIALIZATION_DELAY 1000 // MILLISECONDS - time to wait before end of initialization

#define LOOP_DELAY 200 //time to wait beetwen each loop

// if temp sensor not beetween this value something are wrong set in fault stop relay
#define TEMP_FAIL_MIN 0
#define TEMP_FAIL_MAX 1
const float tempMinMaxFail[2] = { -25.0, 80.0};

//for save/load
typedef struct myTempStatsSave
{
  float lowest;// low temperature Termistor given
  float maximum;// max temperature Termistor given
} myTempStatsSave;

typedef struct myTempStats
{
  float lowest;// low temperature Termistor given
  float maximum;// max temperature Termistor given
  unsigned long timeOutSave; // time before save stats auto
  unsigned long lastTimeSave; // we store time when we save stats last time
  bool needSave; // when low max are changed it's set true, so know we can save stats
  bool needRedraw; // when low max are changed it's set true, so know we can redraw stats
} myTempStats;

//for save/load
typedef struct myTempSettingsSave
{
  float target;// temperature below/under relay stop
  float delta;// delta to add/remove to target for start relay again
  float correction;// to add/remove to actual for get correct temperature
  byte calculationMode;//for know when set relay on, actual temp below/above target
} myTempSettingsSave;

typedef struct myTempSettings
{
  float target;// temperature below/under relay stop
  float delta;// delta to add/remove to target for start relay again
  float correction;// to add/remove to actual for get correct temperature
  byte calculationMode;//for know when set relay on, actual temp below/above target
  bool needRedraw; // when low max are changed it's set true, so know we can redraw stats
} myTempSettings;

#define DRAW_X1 0
#define DRAW_Y1 1
#define DRAW_X2 2
#define DRAW_Y2 3
typedef struct myTemperature
{
  byte id;
  char title[6];
  float actual;// actual temperature Termistor given
  myTempStats stats;
  myTempSettings settings;
  byte relayState;
  int relayPIN;
  int thermistorPIN;
  const int relayOnHLine[4];// x pos, y pos, x2 pos, y2 pos for draw rect at top of temp, showed when relay on
  const int actualPosXY[2]; // x, y position of temperature actual
  const int targetPosXY[2];// x, y position of temperature target
  const int lowPosXY[2];// x, y position of temperature min
  const int maxPosXY[2];// x, y position of temperature max
  bool needRedraw;//when actual value changed, relay on/off, so know we can redraw actual
} myTemperature;

myTemperature temperature[TEMPERATURE_NUMBER] = {
  {
    0,//id
    "#1", //title
    (float)tempMinMaxFail[TEMP_FAIL_MIN] - 1, // actual temperature Termistor given, set out of range min - max needed at start
    { (float)tempMinMaxFail[TEMP_FAIL_MAX], // low temperature Termistor given
      (float)tempMinMaxFail[TEMP_FAIL_MIN],
      28800000,//timeout, save stats every 8h, save EEPROM cycle
      0,//last time save
      false,//if stats changed
      true//need redraw
    }, // max temperature Termistor given
    { (float)24.0,// temperature below/under relay stop
      (float)3.0,// delta to add/remove to target for start relay again
      (float)0.0, // to add/remove to actual for get correct temperature
      CALCULATION_MODE_HEATING,
      true//need redraw
    },
    RELAY_OFF,
    RELAY_1_PIN,
    THERMISTOR_1_PIN,
    {12, 20, 40, 22},// x pos, y pos, x2 pos for horizontal line at top of temp, showed when relay on
    {2, 24},// x, y position of temperature actual
    {2, 48},// x, y position of temperature target
    {30, 8},// x, y position of temperature min
    {30, 0},// x, y position of temperature max
    false
  }, {
    1,//id
    "#2", //title
    (float)tempMinMaxFail[TEMP_FAIL_MIN] - 1, // actual temperature Termistor given, set out of range min - max needed at start
    { (float)tempMinMaxFail[TEMP_FAIL_MAX], // low temperature Termistor given
      (float)tempMinMaxFail[TEMP_FAIL_MIN],
      28800000,//timeout, save stats every 8h, save EEPROM cycle
      0,//last time save
      false,//if stats changed
      true//need redraw
    }, // max temperature Termistor given
    { (float)24.0,// temperature below/under relay stop
      (float)3.0,// delta to add/remove to target for start relay again
      (float)0.0,// to add/remove to actual for get correct temperature
      CALCULATION_MODE_HEATING,
      true//need redraw
    },
    RELAY_OFF,
    RELAY_2_PIN,
    THERMISTOR_2_PIN,
    {75, 20, 105, 22},// x pos, y pos, x2 pos for horizontal line at top of temp, showed when relay on
    {65, 24},// x, y position of temperature actual
    {65, 48},// x, y position of temperature target
    {80, 8},// x, y position of temperature min
    {80, 0},// x, y position of temperature max
    false
  }
};

DisplaySH1106_128x64_I2C display(-1); // This line is suitable for most platforms by default
//DisplaySSD1306_128x64_SPI display(-1);

NanoFont nfUtils(ssd1306xled_font6x8);

static Key button                   = Key::K_NONE;
static Key newButton                = Key::K_NONE;
static ActualMenu actualMenuShow    = ActualMenu::AM_DATA_TEMPERATURE;
static ActualMenu newActualMenuShow = ActualMenu::AM_SAME;

const unsigned long timeOutButtionSelect PROGMEM = 4000; // time before show settings main menu in ms default 4seconds
unsigned long startTimeButtonPressed = 0; // we store time when user begin to press button select

const unsigned long timeOutGetOutSettings PROGMEM = 30000; // time after last press get out settings and show data temperature menu in ms default 15seconds
unsigned long startTimeLastButtonPressed = 0; // we store time when user last press a button

const unsigned long timeOutCheckAndRefresh PROGMEM = 1500; // time before update sonde, relay and screen data temp default 1seconds
unsigned long lastCheckAndRefresh = 0; // we store time when last check
bool canCheckAndRefresh = false; // at start of loop we check timeout and set this var so in loop we know if we can process some function

// pos of temperature on screen for change it
const int actualTempChangeX = 35;
const int actualTempChangeY = 24;

//for know if we apply settings on both temp or only one
SettingsTemperature actualSettingsTemperature = SettingsTemperature::ST_BOTH;

//when in menu for change target, delta, correction we change this var so on select we can get new value from here
float actualTempChange = 0;

// when change menu set to true and if true not change menu on next loop, user must release button for this go to false again
bool menuChanged = false;

/*const int canvasWidth = 128; // Width must be power of 2, i.e. 16, 32, 64, 128...
  const int canvasHeight = 16; // Height must be divided on 8, i.e. 8, 16, 24, 32...
  uint8_t canvasData[canvasWidth * (canvasHeight / 8)];
  //Create canvas object
  NanoCanvas1 canvas(canvasWidth, canvasHeight, canvasData);*/

#ifdef SERIAL_OUTPUT
#define SERIAL_PRINT(Str)   Serial.print(Str)
#define SERIAL_PRINTLN(Str) Serial.println(Str)
#else
#define SERIAL_PRINT(Str)
#define SERIAL_PRINTLN(Str)
#endif

int configFreeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

#ifdef SERIAL_OUTPUT
/**
   for get output in monitor/screen formated
   configPrintLnFormat("test %s", "ok");
*/
char outstr1[6] = "-----";
char outstr2[6] = "-----";
char outstr3[6] = "-----";
char logStr[255];
void configPrintLnFormat(const __FlashStringHelper* str, ... ) {
  va_list args;
  va_start(args, str);
#ifdef __AVR__
  vsnprintf_P(logStr, sizeof(logStr), reinterpret_cast<char*>(const_cast<__FlashStringHelper*>(str)), args); //progmem for AVR
#else
  vsnprintf(logStr, sizeof(logStr), reinterpret_cast<char*>(const_cast<__FlashStringHelper*>(str)), args); //for the rest of the world
#endif
  va_end(args);
  Serial.println(logStr);
}
#endif

void configSoftReboot() {
  // wait 3sec
  int restart = 0;
  while (restart < 300) {
    restart++;
    delay(10);
  }

  // soft restart
  SERIAL_PRINTLN(F("Restart..."));
  wdt_enable(WDTO_15MS);

  while (1) {}
}

void configResetApp() {
  byte state = 0;

  // reset settings
  // check EEPROM was already write and ok else no need reset
  EEPROM.get(EEPROM_ADRESS_SETTINGS_STATE, state);
  if (state == EEPROM_STATE_OK) {
    state = EEPROM_STATE_OK - 1;
    EEPROM.put(EEPROM_ADRESS_SETTINGS_STATE, state);
    SERIAL_PRINTLN(F("Reset settings"));
  }

  // reset stats 0
  EEPROM.get(EEPROM_ADRESS_STATS_STATE, state);
  if (state == EEPROM_STATE_OK) {
    state = EEPROM_STATE_OK - 1;
    EEPROM.put(EEPROM_ADRESS_STATS_STATE, state);
    SERIAL_PRINTLN(F("Reset stats 1"));
  }

  // reset stats 1
  EEPROM.get(EEPROM_ADRESS_STATS_STATE + EEPROM_SIZE_STATE + EEPROM_SIZE_STATS, state);
  if (state == EEPROM_STATE_OK) {
    state = EEPROM_STATE_OK - 1;
    EEPROM.put(EEPROM_ADRESS_STATS_STATE + EEPROM_SIZE_STATE + EEPROM_SIZE_STATS, state);
    SERIAL_PRINTLN(F("Reset stats 2"));
  }

  configSoftReboot();
}

void configLoopDelay() {
  lcd_delay(LOOP_DELAY);
}

void configInit() {
#ifdef SERIAL_OUTPUT
  Serial.begin(BAUD);
  Serial.setTimeout(10000);
  Serial.flush();
  while (!Serial) // Wait for the serial connection to be establised.
    delay(50);
#endif

  SERIAL_PRINTLN("");
  SERIAL_PRINTLN(F("Start initialization"));

#ifdef DISABLE_ONBOARD_LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_PIN_OFF); //we turn off on board led
  SERIAL_PRINTLN(F("Turned off onboard led"));
#endif

  SERIAL_PRINTLN(F("Termistor"));
  analogReference(EXTERNAL); // sonde use 3.3v

  SERIAL_PRINTLN(F("Relay"));
  pinMode(RELAY_1_PIN, OUTPUT);
  digitalWrite(RELAY_1_PIN, RELAY_PIN_OFF);
  pinMode(RELAY_2_PIN, OUTPUT);
  digitalWrite(RELAY_2_PIN, RELAY_PIN_OFF);

  SERIAL_PRINTLN(F("Display"));
  display.begin();
  display.clear();
  display.fill(0x00);
  display.setFixedFont(ssd1306xled_font6x8);

  SERIAL_PRINTLN(F("Configuration"));
  settingsLoad();

  SERIAL_PRINTLN(F("Statistiques"));
  statsLoad();

  SERIAL_PRINTLN(F("Menu"));
  button = getPressedButton(BUTTON_PIN);

  delay(INITIALIZATION_DELAY);
  SERIAL_PRINTLN(F("End initialization"));
}
