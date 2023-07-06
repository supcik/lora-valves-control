/**
 ******************************************************************************
 * @file        : battery.hpp
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

#pragma once

#include <Arduino.h>
const int kDefaultVbatPin = A1;  // A1 is indeed 15/AIN2 on Adafruit Feather M0

class Battery {
   public:
    explicit Battery(int vbatPin = kDefaultVbatPin);
    uint16_t Voltage();

   private:
    int vbatPin_;
};
