#include "station.h"

#include "HERMENEXExternal.h"

#include <CastorS.h>

#include <handler.h>

void initStations() {
    // Solder (CastorS)
    initPWM();
    initPTC();

    // Hot Air (PolluxS)
    initAirHandler();
}

// NOTE: There is another monitorStations() defined as inline in monitoring.h
// (Serial debug print). Both kept intentionally — do not delete yet.
// The version below is the runtime update path.
void monitorStations() {
    // Kedua station boleh berjalan bersamaan
    updatePID();
    updateAirHandler();
}

// Placeholder (declared in station.h, not yet implemented)
void updateStations() {
    // Currently empty — dual update is handled by monitorStations() + explicit
    // calls in loop(). Kept for future expansion.
}

void handleStationEncoder(int delta) {
    if (delta == 0) return;

    switch (activeStation) {
        case 0: // Solder
            castorS.encoder(delta);
            break;

        case 1: // Hot Air
            handleAirEncoder(delta);
            break;
    }
}

void handleStationButton() {
    switch (activeStation) {
        case 0: // Solder — short click bisa diarahkan ke boost nanti
            break;

        case 1: // Hot Air
            handleAirButton();
            break;
    }
}
