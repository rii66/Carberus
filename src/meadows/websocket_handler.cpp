#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

#include "HERMENEXExternal.h"

#include <CastorS.h>          // boost, pid, tip, pwm, activeTip, tipDatabase
#include <handler.h>          // air API (PolluxS)

#include "../encoder.h"
#include "../pandora/storage.h"

#include "websocket_handler.h"
#include "webserver.h"

// Eksplisit — jaga-jaga ArduinoDroid tidak menarik extern dari webserver.h
extern WebServer server;
extern WebSocketsServer webSocket;

void onWebSocketEvent(
    uint8_t num,
    WStype_t type,
    uint8_t *payload,
    size_t length)
{
    if (type != WStype_TEXT)
        return;

    String msg = String((char *)payload);

    Serial.println("RX: " + msg);

    /* ================= SET TEMP (Solder) ================= */

    if (msg.startsWith("SET:")) {

        targetTemp = constrain(
            msg.substring(4).toInt(),
            hermenexConfig.tempMin,
            maxTemp);

        webControl = true;
        manualPWMMode = false;

        saveSettings();
    }

    /* ================= MANUAL PWM (Solder) ================= */

    else if (msg.startsWith("PWM:")) {

        manualPWMMode = true;

        int pwmPercent = constrain(
            msg.substring(4).toInt(),
            0,
            100);

        int pwm = map(
            pwmPercent,
            0,
            100,
            0,
            maxPwmLimit);

        setPWM(pwm);
    }

    /* ================= BOOST ================= */

    else if (msg == "BOOST") {

        startBoost();
    }

    /* ================= MAX TEMP (Solder) ================= */

    else if (msg.startsWith("MAXTEMP:")) {

        int hwLimit = activeTip ? activeTip->maxTemp : hermenexConfig.tempMaxT12;

        maxTemp = constrain(
            msg.substring(8).toInt(),
            hermenexConfig.tempMin,
            hwLimit);

        if (targetTemp > maxTemp)
            targetTemp = maxTemp;

        saveSettings();
    }

    /* ================= SLEEP ================= */

    else if (msg.startsWith("SLEEPTIME:")) {

        sleepTimeSec = msg.substring(10).toInt();

        saveSleep();
    }

    /* ================= PID ================= */

    else if (msg.startsWith("PID:")) {

        if (!activeTip)
            return;

        String data = msg.substring(4);

        int p1 = data.indexOf(',');
        int p2 = data.lastIndexOf(',');

        float newKp = data.substring(0, p1).toFloat();
        float newKi = data.substring(p1 + 1, p2).toFloat();
        float newKd = data.substring(p2 + 1).toFloat();

        kp = newKp;
        ki = newKi;
        kd = newKd;

        activeTip->kp = newKp;
        activeTip->ki = newKi;
        activeTip->kd = newKd;

        saveActivePID();
    }

    /* ================= TIP ================= */

    else if (msg.startsWith("TIP:")) {

        String requested = msg.substring(4);

        for (int i = 0; i < TOTAL_SUPPORTED_TIPS; i++) {

            if (requested.equalsIgnoreCase(tipDatabase[i].name)) {

                applyTipProfile(&tipDatabase[i]);

                saveTip();

                break;
            }
        }
    }

    /* ================= HOT AIR : SET TEMP ================= */

    else if (msg.startsWith("HASET:")) {

        uint16_t t = constrain(
            msg.substring(6).toInt(),
            hermenexConfig.tempMin,
            hermenexConfig.tempMaxHotAir);

        airSetTemp(t);
        airSaveSettings();
    }

    /* ================= HOT AIR : FAN / AIR FLOW ================= */

    else if (msg.startsWith("AIR:")) {

        uint8_t fan = constrain(
            msg.substring(4).toInt(),
            0,
            hermenexConfig.fanMaxPwm);

        airSetFan(fan);
        airSaveSettings();
    }

    /* ================= HOT AIR : POWER ON/OFF ================= */

    else if (msg.startsWith("HAPWR:")) {

        bool on = (msg.substring(6).toInt() != 0);
        airSwitchPower(on);
    }

    lastActivity = millis();
}

void initWebSocket()
{
    webSocket.begin();
    webSocket.onEvent(onWebSocketEvent);
}

void sendLiveData()
{
    String packet = "{";

    /* ----- Solder ----- */
    packet += "\"temp\":" + String(currentTemp) + ",";
    packet += "\"set\":" + String(targetTemp) + ",";

    packet += "\"pwm\":";
    packet += String(map(pwmOut, 0, maxPwmLimit, 0, 100));
    packet += ",";

    packet += "\"wifi\":";
    packet += (WiFi.status() == WL_CONNECTED ? "true" : "false");
    packet += ",";

    packet += "\"tip\":\"";
    packet += (activeTip ? activeTip->name : "NONE");
    packet += "\",";

    packet += "\"kp\":";
    packet += String(kp, 2);
    packet += ",";

    packet += "\"ki\":";
    packet += String(ki, 2);
    packet += ",";

    packet += "\"kd\":";
    packet += String(kd, 2);
    packet += ",";

    packet += "\"maxTemp\":";
    packet += String(activeTip ? activeTip->maxTemp : maxTemp);
    packet += ",";

    packet += "\"maxPWM\":";
    packet += String(activeTip ? activeTip->maxPWM : 100);
    packet += ",";

    packet += "\"state\":\"";

    if (tipError)
        packet += "ERROR";
    else if (boostMode)
        packet += "BOOST";
    else if (sleeping)
        packet += "SLEEP";
    else if (manualPWMMode)
        packet += "MANUAL";
    else
        packet += "PID";

    packet += "\",";

    /* ----- Hot Air ----- */
    packet += "\"haTemp\":" + String(airGetTemp()) + ",";
    packet += "\"haSet\":" + String(airGetTargetTemp()) + ",";
    packet += "\"haPower\":" + String(airGetPower()) + ",";
    packet += "\"haFan\":" + String(airGetFan()) + ",";
    packet += "\"haMode\":\"" + String(airGetModeStr()) + "\",";
    packet += "\"haAC\":" + String(airHasAC() ? "true" : "false") + ",";
    packet += "\"haMaxTemp\":" + String(hermenexConfig.tempMaxHotAir);

    packet += "}";

    webSocket.broadcastTXT(packet);
}
