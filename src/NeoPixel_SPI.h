#pragma once

#include <stdint.h>

#include "SPI.h"

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} LedColor_t;

class NeoPixel_SPI {
   public:
    NeoPixel_SPI(uint8_t ledNum);
    ~NeoPixel_SPI();

    void begin(SPIClass* spi);
    void end();

    bool setPixel(int8_t idx, uint8_t r, uint8_t g, uint8_t b);  // Set LED States to Pixel Buffer
    void show();                                                 // Transfer Current LED States

    void setFrequency();

   private:
    SPIClass *_spiLed;
    uint16_t _spi_freq;

    LedColor_t *_ledBuffer;

    uint8_t _ledNum; 
};