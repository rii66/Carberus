#include <U8g2lib.h>
#include <Arduino.h>

#include "icons.h"
#include "../pages.h"
#include "../buzzer.h"
#include "HERMENEXExternal.h"
#include <CastorS.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

// Dari menuHandlr
extern int  getStationItem();
extern bool isStationMenu();

/* ====================================================
                       SET PAGE
==================================================== */
void drawSetPage(int x) {
  char buf[16];

  // ===== TITLE PAGE =====
  if (!inEdit) {
    u8g2.setFont(u8g2_font_fub30_tf);
    u8g2.drawStr(x + 28, 38, "SET");

    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(x + 30, 60, "Click Enter");
    return;
  }

  // ===== HEADER =====
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawLine(x, 12, x + 127, 12);

  // SAVE
  if (item == SET_SAVE) {
    u8g2.drawBox(x + 4, 2, 40, 8);
    u8g2.setDrawColor(0);
    u8g2.drawStr(x + 10, 9, "SAVE");
    u8g2.setDrawColor(1);
  } else {
    u8g2.drawStr(x + 10, 9, "SAVE");
  }

  // EXIT
  if (item == SET_EXIT) {
    u8g2.drawBox(x + 84, 2, 40, 8);
    u8g2.setDrawColor(0);
    u8g2.drawStr(x + 92, 9, "EXIT");
    u8g2.setDrawColor(1);
  } else {
    u8g2.drawStr(x + 92, 9, "EXIT");
  }

  // ===== CURRENT TEMP =====
  drawcurrentTemp(x);

  // ===== BODY: STATION + TEMP =====
  u8g2.setFont(u8g2_font_6x10_tf);

  // STATION
  if (item == SET_STATION) u8g2.drawStr(x + 4, 26, ">");
  u8g2.drawStr(x + 16, 26, "STATION");

  // Tampilkan station aktif
  if (activeStation == STATION_HOTAIR)
    u8g2.drawStr(x + 78, 26, "HOTAIR");
  else
    u8g2.drawStr(x + 78, 26, "SOLDER");

  // TEMP
  if (item == SET_TEMP) u8g2.drawStr(x + 4, 42, ">");
  u8g2.drawStr(x + 16, 42, "TEMP");

  u8g2.setFont(u8g2_font_logisoso16_tf);
  sprintf(buf, "%d", targetTemp);
  u8g2.drawStr(x + 70, 46, buf);

  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(x + 110, 42, "C");

  // ===== FOOTER =====
  u8g2.drawLine(x, 54, x + 127, 54);

  sprintf(buf, "MIN:%d", hermenexConfig.tempMin);
  u8g2.drawStr(x + 8, 63, buf);

  sprintf(buf, "MAX:%d", maxTemp);
  u8g2.drawStr(x + 74, 63, buf);
}

// STATUS TEMPERATUR MENU //
void drawcurrentTemp(int x)
{
  char buf[8];
  sprintf(buf, "%dC", currentTemp);

  u8g2.setFont(u8g2_font_6x10_tf);
  int w = u8g2.getStrWidth(buf);
  u8g2.drawStr(x + (128 - w) / 2, 9, buf);
}

/* ====================================================
                   STATION SUBMENU
==================================================== */
void drawStationPage(int x) {
  char buf[16];
  int si = getStationItem();

  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawLine(x, 12, x + 127, 12);

  // SAVE
  if (si == STATION_SAVE) {
    u8g2.drawBox(x + 4, 2, 40, 8);
    u8g2.setDrawColor(0);
    u8g2.drawStr(x + 10, 9, "SAVE");
    u8g2.setDrawColor(1);
  } else {
    u8g2.drawStr(x + 10, 9, "SAVE");
  }

  // EXIT
  if (si == STATION_EXIT) {
    u8g2.drawBox(x + 84, 2, 40, 8);
    u8g2.setDrawColor(0);
    u8g2.drawStr(x + 92, 9, "EXIT");
    u8g2.setDrawColor(1);
  } else {
    u8g2.drawStr(x + 92, 9, "EXIT");
  }

  // Title
  u8g2.setFont(u8g2_font_7x14_tf);
  u8g2.drawStr(x + 28, 28, "STATION");

  u8g2.setFont(u8g2_font_6x10_tf);

  // SOLDER
  if (si == STATION_SOLDER) {
    u8g2.drawBox(x + 10, 34, 50, 12);
    u8g2.setDrawColor(0);
    u8g2.drawStr(x + 16, 43, "SOLDER");
    u8g2.setDrawColor(1);
  } else {
    u8g2.drawStr(x + 16, 43, "SOLDER");
  }

  // HOTAIR
  if (si == STATION_HOTAIR) {
    u8g2.drawBox(x + 68, 34, 50, 12);
    u8g2.setDrawColor(0);
    u8g2.drawStr(x + 74, 43, "HOTAIR");
    u8g2.setDrawColor(1);
  } else {
    u8g2.drawStr(x + 74, 43, "HOTAIR");
  }

  // Footer: aktif
  u8g2.drawLine(x, 54, x + 127, 54);
  u8g2.drawStr(x + 8, 63, "NOW:");
  if (activeStation == STATION_HOTAIR)
    u8g2.drawStr(x + 40, 63, "HOTAIR");
  else
    u8g2.drawStr(x + 40, 63, "SOLDER");
}

