/**
 ******************************************************************************
 * @file        : main.cpp
 * @brief       : Main program
 * @author      : Jacques Supcik <jacques.supcik@hefr.ch>
 * @date        : 2 June 2023
 ******************************************************************************
 * @copyright   : Copyright (c) 2023 HEIA-FR / ISC
 *                Haute école d'ingénierie et d'architecture de Fribourg
 *                Informatique et Systèmes de Communication
 * @attention   : SPDX-License-Identifier: MIT OR Apache-2.0
 ******************************************************************************
 * @details
 * Main program
 ******************************************************************************
 */

#include <Arduino.h>
#include <ArduinoLog.h>
#include <ArduinoLowPower.h>
#include <SPI.h>
#include <hal/hal.h>
#include <lmic.h>
#include <math.h>
#include <stdint.h>

#include "battery.hpp"
#include "lora_logger.hpp"
#include "payload.hpp"
#include "secrets.h"
#include "valve.hpp"

#undef LOW_POWER

const int kLedPin                       = 13;
const int nOfValves                     = 6;
const uint32_t kSendInterval            = 5 * 60;  // 5 minutes
const uint32_t kLoopSleep               = 1 * 60;  // 1 minute
const uint32_t kLoraTransmissionTimeout = 30;      // 30 seconds

const int kValvePins[nOfValves][2] = {
    {14, 15}, {16, 17}, {18, 19}, {0, 1}, {12, 11}, {10, 5}};

void os_getArtEui(u1_t* buf) { memcpy_P(buf, kAppEUI, 8); }   // NOLINT
void os_getDevEui(u1_t* buf) { memcpy_P(buf, kDevEUI, 8); }   // NOLINT
void os_getDevKey(u1_t* buf) { memcpy_P(buf, kAppKey, 16); }  // NOLINT

// Pin mapping for Adafruit Feather M0 LoRa, etc.
// /!\ By default Adafruit Feather M0's pin 6 and DIO1 are not connected.
// Please ensure they are connected.
const lmic_pinmap lmic_pins = {
    .nss            = 8,
    .rxtx           = LMIC_UNUSED_PIN,
    .rst            = 4,
    .dio            = {3, 6, LMIC_UNUSED_PIN},
    .rxtx_rx_active = 0,
    .rssi_cal       = 8,  // LBT cal for the Adafruit Feather M0 LoRa, in dB
    .spi_freq       = 8000000,
};

// NOLINTBEGIN(*-global-variables)
static uint32_t lastTransmission = 0;
static bool loraTransmission     = false;

static RTCZero rtc;
static Battery battery;
static Valve* valves[nOfValves];
// NOLINTEND(*-global-variables)

void onEvent(ev_t event) {
    LoraLogEvent(event);
    switch (event) {
        case EV_JOINED:
            // Disable link check validation (automatically enabled
            // during join, but because slow data rates change max TX
            // size, we don't use it in this example.
            LMIC_setLinkCheckMode(0);
            break;

        case EV_JOIN_FAILED:
        case EV_REJOIN_FAILED:
        case EV_JOIN_TXCOMPLETE:
            loraTransmission = false;
            break;

        case EV_TXCOMPLETE:
            if (LMIC.dataLen == (nOfValves + 1) / 2) {
                Log.infoln("Received valid payload");
                Payload payload;
                payload.SetPayload(LMIC.frame + LMIC.dataBeg, LMIC.dataLen);
                for (int i = 0; i < nOfValves; i++) {
                    int p = payload.GetPeriod(i);  // NOLINT
                    if (p == 0) {
                        Log.infoln("Nothing to do for valve %i", i);
                        continue;
                    }
                    if (p < -1) { // open forever
                        valves[i]->Open();
                        continue;
                    }
                    if (p < 0) {
                        valves[i]->Close(true);
                        continue;
                    }
                    // p > 0
                    valves[i]->Open();
                    valves[i]->ScheduleClose(p *
                                             60);  // minute to seconds NOLINT
                }
            }

            loraTransmission = false;
            break;
        default:
            break;
    }
}

