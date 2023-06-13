/**
 ******************************************************************************
 * @file        : valve.hpp
 * @brief       : Valve Driver
 * @author      : Jacques Supcik <jacques.supcik@hefr.ch>
 * @date        : 31 May 2023
 ******************************************************************************
 * @copyright   : Copyright (c) 2023 HEIA-FR / ISC
 *                Haute école d'ingénierie et d'architecture de Fribourg
 *                Informatique et Systèmes de Communication
 * @attention   : SPDX-License-Identifier: MIT OR Apache-2.0
 ******************************************************************************
 * @details
 * Valve Driver
 ******************************************************************************
 */

#pragma once

#include <Arduino.h>
#include <RTCZero.h>

static const int kPulseWidth = 300; // 300 ms
class Valve {
   public:
    // NOLINTNEXTLINE
    Valve(int id, RTCZero* rtc, int pinOn, int pinOff, int pulseWidth = kPulseWidth);
    void Open();
    void Close(bool force = false);
    void ScheduleClose(int seconds);
    bool IsOpen() const;

    void LoopOnce();

   private:
    int id_;
    RTCZero* rtc_;
    int pinOn_;
    int pinOff_;
    int pulseWidth_;  // ms
    bool isOpen_;
    uint32_t t0_;
    uint32_t delay_;
};