/* =========================================================
                             BOOST PAGE
==================================================== */
void drawBoostPage(int x) {
  char buf[12];

  if (!inEdit) {
    u8g2.setFont(u8g2_font_fub30_tf);
    u8g2.drawStr(x + 8, 38, "BosT");

    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(x + 30, 60, "masuk");
    return;
  }

  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawLine(x, 12, x + 127, 12);

  drawcurrentTemp(x);

  if (item == BOOST_SAVE) {
    u8g2.drawBox(x + 4, 2, 40, 8);
    u8g2.setDrawColor(0);
    u8g2.drawStr(x + 10, 9, "SAVE");
    u8g2.setDrawColor(1);
  } else {
    u8g2.drawStr(x + 10, 9, "SAVE");
  }

  if (item == BOOST_EXIT) {
    u8g2.drawBox(x + 84, 2, 40, 8);
    u8g2.setDrawColor(0);
    u8g2.drawStr(x + 92, 9, "EXIT");
    u8g2.setDrawColor(1);
  } else {
    u8g2.drawStr(x + 92, 9, "EXIT");
  }

  if (item == BOOST_TEMP) u8g2.drawStr(x + 4, 30, ">");
  u8g2.drawStr(x + 16, 30, "TEMP");
  sprintf(buf, "%dC", boostTemp);
  u8g2.drawStr(x + 88, 30, buf);

  if (item == BOOST_TIME) u8g2.drawStr(x + 4, 44, ">");
  u8g2.drawStr(x + 16, 44, "TIME");
  sprintf(buf, "%ds", boostTimeSec);
  u8g2.drawStr(x + 88, 44, buf);

  u8g2.drawLine(x, 63, x + 127, 63);
}

/* ====================================================
                             SLEEP PAGE
==================================================== */
void drawSleepPage(int x) {
  char buf[12];

  if (!inEdit) {
    u8g2.setFont(u8g2_font_profont29_mr);
    u8g2.drawStr(x + 12, 36, "SLEEP");

    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(x + 30, 60, "masuk");
    return;
  }

  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawLine(x, 12, x + 127, 12);

  drawcurrentTemp(x);

  if (item == SLEEP_SAVE) {
    u8g2.drawBox(x + 4, 2, 40, 8);
    u8g2.setDrawColor(0);
    u8g2.drawStr(x + 10, 9, "SAVE");
    u8g2.setDrawColor(1);
  } else {
    u8g2.drawStr(x + 10, 9, "SAVE");
  }

  if (item == SLEEP_EXIT) {
    u8g2.drawBox(x + 84, 2, 40, 8);
    u8g2.setDrawColor(0);
    u8g2.drawStr(x + 92, 9, "EXIT");
    u8g2.setDrawColor(1);
  } else {
    u8g2.drawStr(x + 92, 9, "EXIT");
  }

  if (item == SLEEP_TEMP) u8g2.drawStr(x + 4, 30, ">");
  u8g2.drawStr(x + 16, 30, "TEMP");
  sprintf(buf, "%dC", sleepTemp);
  u8g2.drawStr(x + 88, 30, buf);

  if (item == SLEEP_TIME) u8g2.drawStr(x + 4, 44, ">");
  u8g2.drawStr(x + 16, 44, "TIME");
  sprintf(buf, "%ds", sleepTimeSec);
  u8g2.drawStr(x + 88, 44, buf);

  u8g2.drawLine(x, 63, x + 127, 63);
}

