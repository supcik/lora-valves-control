/**
 ******************************************************************************
 * @file        : payload.cpp
 * @brief       : Payload decoder
 * @author      : Jacques Supcik <jacques.supcik@hefr.ch>
 * @date        : 31 May 2023
 ******************************************************************************
 * @copyright   : Copyright (c) 2023 HEIA-FR / ISC
 *                Haute école d'ingénierie et d'architecture de Fribourg
 *                Informatique et Systèmes de Communication
 * @attention   : SPDX-License-Identifier: MIT OR Apache-2.0
 ******************************************************************************
 * @details
 * Payload decoder
 ******************************************************************************
 */

#include "payload.hpp"

const int Payload::periods_[] = {
    0, 1, 2, 3, 4, 5, 6, 15, 20, 30, 40, 45, 60, 90, 120, -1};

// NOLINTNEXTLINE
void Payload::SetPayload(u_int8_t* payload, int len) {
    if (len > (int)sizeof(payload_)) {
        len = sizeof(payload_);
    }
    memcpy(payload_, payload, len);
}

// NOLINTNEXTLINE
int Payload::GetPeriod(int index) {
    int i = index / 2;  // NOLINT
    if (i < 0 || i >= (int)sizeof(payload_)) {
        return -1;
    }
    uint8_t p = payload_[i];  // NOLINT
    if (index % 2 == 0) {
        p = p >> 4;
    }
    return periods_[p & 0x0F];  // NOLINT
}
