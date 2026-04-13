#pragma once

#if defined(DEVICE_S3DEVKIT_ST7789)

#include "Interfaces/IDeviceView.h"
#include "States/GlobalState.h"

#include <Arduino.h>
#include <LovyanGFX.hpp>

#ifndef LCD_ST7789_WIDTH
#define LCD_ST7789_WIDTH 240
#endif

#ifndef LCD_ST7789_HEIGHT
#define LCD_ST7789_HEIGHT 320
#endif

#ifndef LCD_ST7789_ROTATION
#define LCD_ST7789_ROTATION 1
#endif

#ifndef LCD_ST7789_SCLK_PIN
#define LCD_ST7789_SCLK_PIN 12
#endif

#ifndef LCD_ST7789_MOSI_PIN
#define LCD_ST7789_MOSI_PIN 11
#endif

#ifndef LCD_ST7789_MISO_PIN
#define LCD_ST7789_MISO_PIN -1
#endif

#ifndef LCD_ST7789_CS_PIN
#define LCD_ST7789_CS_PIN 10
#endif

#ifndef LCD_ST7789_DC_PIN
#define LCD_ST7789_DC_PIN 14
#endif

#ifndef LCD_ST7789_RST_PIN
#define LCD_ST7789_RST_PIN 15
#endif

#ifndef LCD_ST7789_BL_PIN
#define LCD_ST7789_BL_PIN 16
#endif

#define DARK_GREY_RECT 0x4208
#define HELP_COLOR 0xC618

class LGFX_S3DevKitSt7789 : public lgfx::LGFX_Device {
    lgfx::Panel_ST7789 _panel;
    lgfx::Bus_SPI _bus;
    lgfx::Light_PWM _light;

public:
    LGFX_S3DevKitSt7789() {
        {
            auto cfg = _bus.config();
            cfg.spi_host = SPI2_HOST;
            cfg.spi_mode = 0;
            cfg.freq_write = 40000000;
            cfg.freq_read = 16000000;
            cfg.pin_sclk = LCD_ST7789_SCLK_PIN;
            cfg.pin_mosi = LCD_ST7789_MOSI_PIN;
            cfg.pin_miso = LCD_ST7789_MISO_PIN;
            cfg.pin_dc = LCD_ST7789_DC_PIN;
            cfg.spi_3wire = false;
            cfg.use_lock = true;
            cfg.dma_channel = SPI_DMA_CH_AUTO;
            _bus.config(cfg);
            _panel.setBus(&_bus);
        }

        {
            auto cfg = _panel.config();
            cfg.pin_cs = LCD_ST7789_CS_PIN;
            cfg.pin_rst = LCD_ST7789_RST_PIN;
            cfg.pin_busy = -1;
            cfg.panel_width = LCD_ST7789_WIDTH;
            cfg.panel_height = LCD_ST7789_HEIGHT;
            cfg.memory_width = LCD_ST7789_WIDTH;
            cfg.memory_height = LCD_ST7789_HEIGHT;
            cfg.offset_x = 0;
            cfg.offset_y = 0;
            cfg.offset_rotation = 0;
            cfg.invert = true;
            cfg.rgb_order = false;
            cfg.dlen_16bit = false;
            _panel.config(cfg);
        }

        {
            auto cfg = _light.config();
            cfg.pin_bl = LCD_ST7789_BL_PIN;
            cfg.invert = false;
            cfg.freq = 12000;
            cfg.pwm_channel = 7;
            _light.config(cfg);
            _panel.setLight(&_light);
        }

        setPanel(&_panel);
    }
};

class S3DevKitSt7789DeviceView : public IDeviceView {
public:
    S3DevKitSt7789DeviceView();

    void initialize() override;
    SPIClass& getSharedSpiInstance() override;
    void* getScreen() override;
    void logo() override;
    void welcome(TerminalTypeEnum& terminalType, std::string& terminalInfos) override;
    void show(PinoutConfig& config) override;
    void loading() override;
    void clear() override;
    void drawLogicTrace(uint8_t pin, const std::vector<uint8_t>& buffer, uint8_t step) override;
    void drawAnalogicTrace(uint8_t pin, const std::vector<uint8_t>& buffer, uint8_t step) override;
    void drawWaterfall(
        const std::string& title,
        float startValue,
        float endValue,
        const char* unit,
        int rowIndex,
        int rowCount,
        int level
    ) override;
    void setRotation(uint8_t rotation) override;
    void setBrightness(uint8_t brightness) override;
    uint8_t getBrightness() override;
    void topBar(const std::string& title, bool submenu, bool searchBar) override;
    void horizontalSelection(
        const std::vector<std::string>& options,
        uint16_t selectedIndex,
        const std::string& description1,
        const std::string& description2
    ) override;

private:
    LGFX_S3DevKitSt7789 tft;
    SPIClass sharedSpi{FSPI};
    uint8_t brightnessPct = 100;
    bool initialized_ = false;

    void drawCenterText(const std::string& text, int y, int fontSize);
    void welcomeWeb(const std::string& ip);
    void welcomeSerial(const std::string& baud);
};

#endif
