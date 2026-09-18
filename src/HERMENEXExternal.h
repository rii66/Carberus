#ifndef HERMENEX_EXTERNAL_H
#define HERMENEX_EXTERNAL_H

#include <Arduino.h>
#include "tip.h"
#include "boost.h"

// ============================================================
// MAIN FIRMWARE CONFIGURATION BRIDGE
// ============================================================
// Nilai ini DIDEFINISIKAN di firmware utama (HERMENEXExternalBridge.cpp)
struct HERMENEXConfig {
    // Pins
    uint8_t  tempPin;
    uint8_t  pwmPin;
    uint8_t  hotAirAdc;
    uint8_t  fanPwm;
    uint8_t  zeroCross;
    uint8_t  heaterAc;
    uint8_t  oledSda;
    uint8_t  oledScl;
    uint8_t  encA;
    uint8_t  encB;
    uint8_t  encSw;
    uint8_t  buzzerPin;
    uint8_t  motionPin;

    // Temp limits / defaults
    int      tempMin;
    int      tempMaxHotAir;
    int      tempMaxT12;
    int      tempMaxC210;
    int      defaultTemp;
    int      defaultBoostTemp;
    int      defaultSleepTemp;
    int      defaultBoostTime;
    int      defaultHotAirTemp;

    // ADC detect
    uint16_t adcNoTip;
    uint16_t adcNoTipPtc;

    // Solder PWM
    uint32_t pwmFreq;
    uint8_t  pwmRes;
    uint16_t pwmMaxVal;

    // Hot air power / fan
    uint16_t powerPeriod;
    uint8_t  heaterMaxPower;
    uint32_t fanPwmFreq;
    uint8_t  fanPwmRes;
    uint8_t  fanMinSpeed;
    uint8_t  fanMaxPwm;

    // Network (Meadows)
    const char* otaHostname;
    const char* wifiSsid1;
    const char* wifiPass1;
    const char* wifiSsid2;
    const char* wifiPass2;
    const char* apSsid;
    const char* apPass;
};

extern HERMENEXConfig hermenexConfig;

// ============================================================
// SHARED GLOBAL STATE (defined in sketch GlobalState / boost)
// ============================================================
extern uint8_t heaterState;

extern int item;
extern int page;
extern bool inMenu;
extern bool inEdit;
extern bool menuClick;
extern bool booting;
extern uint8_t pidStage;
extern bool isEditingValue;

extern bool sleeping;
extern bool boostMode;
extern bool manualPWMMode;
extern bool buzzerEnabled;
extern bool webControl;
extern bool motionDetected;
extern bool btnHolding;

extern int maxPwmLimit;
extern bool tipError;
extern bool overHeat;

extern TipID currentTip;
extern uint8_t currentTipMode;

extern int boostTemp;
extern int boostTimeSec;
extern int sleepTemp;
extern int sleepTimeSec;
extern unsigned long sleepTimer;
extern int savedSleepTemp;

extern int tempOffset;
extern int adcOffset;
extern int adcRaw;

extern bool boostEditMode;
extern bool pidEditMode;
extern bool sleepEditMode;
extern bool buzzerEditMode;
extern bool calEditMode;

extern int lastEncoder;
extern int lastMenuPos;

extern unsigned long lastMotion;
extern unsigned long btnPressStart;
extern unsigned long lastActivity;

extern int maxTemp;
extern int targetTemp;
extern int currentTemp;
extern int pwmOut;

extern float kp;
extern float ki;
extern float kd;

// 0 = SOLDER, 1 = HOTAIR
extern int activeStation;

#define STATE_IDLE   0
#define STATE_HEAT   1
#define STATE_HOLD   2
#define STATE_SLEEP  3
#define STATE_BOOST  4
#define STATE_OVRHT  5
#define STATE_TIP    6

// ================= MEADOWS NETWORK API =================
// Deklarasi di sini supaya main/sketch selalu melihat simbol
// meskipun nested include meadows/* gagal di ArduinoDroid.
void initWiFi();
void setupOTA();
void initWebServer();
void initWebSocket();
void sendLiveData();

#endif
