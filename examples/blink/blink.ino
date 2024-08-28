#include "NeoPixel_SPI.h"

#define SPI_NSS_PIN PB12
#define SPI_SCK_PIN PB13
#define SPI_MISO_PIN PB14
#define SPI_MOSI_PIN PB15

#define NUM_LEDS 1

// SPI Controls
SPIClass spiLed(SPI_MOSI_PIN, SPI_MISO_PIN, SPI_SCK_PIN);
SPISettings spiSettings = SPISettings(1500000, MSBFIRST, SPI_MODE2, SPI_TRANSMITONLY);
NeoPixel_SPI neopixel(NUM_LEDS);

void setup() {
    // SPI Configure fro Neopixel
    SPI.setMOSI(SPI_MOSI_PIN);
    SPI.setMISO(SPI_MISO_PIN);
    SPI.setSCLK(SPI_SCK_PIN);
    // SPI.setSSEL(SPI_NSS_PIN);
    digitalWrite(MOSI, HIGH);
    delay(1);
    SPI.begin();
    SPI.beginTransaction(spiSettings);
    neopixel.begin(&SPI);    
}

void loop() {
    neopixel.setPixel(0, 0, 0, 0);
    digitalWrite(MOSI, LOW);
    delay(1);
    neopixel.show();
    digitalWrite(MOSI, HIGH);
    delay(1);

    delay(1000); 
    
    neopixel.setPixel(0, 63, 0, 0);
    digitalWrite(MOSI, LOW);
    delay(1);
    neopixel.show();
    digitalWrite(MOSI, HIGH);
    delay(1);

    delay(1000); 

}
