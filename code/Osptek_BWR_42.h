#ifndef OSPTEK_BWR_42_H
#define OSPTEK_BWR_42_H

#include <Arduino.h>
#include <SPI.h>

// --- Configuration ---
#define OSP_LCD_WIDTH   300
#define OSP_LCD_HEIGHT  400 // Physical height
#define OSP_LOGICAL_H   200 // Logical height (Interlaced 2:1)
#define OSP_BUFFER_SIZE 30000

// --- Colors (4-bit Nibble) ---
// Mapped based on reverse-engineering:
// Bit 0,1 -> Red Channel; Bit 2,3 -> Black/White Channel
enum OspColor {
    COLOR_NEUTRAL = 0x0, // 0000 - Skin/Off
    COLOR_RED     = 0x3, // 0011 - Red
    COLOR_WHITE   = 0xC, // 1100 - White
    COLOR_BLACK   = 0xF  // 1111 - Black
};

class OsptekBWR {
public:
    OsptekBWR(int8_t cs, int8_t dc, int8_t rst, int8_t te, int8_t led = -1);
    
    void begin(bool highSpeedMode = false);
    void clear(OspColor color);
    void update(); // Send buffer to display
    
    // Graphics Primitives
    void drawPixel(int16_t x, int16_t y, OspColor color);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, OspColor color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, OspColor color);
    void drawCircle(int16_t x0, int16_t y0, int16_t r, OspColor color);

private:
    int8_t _cs, _dc, _rst, _te, _led;
    uint8_t _buffer[OSP_BUFFER_SIZE];
    bool _hpm_mode;

    void writeCmd(uint8_t cmd);
    void writeData(uint8_t data);
    void reset();
    void initRegs();
};

#endif