/**
 ******************************************************************************
 * @file        : payload.hpp
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

#pragma once

#include <Arduino.h>

class Payload {
   public:
    Payload() = default;
    void SetPayload(u_int8_t* payload, int len);
    int GetPeriod(int index);

   private:
    static const int periods_[16];
    u_int8_t payload_[4];
};
