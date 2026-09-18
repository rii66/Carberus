#include <Wire.h>
#include <U8g2lib.h>

#include "HERMENEXExternal.h"
#include "../pages.h"
#include "icons.h"
#include "uiDashboard.h"

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

// Dari menuHandlr
extern bool isStationMenu();

int animX = 0;
int targetX = 0;

void initDisplay() {
  Wire.begin(hermenexConfig.oledSda, hermenexConfig.oledScl);
  u8g2.begin();
}

void drawUI() {
  u8g2.clearBuffer();

  if (inMenu) {

    // Submenu Station overlay (di atas SET page)
    if (isStationMenu()) {
      drawStationPage(0);
    }
    else {
      targetX = page * 128;

      int diff = targetX - animX;

      if (abs(diff) > 1)
        animX += diff / 3;
      else
        animX = targetX;

      drawSetPage(0 - animX);
      drawBoostPage(128 - animX);
      drawSleepPage(256 - animX);
      drawCalPage(384 - animX);
      drawPIDPage(512 - animX);
      drawTipPage(640 - animX);
      drawBuzzerPage(768 - animX);
    }

  }
  else {
    drawDashboard();
  }

  u8g2.sendBuffer();
}
