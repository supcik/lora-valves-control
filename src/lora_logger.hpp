/**
 ******************************************************************************
 * @file        : lora_logger.hpp
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

#pragma once

#include <Arduino.h>
#include <lmic.h>

void LoraLogEvent(ev_t event);
