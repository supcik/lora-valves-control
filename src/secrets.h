// clang-format off
/**
 ******************************************************************************
 * @file        : secrets.h
 * @brief       : Defaults for secrets
 * @author      : Jacques Supcik <jacques.supcik@hefr.ch>
 * @date        : 2 June 2023
 ******************************************************************************
 * @copyright   : Copyright (c) 2023 HEIA-FR / ISC
 *                Haute école d'ingénierie et d'architecture de Fribourg
 *                Informatique et Systèmes de Communication
 * @attention   : SPDX-License-Identifier: MIT OR Apache-2.0
 ******************************************************************************
 * @details
 *
 * Add your "real" secrets in a ".env" file in the root of the project. The content
 * of the file should be:
 *
 * APP_EUI="{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }"
 * DEV_EUI="{ 0x??, 0x??, 0x??, 0x??, 0x??, 0x??, 0x??, 0x?? }"
 * APP_KEY="{ 0x??, 0x??, 0x??, 0x??, 0x??, 0x??, 0x??, 0x??, 0x??, 0x??, 0x??, 0x??, 0x??, 0x??, 0x??, 0x?? }"

 ******************************************************************************
 */
// clang-format on

#pragma once

#include <Arduino.h>
#include <stdint.h>

// APP_EUI : 8 bytes, LSB
#ifndef APP_EUI
// clang-format off
#define APP_EUI { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
// clang-format on
#endif

// DEV_EUI : 8 bytes, LSB
#ifndef DEV_EUI
// clang-format off
#define DEV_EUI { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 }
// clang-format on
#endif

// APP_KEY : 16 bytes, LSB
#ifndef APP_KEY
// clang-format off
// NOLINTNEXTLINE
#define APP_KEY { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F }
// clang-format on
#endif

const uint8_t kAppEUI[] = APP_EUI;
const uint8_t kDevEUI[] = DEV_EUI;
const uint8_t kAppKey[] = APP_KEY;
