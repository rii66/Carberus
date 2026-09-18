#include <Arduino.h>
#include <handler.h>
#include <WiFi.h>
#include <U8g2lib.h>

#include "uiDashboard.h"

#include "HERMENEXExternal.h"
#include "../pages.h"
#include "../menuHandlr.h"
#include <CastorS.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

// API Hot Air
extern uint16_t airGetTemp();
extern uint16_t airGetTargetTemp();
extern uint8_t  airGetPower();
extern uint8_t  airGetFan();
extern bool     airIsOn();
extern bool     airHasAC();
extern const char* airGetModeStr();

/* =========================================================
   DASHBOARD SOLDER
========================================================= */
static void drawSolderDashboard() {

  /* ===== CURRENT TEMP ===== */
  u8g2.setFont(u8g2_font_logisoso24_tf);

  if (tipError) {
    u8g2.drawStr(0, 42, "EROR");
  } else {
    char tempBuf[12];
    sprintf(tempBuf, "%d\xB0", currentTemp);
    u8g2.drawStr(0, 42, tempBuf);
  }

  /* ===== TOP BAR ===== */
  u8g2.setFont(u8g2_font_6x10_tf);

  // Tip status
  char tipBuf[16];
  if (currentTipMode == TIP_ITEM_AUTO) {
    if (activeTip) sprintf(tipBuf, "A>%s", activeTip->name);
    else           sprintf(tipBuf, "AUTO");
  } else {
    if (activeTip) sprintf(tipBuf, "%s", activeTip->name);
    else           sprintf(tipBuf, "NoTip");
  }
  u8g2.drawStr(0, 10, tipBuf);

  // Station badge
  u8g2.drawStr(48, 10, "SLD");

  // WiFi
  u8g2.drawStr(100, 10, WiFi.status() == WL_CONNECTED ? "WIFI" : "AP");

  /* ===== RIGHT PANEL ===== */
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(78, 18, "SET");

  u8g2.setFont(u8g2_font_logisoso18_tf);
  if (tipError) {
    u8g2.drawStr(66, 36, "EROR");
  } else if (sleeping) {
    u8g2.drawStr(66, 36, "SLEEP");
  } else {
    char setBuf[12];
    sprintf(setBuf, "%d\xB0", targetTemp);
    u8g2.drawStr(74, 36, setBuf);
  }

  /* ===== PWM ===== */
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(78, 46, "PWM");

  int pwmPercent = map(pwmOut, 0, 255, 0, 100);
  int barWidth   = map(pwmOut, 0, 255, 0, 48);

  u8g2.drawFrame(74, 50, 48, 8);
  u8g2.drawBox(74, 50, barWidth, 8);

  char pwmBuf[8];
  sprintf(pwmBuf, "%d%%", pwmPercent);
  u8g2.drawStr(86, 64, pwmBuf);

  /* ===== MODE BADGE ===== */
  u8g2.drawFrame(0, 54, 52, 10);

  if (boostMode)
    u8g2.drawStr(4, 62, "BOOST");
  else if (sleeping)
    u8g2.drawStr(4, 62, "TiduR");
  else if (manualPWMMode)
    u8g2.drawStr(4, 62, "PWM");
  else
    u8g2.drawStr(4, 62, "StatuS");
}

/* =========================================================
   DASHBOARD HOT AIR  
========================================================= */
static void drawHotAirDashboard() {

    char buf[16];

    // =====================================================
    // TOP STATUS BAR
    // =====================================================

    u8g2.setFont(u8g2_font_6x10_tf);

    u8g2.drawStr(0, 9, "HOTAIR");

    u8g2.drawStr(
        48, 9,
        airHasAC() ? "AC:OK" : "AC:--"
    );

    u8g2.drawStr(
        100, 9,
        WiFi.status() == WL_CONNECTED ? "WIFI" : "AP"
    );


    // =====================================================
    // CURRENT TEMP — BESAR
    // =====================================================

    u8g2.setFont(u8g2_font_logisoso24_tf);

    sprintf(buf, "%d\xB0", airGetTemp());

    u8g2.drawStr(2, 37, buf);


    // =====================================================
    // TARGET TEMP — SEDANG
    // =====================================================

    u8g2.setFont(u8g2_font_6x10_tf);

    u8g2.drawStr(86, 19, "SET");

    u8g2.setFont(u8g2_font_logisoso18_tf);

    sprintf(buf, "%d\xB0", airGetTargetTemp());

    u8g2.drawStr(82, 36, buf);


    // =====================================================
    // BADGE POWER
    // =====================================================

    u8g2.setFont(u8g2_font_6x10_tf);

    u8g2.drawFrame(0, 51, 58, 13);

    if (airIsOn()) {

        u8g2.drawBox(0, 51, 58, 13);

        u8g2.setDrawColor(0);

        sprintf(buf, "ON %dW", airGetPower());

        u8g2.drawStr(4, 61, buf);

        u8g2.setDrawColor(1);

    } else {

        const char* m = airGetModeStr();

        if (strcmp(m, "COOLING") == 0)
            u8g2.drawStr(4, 61, "COOL");
        else
            u8g2.drawStr(8, 61, "OFF");
    }


    // =====================================================
    // FAN
    // =====================================================

    u8g2.drawStr(82, 46, "FAN");

    uint8_t fan = airGetFan();

    sprintf(buf, "%d", fan);
    u8g2.drawStr(108, 46, buf);


    // =====================================================
    // FAN BAR
    // =====================================================

    int fanBar = map(
        fan,
        0,
        255,
        0,
        48
    );

    u8g2.drawFrame(78, 51, 48, 10);

    if (fanBar > 0)
        u8g2.drawBox(
            78,
            51,
            fanBar,
            10
        );
}

/* =========================================================
   ENTRY
========================================================= */
void drawDashboard() {
  if (activeStation == 1)
    drawHotAirDashboard();
  else
    drawSolderDashboard();
}
