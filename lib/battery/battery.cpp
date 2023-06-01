/**
 ******************************************************************************
 * @file        : battery.cpp
 * @brief       : Battery Driver
 * @author      : Jacques Supcik <jacques.supcik@hefr.ch>
 * @date        : 31 May 2023
 ******************************************************************************
 * @copyright   : Copyright (c) 2023 HEIA-FR / ISC
 *                Haute école d'ingénierie et d'architecture de Fribourg
 *                Informatique et Systèmes de Communication
 * @attention   : SPDX-License-Identifier: MIT OR Apache-2.0
 ******************************************************************************
 * @details
 * Battery Driver
 ******************************************************************************
 */

#include "battery.hpp"

#include <Arduino.h>

Battery::Battery(int pin) : vbatPin_(pin) {}

// Code from
// https://learn.adafruit.com/adafruit-feather-m0-adalogger/power-management
float Battery::Voltage() {
    float vbat = analogRead(vbatPin_);
    vbat *= 2;     // we divided by 2, so multiply back
    vbat *= 3.3;   // Multiply by 3.3V, our reference voltage / NOLINT
    vbat /= 1024;  // convert to voltage / NOLINT
    return vbat;
}
