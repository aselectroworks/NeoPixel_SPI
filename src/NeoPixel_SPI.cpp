#include "NeoPixel_SPI.h"

NeoPixel_SPI::NeoPixel_SPI(uint8_t ledNum) {
    _ledNum = ledNum;
    _ledBuffer = (LedColor_t*)malloc(_ledNum * sizeof(LedColor_t));
}
NeoPixel_SPI::~NeoPixel_SPI() { free(_ledBuffer); }

void NeoPixel_SPI::begin(SPIClass* spi) {
    _spiLed = spi;
    for (uint8_t i = 0; i < _ledNum; i++) {
        setPixel(i, 0, 0, 0);
    }
}
void NeoPixel_SPI::end() {}

bool NeoPixel_SPI::setPixel(int8_t idx, uint8_t r, uint8_t g, uint8_t b) {
    if (idx > _ledNum) return false;
    _ledBuffer[idx].r = r;
    _ledBuffer[idx].g = g;
    _ledBuffer[idx].b = b;
    return true;
}

void bit_conv(uint8_t* buf, uint8_t val) {
    uint8_t mask = 0x80;
    for (int i = 0; i < 4; i++) {
        uint8_t result = (val & mask) ? 0xE : 0x8;
        result <<= 4;
        mask >>= 1;
        result |= (val & mask) ? 0xE : 0x8;
        mask >>= 1;
        buf[i] = result;
    }
}

void rgb_conv(uint8_t* buf, uint8_t r, uint8_t g, uint8_t b) {
    bit_conv(buf, g);
    bit_conv(&(buf[4]), r);
    bit_conv(&(buf[8]), b);
}

void NeoPixel_SPI::show() {
    // Transfer LED Buffer
    uint8_t* ledSendBuffer = (uint8_t*)malloc(sizeof(LedColor_t) * 4 * _ledNum);

    for (uint8_t i = 0; i < _ledNum; i++) {  // LED Count
        rgb_conv(ledSendBuffer + (i * 12), _ledBuffer[i].r, _ledBuffer[i].g, _ledBuffer[i].b);
    }
    _spiLed->transfer(ledSendBuffer, sizeof(LedColor_t) * 4 * _ledNum, SPI_CONTINUE);

    free(ledSendBuffer);
}

void NeoPixel_SPI::setFrequency() {}
