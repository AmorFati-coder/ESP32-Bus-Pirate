#if defined(DEVICE_S3DEVKIT_ST7789)

#include "S3DevKitAdInput.h"
#include "Inputs/InputKeys.h"

S3DevKitAdInput::S3DevKitAdInput()
    : pendingInput(KEY_NONE),
      stableKey(KEY_NONE),
      lastObservedKey(KEY_NONE),
      lastTransitionMs(0) {
    analogReadResolution(12);
    pinMode(ADKEY_PIN, INPUT);
    delay(10);
}

char S3DevKitAdInput::handler() {
    while (true) {
        char c = readChar();
        if (c != KEY_NONE) return c;
        delay(5);
    }
}

char S3DevKitAdInput::readChar() {
    tick();
    char c = pendingInput;
    pendingInput = KEY_NONE;
    return c;
}

void S3DevKitAdInput::waitPress(uint32_t timeoutMs) {
    unsigned long start = millis();
    while (true) {
        if (readChar() != KEY_NONE) return;
        if (timeoutMs > 0 && (millis() - start) >= timeoutMs) return;
        delay(5);
    }
}

void S3DevKitAdInput::tick() {
    const int raw = readRaw();
    const char currentKey = classifyRaw(raw);
    const unsigned long now = millis();

    if (currentKey != lastObservedKey) {
        lastObservedKey = currentKey;
        lastTransitionMs = now;
        return;
    }

    if (currentKey != stableKey && (now - lastTransitionMs) >= ADKEY_DEBOUNCE_MS) {
        stableKey = currentKey;
        if (stableKey != KEY_NONE) {
            pendingInput = stableKey;
        }
    }
}

int S3DevKitAdInput::readRaw() {
    uint32_t total = 0;
    constexpr int samples = 8;
    for (int i = 0; i < samples; ++i) {
        total += analogRead(ADKEY_PIN);
        delayMicroseconds(300);
    }
    return static_cast<int>(total / samples);
}

char S3DevKitAdInput::classifyRaw(int raw) const {
    if (raw >= ADKEY_IDLE_MIN) return KEY_NONE;
    if (raw <= ADKEY_K4_MAX) return KEY_ARROW_DOWN;
    if (raw <= ADKEY_K1_MAX) return KEY_ARROW_LEFT;
    if (raw <= ADKEY_K2_MAX) return KEY_ARROW_RIGHT;
    if (raw <= ADKEY_K3_MAX) return KEY_OK;
    return KEY_NONE;
}

#endif
