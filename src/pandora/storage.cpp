#include "storage.h"
#include <CastorS.h>
#include <Preferences.h>

#include "HERMENEXExternal.h"
#include "../menuHandlr.h"
#include "../pages.h"

// ======================================================
// GLOBAL STORAGE
// ======================================================

Preferences prefs;
Storage storage;


// ======================================================
// BEGIN
// ======================================================

void Storage::begin()
{
    prefs.begin("iron", false);
}


// ======================================================
// SIROCCRO CALIBRATION
// ======================================================

void Storage::loadSiroccroCalibration(
    uint16_t &c0,
    uint16_t &c1,
    uint16_t &c2
)
{
    c0 = prefs.getUShort("sc_c0", 2348);
    c1 = prefs.getUShort("sc_c1", 3004);
    c2 = prefs.getUShort("sc_c2", 3400);
}


void Storage::saveSiroccroCalibration(
    uint16_t c0,
    uint16_t c1,
    uint16_t c2
)
{
    prefs.putUShort("sc_c0", c0);
    prefs.putUShort("sc_c1", c1);
    prefs.putUShort("sc_c2", c2);
}


// ======================================================
// SIROCCRO SETTINGS
// ======================================================

void Storage::loadSiroccroSettings(
    uint16_t &temp,
    uint8_t &fan
)
{
    temp = prefs.getUShort(
        "sc_temp",
        hermenexConfig.defaultHotAirTemp
    );

    fan = prefs.getUChar(
        "sc_fan",
        0
    );
}


void Storage::saveSiroccroSettings(
    uint16_t temp,
    uint8_t fan
)
{
    prefs.putUShort(
        "sc_temp",
        temp
    );

    prefs.putUChar(
        "sc_fan",
        fan
    );
}


// ======================================================
// SIROCCRO PID
// ======================================================

void Storage::loadSiroccroPID(
    float &kp,
    float &ki,
    float &kd
)
{
    kp = prefs.getFloat(
        "sc_kp",
        12.0f
    );

    ki = prefs.getFloat(
        "sc_ki",
        0.6f
    );

    kd = prefs.getFloat(
        "sc_kd",
        25.0f
    );
}


void Storage::saveSiroccroPID(
    float kp,
    float ki,
    float kd
)
{
    prefs.putFloat(
        "sc_kp",
        kp
    );

    prefs.putFloat(
        "sc_ki",
        ki
    );

    prefs.putFloat(
        "sc_kd",
        kd
    );
}


// ======================================================
// PATRI SETTINGS
// ======================================================

void Storage::loadSettings()
{
    targetTemp = prefs.getInt(
        "pt_temp",
        hermenexConfig.defaultTemp
    );

    boostTemp = prefs.getInt(
        "pt_boostT",
        hermenexConfig.defaultBoostTemp
    );

    boostTimeSec = prefs.getInt(
        "pt_boostS",
        hermenexConfig.defaultBoostTime
    );

    sleepTemp = prefs.getInt(
        "pt_sleepT",
        hermenexConfig.defaultSleepTemp
    );

    sleepTimeSec = prefs.getInt(
        "pt_sleepS",
        240
    ); 

    buzzerEnabled = prefs.getBool(
        "pt_buzzer",
        true
    );

    currentTipMode = prefs.getInt(
        "pt_tipMode",
        TIP_ITEM_AUTO
    );


    // Validasi mode tip
    if (
        currentTipMode < TIP_ITEM_T12 ||
        currentTipMode > TIP_ITEM_CUSTOM
    )
    {
        currentTipMode = TIP_ITEM_AUTO;
    }


    // ==================================================
    // LOAD TIP DATABASE
    // ==================================================

    for (int i = 0; i < TOTAL_SUPPORTED_TIPS; i++)
    {
        TipConfig &tip = tipDatabase[i];

        String base = String("pt_") + tip.name;

        tip.kp = prefs.getFloat(
            (base + "_kp").c_str(),
            tip.kp
        );

        tip.ki = prefs.getFloat(
            (base + "_ki").c_str(),
            tip.ki
        );

        tip.kd = prefs.getFloat(
            (base + "_kd").c_str(),
            tip.kd
        );

        tip.tempOffset = prefs.getInt(
            (base + "_tOff").c_str(),
            tip.tempOffset
        );

        tip.adcOffset = prefs.getInt(
            (base + "_aOff").c_str(),
            tip.adcOffset
        );
    }


    // ==================================================
    // LOAD CUSTOM TIP
    // ==================================================

    {
        String base =
            String("pt_") +
            customTipProfile.name;

        customTipProfile.kp = prefs.getFloat(
            (base + "_kp").c_str(),
            customTipProfile.kp
        );

        customTipProfile.ki = prefs.getFloat(
            (base + "_ki").c_str(),
            customTipProfile.ki
        );

        customTipProfile.kd = prefs.getFloat(
            (base + "_kd").c_str(),
            customTipProfile.kd
        );

        customTipProfile.tempOffset = prefs.getInt(
            (base + "_tOff").c_str(),
            customTipProfile.tempOffset
        );

        customTipProfile.adcOffset = prefs.getInt(
            (base + "_aOff").c_str(),
            customTipProfile.adcOffset
        );
    }


    // ==================================================
    // APPLY ACTIVE TIP
    // ==================================================

    switch (currentTipMode)
    {
        case TIP_ITEM_T12:
            applyTipProfile(&tipDatabase[0]);
            break;

        case TIP_ITEM_C210:
            applyTipProfile(&tipDatabase[1]);
            break;

        case TIP_ITEM_CUSTOM:
            applyTipProfile(&customTipProfile);
            break;

        case TIP_ITEM_AUTO:
        default:
            activeTip = nullptr;
            break;
    }
}


