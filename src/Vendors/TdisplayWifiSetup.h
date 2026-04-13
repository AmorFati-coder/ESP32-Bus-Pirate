#pragma once

#if defined(DEVICE_TDISPLAYS3) || defined(DEVICE_S3DEVKIT_ST7789)

#include <Arduino.h>
#include <LovyanGFX.hpp>
#include "Inputs/InputKeys.h"
#include <Interfaces/IDeviceView.h>
#include <Interfaces/IInput.h>

#define NVS_SSID_KEY "ssid"
#define NVS_PASS_KEY "pass"

#define DARK_GREY 0x4208
#define HELP_COLOR 0xC618

bool setupTdisplayWifi(IDeviceView& view, IInput& input);

#endif
