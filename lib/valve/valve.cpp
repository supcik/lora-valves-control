/**
 ******************************************************************************
 * @file        : valve.cpp
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

#include "valve.hpp"

#include <Arduino.h>
#include <ArduinoLog.h>
#include <RTCZero.h>

// NOLINTNEXTLINE
Valve::Valve(int id, RTCZero* rtc, int pinOn, int pinOff, int pulseWidth)
    : id_(id),
      rtc_(rtc),
      pinOn_(pinOn),
      pinOff_(pinOff),
      pulseWidth_(pulseWidth),
      isOpen_(false) {
    pinMode(pinOn_, OUTPUT);
    pinMode(pinOff_, OUTPUT);
    t0_    = 0;
    delay_ = 0;
}

void Valve::Open() {
    if (isOpen_) {
        Log.warningln("Valve %i is already open", id_);
        return;
    }
    Log.infoln("Opening valve %i", id_);
    digitalWrite(pinOn_, HIGH);
    delay(pulseWidth_);
    digitalWrite(pinOn_, LOW);
    isOpen_ = true;
}

void Valve::Close(bool force) {
    if (!isOpen_ && !force) {
        Log.warningln("Valve %i is already closed", id_);
        return;
    }
    Log.infoln("Closing valve %i", id_);
    digitalWrite(pinOff_, HIGH);
    delay(pulseWidth_);
    digitalWrite(pinOff_, LOW);
    isOpen_ = false;
}

void Valve::ScheduleClose(int seconds) {
    t0_    = rtc_->getY2kEpoch();
    delay_ = seconds;
    Log.infoln("Scheduling valve %i to close in %i seconds", id_, seconds);
}

bool Valve::IsOpen() const { return isOpen_; }

void Valve::LoopOnce() {
    if (delay_ == 0) {
        return;
    }
    uint32_t t = rtc_->getY2kEpoch();  // NOLINT
    if (t - t0_ >= delay_) {
        Close();
        delay_ = 0;
    }
}
