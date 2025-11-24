#include "Osptek_BWR_42.h"

/*
 * Demo: Procedural Flower Pattern
 * Hardware: ESP32-S3 + Osptek 4.2" BWR (ST7306)
 * * Wiring:
 * SCLK -> GPIO 12
 * MOSI -> GPIO 11
 * CS   -> GPIO 10
 * DC   -> GPIO 9
 * RST  -> GPIO 4
 * TE   -> GPIO 5
 * LED  -> GPIO 2
 */

OsptekBWR lcd(10, 9, 4, 5, 2);

// Mathematical Flower Generator
// Uses implicit shading logic to map calculated values to 4-Color Palette
void drawFlower() {
    int cx = OSP_LCD_WIDTH / 2;
    int cy = OSP_LCD_HEIGHT / 2;
    
    for (int y = 0; y < OSP_LCD_HEIGHT; y++) {
        for (int x = 0; x < OSP_LCD_WIDTH; x++) {
            // Normalize coordinates
            float nx = (x - cx) / 150.0f;
            float ny = (y - cy) / 200.0f;
            
            // Polar coordinates
            float r = sqrt(nx*nx + ny*ny);
            float angle = atan2(ny, nx);
            
            // Petal shape formula: r = cos(k * theta)
            // Use 5 petals (k=2.5 for internal loops, 5 for simple)
            float petal = abs(cos(2.5 * angle));
            
            // Distort radius based on petal shape
            float shape = petal * 0.8; 
            
            OspColor pixelColor;
            
            // Color Mapping Logic ("Shader")
            if (r < 0.15) {
                // Center: Black
                pixelColor = COLOR_BLACK;
            } else if (r < shape) {
                // Petals: Red
                // Add some texture: if radius is consistent with pattern, make it white stripes
                if (int(r * 20) % 2 == 0) {
                    pixelColor = COLOR_RED;
                } else {
                    pixelColor = COLOR_NEUTRAL; // Subtle texture
                }
            } else if (r < shape + 0.05) {
                // Outline: Black
                pixelColor = COLOR_BLACK;
            } else {
                // Background: White
                pixelColor = COLOR_WHITE;
            }
            
            lcd.drawPixel(x, y, pixelColor);
        }
    }
    
    // Draw text box (Manual pixel font drawing is complex, using simple rects here)
    // Draw a black frame at bottom
    lcd.fillRect(20, 350, 260, 30, COLOR_BLACK);
    lcd.fillRect(22, 352, 256, 26, COLOR_WHITE);
    // Red "Progress Bar" inside
    lcd.fillRect(76, 355, 200, 20, COLOR_RED);

    // Indicate scan direction
    int arrowX = 280;
    int arrowY = 355;
    lcd.fillRect(arrowX + 3, arrowY + 6, 4, 15, COLOR_RED);
    lcd.fillRect(arrowX,     arrowY + 4, 10, 2, COLOR_RED); 
    lcd.fillRect(arrowX + 2, arrowY + 2, 6,  2, COLOR_RED); 
    lcd.fillRect(arrowX + 4, arrowY + 0, 2,  2, COLOR_RED); 
}

void setup() {
    Serial.begin(115200);
    Serial.println("Booting Osptek Demo...");

    // Initialize LCD
    // param: true = 16Hz HPM Mode, false = 1Hz LPM Mode
    // Note: 16Hz mode consumes more power but allows animation.
    //       1Hz mode is best for static images and low power.
    lcd.begin(false); 
    
    Serial.println("LCD Initialized. Rendering...");
    
    // 1. Clear Screen to White
    lcd.clear(COLOR_WHITE);
    lcd.update();
    delay(2000);

    // 2. Draw Math Flower
    long startTime = millis();
    drawFlower();
    long renderTime = millis() - startTime;
    Serial.printf("Render Time: %ld ms\n", renderTime);
    
    lcd.update();
    Serial.println("Display Updated.");
}

void loop() {
    // In 1Hz mode, we just hold the image.
    // If initialized with begin(true), you could animate here.
    delay(1000);
}