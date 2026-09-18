#include "encoder.h"
#include "pages.h"
#include "HERMENEXExternal.h"
#include <CastorS.h>

volatile int encoderPos = hermenexConfig.defaultTemp; 
volatile bool encoderMoved = false;

volatile uint8_t lastState = 0;
// lastEncInterrupt sudah tidak dipakai lagi, jadi bisa dihapus.

/* ===== ISR ===== */
void IRAM_ATTR encoderISR() {
  // HAPUS logika micros() dan debounce di sini.
  
  uint8_t a = digitalRead(hermenexConfig.encA);
  uint8_t b = digitalRead(hermenexConfig.encB);

  uint8_t encoded = (a << 1) | b;
  uint8_t sum = (lastState << 2) | encoded;

  bool moved = false;

  // Logika pembacaan pola putar kanan
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderPos++;
    moved = true;
  }

  // Logika pembacaan pola putar kiri
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderPos--;
    moved = true;
  }

  if (moved) encoderMoved = true;

  // Wajib selalu update state meskipun terjadi bouncing
  lastState = encoded; 
}

/* ===== INIT ===== */
void initEncoder() {
  pinMode(hermenexConfig.encA, INPUT_PULLUP);
  pinMode(hermenexConfig.encB, INPUT_PULLUP);
  pinMode(hermenexConfig.encSw, INPUT_PULLUP);

  lastState = (digitalRead(hermenexConfig.encA) << 1) | digitalRead(hermenexConfig.encB);

  attachInterrupt(digitalPinToInterrupt(hermenexConfig.encA), encoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(hermenexConfig.encB), encoderISR, CHANGE);
}

/* ===== BUTTON ===== */
bool buttonPressed() {
  return !digitalRead(hermenexConfig.encSw);
}

/* ===== DELTA (FINAL FIX) ===== */
int getEncoderDelta() {
  // Bagi encoderPos dengan 4 supaya 1 klik fisik = 1 langkah logika
  // (Ubah angka 4 menjadi 2 jika menunya terasa butuh 2x klik baru pindah)
  int currentLogicPos = encoderPos / 4; 
  static int lastLogicPos = currentLogicPos;

  int diff = currentLogicPos - lastLogicPos;

  if (diff != 0) {
    lastLogicPos = currentLogicPos;
    return diff; // Akan selalu bernilai 1 atau -1 setiap 1 kali klik fisik
  }

  return 0;
}