/* ====================================================
                             CAL PAGE
==================================================== */
void drawCalPage(int x) {
  char buf[12];

  if (!inEdit) {
    u8g2.setFont(u8g2_font_fub30_tf);
    u8g2.drawStr(x + 28, 40, "CAL");

    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(x + 30, 60, "Click Enter");
    return;
  }

  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawLine(x, 12, x + 127, 12);

  drawcurrentTemp(x);

  if (item == CAL_SAVE) {
    u8g2.drawBox(x + 4, 2, 40, 8);
    u8g2.setDrawColor(0);
    u8g2.drawStr(x + 10, 9, "SAVE");
    u8g2.setDrawColor(1);
  } else {
    u8g2.drawStr(x + 10, 9, "SAVE");
  }

  if (item == CAL_EXIT) {
    u8g2.drawBox(x + 84, 2, 40, 8);
    u8g2.setDrawColor(0);
    u8g2.drawStr(x + 92, 9, "EXIT");
    u8g2.setDrawColor(1);
  } else {
    u8g2.drawStr(x + 92, 9, "EXIT");
  }

  // SOLDER offset
  if (item == CAL_SOLDER) u8g2.drawStr(x + 4, 30, ">");
  u8g2.drawStr(x + 16, 30, "SOLDER");
  sprintf(buf, "%d", tempOffset);
  u8g2.drawStr(x + 88, 30, buf);

  // HOTAIR (placeholder offset 0 jika belum ada var)
  if (item == CAL_HOTAIR) u8g2.drawStr(x + 4, 44, ">");
  u8g2.drawStr(x + 16, 44, "HOTAIR");
  u8g2.drawStr(x + 88, 44, "---");

  u8g2.drawLine(x, 54, x + 127, 54);
  u8g2.drawStr(x + 20, 63, "OFFSET C");
}

/* ====================================================
                             PID PAGE
==================================================== */
void drawPIDPage(int x) {
  char buf[16];

  if (!inEdit) {
    u8g2.setFont(u8g2_font_fub30_tf);
    u8g2.drawStr(x + 28, 40, "PID");

    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(x + 34, 60, "Click Enter");
    return;
  }

  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawLine(x, 12, x + 127, 12);

  drawcurrentTemp(x);

  if (!isEditingValue) {

    if (item == PID_SAVE) {
      u8g2.drawBox(x + 4, 2, 40, 8);
      u8g2.setDrawColor(0);
      u8g2.drawStr(x + 10, 9, "SAVE");
      u8g2.setDrawColor(1);
    } else {
      u8g2.drawStr(x + 10, 9, "SAVE");
    }

    if (item == PID_EXIT) {
      u8g2.drawBox(x + 84, 2, 40, 8);
      u8g2.setDrawColor(0);
      u8g2.drawStr(x + 92, 9, "EXIT");
      u8g2.setDrawColor(1);
    } else {
      u8g2.drawStr(x + 92, 9, "EXIT");
    }

    if (item == PID_KI) u8g2.drawStr(x + 10, 28, ">");
    u8g2.drawStr(x + 22, 28, "KI ────");

    if (item == PID_KP) u8g2.drawStr(x + 10, 42, ">");
    u8g2.drawStr(x + 22, 42, "KP ────");

    if (item == PID_KD) u8g2.drawStr(x + 10, 56, ">");
    u8g2.drawStr(x + 22, 56, "KD ────");
  }
  else {

    float val = 0;
    const char* label = "";

    if (item == PID_KP) { val = kp; label = "KP"; }
    else if (item == PID_KI) { val = ki; label = "KI"; }
    else if (item == PID_KD) { val = kd; label = "KD"; }

    u8g2.drawStr(x + 10, 9, "SAVE");
    u8g2.drawStr(x + 92, 9, "EXIT");

    u8g2.setFont(u8g2_font_fub14_tf);
    u8g2.drawStr(x + 48, 28, label);

    u8g2.drawFrame(x + 18, 36, 92, 8);

    int slider = map((int)(val * 100), 0, 99900, 0, 88);
    u8g2.drawDisc(x + 20 + slider, 40, 3);

    u8g2.setFont(u8g2_font_6x12_tf);
    sprintf(buf, "%.2f", val);
    u8g2.drawStr(x + 42, 58, buf);
  }
}

