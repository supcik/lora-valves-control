/**
 ******************************************************************************
 * @file        : lora_logger.cpp
 * @brief       : Log LoraWan Messages to Serial port (for debugging purpose)
 * @author      : Jacques Supcik <jacques.supcik@hefr.ch>
 * @date        : 2 June 2023
 ******************************************************************************
 * @copyright   : Copyright (c) 2023 HEIA-FR / ISC
 *                Haute école d'ingénierie et d'architecture de Fribourg
 *                Informatique et Systèmes de Communication
 * @attention   : SPDX-License-Identifier: MIT OR Apache-2.0
 ******************************************************************************
 * @details
 * Log LoraWan Messages to Serial port (for debugging purpose)
 ******************************************************************************
 */

#include "lora_logger.hpp"

#include <Arduino.h>
#include <ArduinoLog.h>
#include <lmic.h>

static const int kKeySize = 16;

void LoraLogEvent(ev_t event) {
    // ostime_t now = os_getTime();
    switch (event) {
        case EV_SCAN_TIMEOUT:
            Log.traceln("EV_SCAN_TIMEOUT");
            break;
        case EV_BEACON_FOUND:
            Log.traceln("EV_BEACON_FOUND");
            break;
        case EV_BEACON_MISSED:
            Log.traceln("EV_BEACON_MISSED");
            break;
        case EV_BEACON_TRACKED:
            Log.traceln("EV_BEACON_TRACKED");
            break;
        case EV_JOINING:
            Log.traceln("EV_JOINING");
            break;
        case EV_JOINED:
            Log.traceln("EV_JOINED");
            {
                char buf[32];  // NOLINT
                u4_t netid        = 0;
                devaddr_t devaddr = 0;
                u1_t nwkKey[kKeySize];
                u1_t artKey[kKeySize];
                LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);

                snprintf(buf, sizeof(buf), "%lu", netid);
                Log.traceln("netid: %s", buf);

                snprintf(buf, sizeof(buf), "%08lX", devaddr);
                Log.traceln("devaddr: %s", buf);

                char* ptr = buf;
                // NOLINTNEXTLINE
                for (size_t i = 0; i < sizeof(artKey); i++) {
                    if (i != 0) {
                        *(ptr++) = ':';
                    }
                    // NOLINTNEXTLINE(runtime/printf)
                    snprintf(ptr, 3, "%02X", artKey[i]);
                    ptr += 2;
                }
                *ptr = 0;
                Log.traceln("AppSKey: %s", buf);

                ptr = buf;
                // NOLINTNEXTLINE
                for (size_t i = 0; i < sizeof(nwkKey); i++) {
                    if (i != 0) {
                        *(ptr++) = ':';
                    }
                    // NOLINTNEXTLINE(runtime/printf)
                    snprintf(ptr, 3, "%02X", nwkKey[i]);
                    ptr += 2;
                }
                Log.traceln("NwkSKey: %s", buf);
            }
            break;

        case EV_JOIN_FAILED:
            Log.traceln("EV_JOIN_FAILED");
            break;
        case EV_REJOIN_FAILED:
            Log.traceln("EV_REJOIN_FAILED");
            break;
        case EV_TXCOMPLETE:
            Log.traceln("EV_TXCOMPLETE (includes waiting for RX windows)");
            if (LMIC.txrxFlags & TXRX_ACK) Serial.println("Received ack");
            if (LMIC.dataLen != 0) {
                Log.traceln("Received %i bytes of payload", LMIC.dataLen);
            }
            break;
        case EV_LOST_TSYNC:
            Log.traceln("EV_LOST_TSYNC");
            break;
        case EV_RESET:
            Log.traceln("EV_RESET");
            break;
        case EV_RXCOMPLETE:
            Log.traceln("EV_RXCOMPLETE");
            break;
        case EV_LINK_DEAD:
            Log.traceln("EV_LINK_DEAD");
            break;
        case EV_LINK_ALIVE:
            Log.traceln("EV_LINK_ALIVE");
            break;
        case EV_TXSTART:
            Log.traceln("EV_TXSTART");
            break;
        case EV_TXCANCELED:
            Log.traceln("EV_TXCANCELED");
            break;
        case EV_RXSTART:
            /* do not print anything -- it wrecks timing */
            break;
        case EV_JOIN_TXCOMPLETE:
            Log.traceln("EV_JOIN_TXCOMPLETE: no JoinAccept");
            break;

        default:
            Log.traceln("Unknown event: ");
            Log.traceln("%u", (unsigned)event);
            break;
    }
}
