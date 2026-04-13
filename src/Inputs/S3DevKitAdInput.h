#pragma once

#if defined(DEVICE_S3DEVKIT_ST7789)

#include "Interfaces/IInput.h"
#include <Arduino.h>

#ifndef ADKEY_PIN
#define ADKEY_PIN 1
#endif

#ifndef ADKEY_K4_MAX
#define ADKEY_K4_MAX 200
#endif

#ifndef ADKEY_K1_MAX
#define ADKEY_K1_MAX 1800
#endif

#ifndef ADKEY_K2_MAX
#define ADKEY_K2_MAX 2700
#endif

#ifndef ADKEY_K3_MAX
#define ADKEY_K3_MAX 3600
#endif

#ifndef ADKEY_IDLE_MIN
#define ADKEY_IDLE_MIN 3800
#endif

#ifndef ADKEY_DEBOUNCE_MS
#define ADKEY_DEBOUNCE_MS 40
#endif

class S3DevKitAdInput : public IInput {
public:
    S3DevKitAdInput();

    char handler() override;
    char readChar() override;
    void waitPress(uint32_t timeoutMs = 0) override;

private:
    void tick();
    int readRaw();
    char classifyRaw(int raw) const;

    char pendingInput;
    char stableKey;
    char lastObservedKey;
    unsigned long lastTransitionMs;
};

#endif