/* ====================================================
                             BUZZER PAGE
==================================================== */
void drawBuzzerPage(int x) {
  if (!inEdit) {
    u8g2.setFont(u8g2_font_fub30_tf);
    u8g2.drawStr(x + 8, 32, "BuzeR");

    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(x + 30, 60, "Click Enter");
    return;
  }

  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawLine(x, 12, x + 127, 12);

  drawcurrentTemp(x);

  if (item == BUZ_SAVE) {
    u8g2.drawBox(x + 4, 2, 40, 8);
    u8g2.setDrawColor(0);
    u8g2.drawStr(x + 10, 9, "SAVE");
    u8g2.setDrawColor(1);
  } else {
    u8g2.drawStr(x + 10, 9, "SAVE");
  }

  if (item == BUZ_EXIT) {
    u8g2.drawBox(x + 84, 2, 40, 8);
    u8g2.setDrawColor(0);
    u8g2.drawStr(x + 92, 9, "EXIT");
    u8g2.setDrawColor(1);
  } else {
    u8g2.drawStr(x + 92, 9, "EXIT");
  }

  if (item == BUZ_ON) u8g2.drawStr(x + 10, 28, ">");
  u8g2.drawStr(x + 24, 28, "ON");

  if (item == BUZ_OFF) u8g2.drawStr(x + 10, 42, ">");
  u8g2.drawStr(x + 24, 42, "OFF");

  u8g2.drawLine(x, 54, x + 127, 54);

  if (buzzerEnabled)
    u8g2.drawStr(x + 36, 63, "STATE: ON");
  else
    u8g2.drawStr(x + 34, 63, "STATE: OFF");
}

/* ====================================================
                          TIP PAGE
==================================================== */
void drawTipPage(int x) {

  if (!inEdit) {
    u8g2.setFont(u8g2_font_fub30_tf);
    u8g2.drawStr(x + 24, 40, "TIP");

    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(x + 30, 60, "Click Enter");
    return;
  }

  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawLine(x, 12, x + 127, 12);

  drawcurrentTemp(x);

  if (item == TIP_ITEM_SAVE) {
    u8g2.drawBox(x + 4, 2, 40, 8);
    u8g2.setDrawColor(0);
    u8g2.drawStr(x + 10, 9, "SAVE");
    u8g2.setDrawColor(1);
  } else {
    u8g2.drawStr(x + 10, 9, "SAVE");
  }

  if (item == TIP_ITEM_EXIT) {
    u8g2.drawBox(x + 84, 2, 40, 8);
    u8g2.setDrawColor(0);
    u8g2.drawStr(x + 92, 9, "EXIT");
    u8g2.setDrawColor(1);
  } else {
    u8g2.drawStr(x + 92, 9, "EXIT");
  }

  if (item == TIP_ITEM_T12)
    u8g2.drawStr(x + 4, 30, ">");
  u8g2.drawStr(x + 16, 30, "T12");

  if (item == TIP_ITEM_C210)
    u8g2.drawStr(x + 52, 30, ">");
  u8g2.drawStr(x + 64, 30, "C210");

  if (item == TIP_ITEM_AUTO)
    u8g2.drawStr(x + 4, 44, ">");
  u8g2.drawStr(x + 16, 44, "AUTO");

  if (item == TIP_ITEM_CUSTOM)
    u8g2.drawStr(x + 52, 44, ">");
  u8g2.drawStr(x + 64, 44, "CUSTOM");

  u8g2.drawLine(x, 54, x + 127, 54);

  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(x + 2, 61, "MODE:");

  if (currentTipMode == TIP_ITEM_AUTO)
    u8g2.drawStr(x + 34, 61, "AUTO");
  else if (currentTipMode == TIP_ITEM_T12)
    u8g2.drawStr(x + 34, 61, "T12");
  else if (currentTipMode == TIP_ITEM_C210)
    u8g2.drawStr(x + 34, 61, "C210");
  else if (currentTipMode == TIP_ITEM_CUSTOM)
    u8g2.drawStr(x + 34, 61, "CUSTOM");
}
