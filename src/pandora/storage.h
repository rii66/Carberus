#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <Preferences.h>


class Storage {
public:
    void begin();

    // =========================
    // SIROCCRO / HOT AIR
    // =========================
    void loadSiroccroCalibration(
        uint16_t &c0,
        uint16_t &c1,
        uint16_t &c2
    );

    void saveSiroccroCalibration(
        uint16_t c0,
        uint16_t c1,
        uint16_t c2
    );

    void loadSiroccroSettings(
        uint16_t &temp,
        uint8_t &fan
    );

    void saveSiroccroSettings(
        uint16_t temp,
        uint8_t fan
    );

    void loadSiroccroPID(
        float &kp,
        float &ki,
        float &kd
    );

    void saveSiroccroPID(
        float kp,
        float ki,
        float kd
    );

    // =========================
    // PATRI / SOLDER
    // =========================
    void loadSettings();
    void saveSettings();

    void loadPID();
    void savePID();
    void saveActivePID();

    void saveBoost();
    void saveSleep();
    void saveCal();
    void saveTip();
    void saveBuzzer();

    void factoryReset();
};

// Global storage object
extern Storage storage;


// =========================
// BARE STORAGE API
// =========================
// Dipakai oleh menu / websocket
void loadSettings();
void saveSettings();

void loadPID();
void savePID();
void saveActivePID();

void saveBoost();
void saveSleep();
void saveCal();
void saveTip();
void saveBuzzer();

#endif
