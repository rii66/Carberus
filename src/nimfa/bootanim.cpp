#include "bootanim.h"
#include "../buzzer.h"

#include <U8g2lib.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

void playBootAnimation() {
  
  // ==========================================
  // PHASE 1: NO SIGNAL
  // ==========================================
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  
  // Efek kedip 2 kali biar keren
  for(int i=0; i<2; i++) {
    u8g2.clearBuffer();
    u8g2.drawStr(30, 36, "NO SIGNAL...");
    u8g2.sendBuffer();
    delay(400);
    
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    delay(300);
  }

  // ==========================================
  // PHASE 2: DINO RUN (Scrolling Ground & Jump)
  // ==========================================
  int dinoX = 20;             // Posisi X Dino
  int groundY = 40;           // Posisi Y Tanah
  int dinoY = groundY;        // Posisi Y Dino (Bisa berubah saat lompat)
  
  bool isJumping = false;
  int jumpSpeed = 0;
  int groundOffset = 0;       // Untuk ilusi tanah bergerak

  // Kaktus bergerak dari ujung kanan ke ujung kiri
  for (int cactusX = 128; cactusX > -16; cactusX -= 4) {
    u8g2.clearBuffer();
    
    // --- 1. Gambar Ground Scrolling ---
    // Ilusi tanah bergerak dengan menggeser garis putus-putus
    groundOffset = (groundOffset + 2) % 8; 
    for(int i = 0; i < 136; i += 8) {
      u8g2.drawLine(i - groundOffset, groundY + 16, i - groundOffset + 4, groundY + 16);
    }

    // --- 2. Logika Lompat ---
    // Jika kaktus mendekat (jarak 30 pixel) & dino belum lompat, maka LOMPAT!
    if (cactusX > dinoX && cactusX < dinoX + 30 && !isJumping) {
       isJumping = true;
       jumpSpeed = 8; // Kekuatan lompat awal
       
       beepJump(); // buzzer event 
       
    }
    
    // Hitungan fisika sederhana untuk lompatan
    if (isJumping) {
       dinoY -= jumpSpeed;    // Dino naik
       jumpSpeed -= 2;        // Gravitasi menarik ke bawah
       
       // Jika sudah mendarat di tanah lagi
       if (dinoY >= groundY) {
         dinoY = groundY;
         isJumping = false;
       }
    }

    // --- 3. Gambar Sprite ---
    // Kaktus
    u8g2.drawXBMP(cactusX, groundY, 16, 16, cactus);

    // Dino (Kalau di udara kaki diam, kalau di tanah lari 2 frame)
    if (isJumping) {
       u8g2.drawXBMP(dinoX, dinoY, 16, 16, dino_run1); 
    } else {
       if ((cactusX / 4) % 2 == 0) { // Ganti frame kaki berdasarkan posisi kaktus
         u8g2.drawXBMP(dinoX, dinoY, 16, 16, dino_run1);
       } else {
         u8g2.drawXBMP(dinoX, dinoY, 16, 16, dino_run2);
       }
    }

    u8g2.sendBuffer();
    delay(19); // Kecepatan animasi
  }

  // ==========================================
  // PHASE 3: CONNECTED LOGO
  // ==========================================
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB10_tr); // Font agak lebih besar
  
  // Gambar Icon WiFi di tengah atas
  u8g2.drawXBMP(56, 16, 16, 16, wifi_icon);
  
  // Gambar teks
  u8g2.drawStr(22, 50, "CONNECTED");
  beepConnected();
  u8g2.sendBuffer();
  delay(600);
  
}