// ======================================================
// SAVE GENERAL SETTINGS
// ======================================================

void Storage::saveSettings()
{
    prefs.putInt(
        "pt_temp",
        targetTemp
    );
}


// ======================================================
// LOAD PID
// ======================================================

void Storage::loadPID()
{
    if (!activeTip)
        return;

    String base =
        String("pt_") +
        activeTip->name;

    kp = prefs.getFloat(
        (base + "_kp").c_str(),
        activeTip->kp
    );

    ki = prefs.getFloat(
        (base + "_ki").c_str(),
        activeTip->ki
    );

    kd = prefs.getFloat(
        (base + "_kd").c_str(),
        activeTip->kd
    );
}


// ======================================================
// SAVE PID
// ======================================================

void Storage::savePID()
{
    if (!activeTip)
        return;

    String base =
        String("pt_") +
        activeTip->name;

    prefs.putFloat(
        (base + "_kp").c_str(),
        kp
    );

    prefs.putFloat(
        (base + "_ki").c_str(),
        ki
    );

    prefs.putFloat(
        (base + "_kd").c_str(),
        kd
    );
}


// ======================================================
// SAVE ACTIVE PID
// ======================================================

void Storage::saveActivePID()
{
    if (!activeTip)
        return;

    String base =
        String("pt_") +
        activeTip->name;

    prefs.putFloat(
        (base + "_kp").c_str(),
        activeTip->kp
    );

    prefs.putFloat(
        (base + "_ki").c_str(),
        activeTip->ki
    );

    prefs.putFloat(
        (base + "_kd").c_str(),
        activeTip->kd
    );
}


// ======================================================
// BOOST
// ======================================================

void Storage::saveBoost()
{
    prefs.putInt(
        "pt_boostT",
        boostTemp
    );

    prefs.putInt(
        "pt_boostS",
        boostTimeSec
    );
}


// ======================================================
// SLEEP
// ======================================================

void Storage::saveSleep()
{
    prefs.putInt(
        "pt_sleepT",
        sleepTemp
    );

    prefs.putInt(
        "pt_sleepS",
        sleepTimeSec
    );   
}


// ======================================================
// CALIBRATION PATRI
// ======================================================

void Storage::saveCal()
{
    if (!activeTip)
        return;

    String base =
        String("pt_") +
        activeTip->name;

    activeTip->tempOffset =
        tempOffset;

    activeTip->adcOffset =
        adcOffset;

    prefs.putInt(
        (base + "_tOff").c_str(),
        tempOffset
    );

    prefs.putInt(
        (base + "_aOff").c_str(),
        adcOffset
    );
}


// ======================================================
// TIP
// ======================================================

void Storage::saveTip()
{
    prefs.putInt(
        "pt_tipMode",
        currentTipMode
    );
}


// ======================================================
// BUZZER
// ======================================================

void Storage::saveBuzzer()
{
    prefs.putBool(
        "pt_buzzer",
        buzzerEnabled
    );
}


// ======================================================
// FACTORY RESET
// ======================================================

void Storage::factoryReset()
{
    prefs.clear();
}


// ======================================================
// BARE WRAPPERS
// ======================================================
// Supaya kode lama tetap bisa:
//
// saveSettings();
// savePID();
// saveBoost();
// dll.
//
// tanpa harus:
//
// storage.saveSettings();

void loadSettings()
{
    storage.loadSettings();
}


void saveSettings()
{
    storage.saveSettings();
}


void loadPID()
{
    storage.loadPID();
}


void savePID()
{
    storage.savePID();
}


void saveActivePID()
{
    storage.saveActivePID();
}


void saveBoost()
{
    storage.saveBoost();
}


void saveSleep()
{
    storage.saveSleep();
}


void saveCal()
{
    storage.saveCal();
}


void saveTip()
{
    storage.saveTip();
}


void saveBuzzer()
{
    storage.saveBuzzer();
}
