#pragma once

#include <WiFi.h>
#include <WiFiMulti.h>
#include <ESPmDNS.h>
#include "HERMENEXExternal.h"

inline void initWiFi() {

    WiFiMulti wifiMulti;

    wifiMulti.addAP(hermenexConfig.wifiSsid1, hermenexConfig.wifiPass1);
    wifiMulti.addAP(hermenexConfig.wifiSsid2, hermenexConfig.wifiPass2);

    WiFi.mode(WIFI_STA);
    // ESP32-C3: 8.5 dBm STA (hemat), 17 dBm AP
    WiFi.setTxPower(WIFI_POWER_8_5dBm);

    Serial.println(F("WiFi: searching..."));

    if (wifiMulti.run(10000) == WL_CONNECTED) {

        Serial.println(F("WIFI STA OK"));
        Serial.print(F("SSID: "));
        Serial.println(WiFi.SSID());

        Serial.print(F("IP: "));
        Serial.println(WiFi.localIP());

        if (MDNS.begin("dioscurios")) {
            MDNS.addService("http", "tcp", 80);
            Serial.println(F("mDNS: http://dioscurios.local"));
        }

    } else {

        Serial.println(F("STA FAILED"));

        WiFi.mode(WIFI_AP);
        WiFi.setTxPower(WIFI_POWER_17dBm);

        WiFi.softAP(hermenexConfig.apSsid, hermenexConfig.apPass);

        Serial.println(F("WIFI: AP"));
        Serial.print(F("IP: "));
        Serial.println(WiFi.softAPIP());
    }
}
