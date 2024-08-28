#include "NeoPixel_SPI.h"
#include "USBSerial_CH32.h"

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
    // SerialUSB.printf("SET IDX: %2d, R: %3d, G: %3d, B: %3d\r\n", idx, r, g, b);
    delay(1);
    if (idx > _ledNum) return false;
    _ledBuffer[idx].r = r;
    _ledBuffer[idx].g = g;
    _ledBuffer[idx].b = b;
    // SerialUSB.printf("BUF IDX: %2d, R: %3d, G: %3d, B: %3d\r\n", idx, _ledBuffer[idx].r, _ledBuffer[idx].g, _ledBuffer[idx].b);
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
#if 0
        SerialUSB.printf("BUF IDX: %2d, R: %3d, G: %3d, B: %3d\r\n", i, _ledBuffer[i].r, _ledBuffer[i].g, _ledBuffer[i].b); 
        delay(1);
#endif
        rgb_conv(ledSendBuffer + (i * 12), _ledBuffer[i].r, _ledBuffer[i].g, _ledBuffer[i].b);
    }
#if 0
    SerialUSB.printf("TEST\r\n"); 
    for(uint8_t i = 0; i < _ledNum; i++) {
        SerialUSB.printf("BUF[%2d]: %2x%2x%2x%2x, %2x%2x%2x%2x, %2x%2x%2x%2x\r\n", i, 
            (uint8_t)*(ledSendBuffer + (i * 12) + 0), (uint8_t)*(ledSendBuffer + (i * 12) + 1), 
            (uint8_t)*(ledSendBuffer + (i * 12) + 2), (uint8_t)*(ledSendBuffer + (i * 12) + 3), 
            (uint8_t)*(ledSendBuffer + (i * 12) + 4), (uint8_t)*(ledSendBuffer + (i * 12) + 5), 
            (uint8_t)*(ledSendBuffer + (i * 12) + 6), (uint8_t)*(ledSendBuffer + (i * 12) + 7), 
            (uint8_t)*(ledSendBuffer + (i * 12) + 8), (uint8_t)*(ledSendBuffer + (i * 12) + 9), 
            (uint8_t)*(ledSendBuffer + (i * 12) + 10), (uint8_t)*(ledSendBuffer + (i * 12) + 11)); 
            delay(1); 
    }
#endif
    _spiLed->transfer(ledSendBuffer, sizeof(LedColor_t) * 4 * _ledNum, SPI_CONTINUE);

    free(ledSendBuffer);
}

void NeoPixel_SPI::setFrequency() {}