void SendLoraPacket() {
    // Check if there is not a current TX/RX job running
    if ((LMIC.opmode & OP_TXRXPEND) != 0) {
        Log.warningln("OP_TXRXPEND, not sending");
        return;
    }
    char buf[32];  // NOLINT
    float vbat = battery.Voltage();
    Log.infoln("Battery voltage: %sV", String(vbat).c_str());
    uint16_t vbat16 = (uint16_t)roundf(vbat * 100);  // NOLINT
    uint8_t payload[3];
    payload[0] = vbat16 & 0xFF;  // LSB - NOLINT
    payload[1] = vbat16 >> 8;    // MSB - NOLINT

    uint8_t valvesStatus = 0;  // NOLINT
    for (int i = 0; i < nOfValves; i++) {
        if (valves[i]->IsOpen()) {
            valvesStatus |= 1 << i;
        }
    }
    payload[2] = valvesStatus;

    snprintf(
        buf, sizeof(buf), "%02X:%02X %02X", payload[0], payload[1], payload[2]);
    Log.infoln("Queuing packet %s", buf);
    LMIC_setTxData2(1, payload, sizeof(payload), 0);
}

void setup() {
    delay(1000);  // Wait 1 seconds for the serial to be available - NOLINT
    pinMode(kLedPin, OUTPUT);
    digitalWrite(kLedPin, LOW);

    Serial.begin(115200);
    Log.begin(LOG_LEVEL_VERBOSE, &Serial);
    Log.infoln("Starting");

    rtc.begin(true);
    for (int i = 0; i < nOfValves; i++) {
        valves[i] = new Valve(i, &rtc, kValvePins[i][0], kValvePins[i][1]);
    }

    Log.infoln("Closing all valves");
    for (int i = 0; i < nOfValves; i++) {
        valves[i]->Close(true);
    }

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be
    // discarded.
    LMIC_reset();

    LMIC_setLinkCheckMode(0);
    LMIC_setDrTxpow(DR_SF7, 14);  // NOLINT

    loraTransmission = false;
    lastTransmission = rtc.getY2kEpoch() - kSendInterval;
}

void loop() {
    static bool first = true;
    uint32_t now      = rtc.getY2kEpoch();  // NOLINT

    if (loraTransmission &&
        (now - lastTransmission > kLoraTransmissionTimeout)) {
        Log.errorln("Transmission timeout... reseting the system");
        NVIC_SystemReset();  // Reset the system
        return;
    }

    if (loraTransmission) {
        // Still transmitting... be silent
        os_runloop_once();
        return;
    }

    digitalWrite(kLedPin, HIGH);
    if (first || (now - lastTransmission > kSendInterval)) {
        Log.infoln("Sending voltage");
        loraTransmission = true;
        first            = false;
        SendLoraPacket();
        lastTransmission = now;
        return;
    }

    char buf[64];   // NOLINT
    char* p = buf;  // NOLINT

    // cppcheck-suppress invalidPrintfArgType_uint
    p += snprintf(buf, sizeof(buf) - (p - buf), "%010lu", now);
    for (int i = 0; i < nOfValves; i++) {
        p += snprintf(p,
                      sizeof(buf) - (p - buf),
                      " %d:%c",
                      i,
                      valves[i]->IsOpen() ? 'O' : 'X');
    }

    Log.infoln("Loop : @%s", buf);

    for (int i = 0; i < nOfValves; i++) {
        valves[i]->LoopOnce();
    }
    digitalWrite(kLedPin, LOW);

#ifdef LOW_POWER
    LowPower.deepSleep(kLoopSleep * 1000);  // NOLINT
#else
    delay(kLoopSleep * 1000);  // NOLINT
#endif
}
