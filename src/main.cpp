/**
 * AtomS3 Lite RGB Rainbow Demo - Fixed Version
 * 
 * Features:
 * - 4 LED modes: Rainbow cycle, Breathing, Random blink, Button control
 * - Click button to switch modes (long press)
 * - Serial output for debugging
 * 
 * Hardware: M5Stack AtomS3 Lite
 * - RGB LED: GPIO 35
 * - Button: GPIO 41
 * 
 * Fixed:
 * - Mode now properly cycles through all 4 modes
 * - Breathing effect is smooth (full cycle)
 * - Button control doesn't conflict with mode switching
 */

#include <Arduino.h>
#include <FastLED.h>

// Pin definitions for AtomS3 Lite
#define LED_PIN     35
#define BUTTON_PIN  41
#define NUM_LEDS    1

// LED array
CRGB leds[NUM_LEDS];

// Color modes
enum Mode {
    RAINBOW_CYCLE,    // Smooth rainbow color cycling
    BREATH,           // Breathing effect with current color
    RANDOM_BLINK,     // Random color changes
    BUTTON_CONTROL    // Manual color selection
};

// Global variables
Mode currentMode = RAINBOW_CYCLE;
uint8_t hue = 0;              // Current hue for rainbow
uint8_t brightness = 128;     // Default brightness
unsigned long lastUpdate = 0;

// Button control color index
uint8_t buttonColorIndex = 0;

void setup() {
    // Initialize serial for debugging
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n========================================");
    Serial.println("  AtomS3 Lite RGB Rainbow Demo");
    Serial.println("  FIXED VERSION");
    Serial.println("========================================");
    
    // Initialize FastLED
    FastLED.addLeds<SK6812, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(brightness);
    
    // Initialize button with internal pullup
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    
    // Initial LED test - white flash
    leds[0] = CRGB::White;
    FastLED.show();
    delay(200);
    leds[0] = CRGB::Black;
    FastLED.show();
    delay(100);
    
    Serial.println("Started!");
    Serial.println("Long press button (>500ms) to change mode");
    Serial.println("Short press in Button Control mode to change color");
}

/**
 * Mode 1: Rainbow Cycle
 * Smoothly cycles through all colors
 */
void rainbowCycle() {
    if (millis() - lastUpdate > 20) {
        hue++;
        leds[0] = CHSV(hue, 255, 255);
        FastLED.show();
        lastUpdate = millis();
    }
}

/**
 * Mode 2: Breathing Effect - FIXED
 * Smooth sine wave breathing
 */
void breathingEffect() {
    // Use sine wave for smooth breathing
    // millis() / 1000.0 gives seconds, * 2 for speed
    float seconds = millis() / 1000.0 * 2;
    // sin() returns -1 to 1, map to 0 to 255
    int breatheBrightness = (sin(seconds) + 1.0) * 127.5;
    
    leds[0] = CHSV(hue, 255, breatheBrightness);
    FastLED.show();
}

/**
 * Mode 3: Random Blink
 * Changes to random color every 200ms
 */
void randomBlink() {
    if (millis() - lastUpdate > 200) {
        leds[0] = CHSV(random8(), 255, 255);
        FastLED.show();
        lastUpdate = millis();
    }
}

/**
 * Mode 4: Button Control - FIXED
 * Display current selected color
 */
void buttonControl() {
    CRGB colors[] = {
        CRGB::Red,
        CRGB::Green, 
        CRGB::Blue,
        CRGB::Yellow,
        CRGB::Purple,
        CRGB::Cyan,
        CRGB::Orange,
        CRGB::White
    };
    const uint8_t numColors = 8;
    
    // Keep showing the selected color
    leds[0] = colors[buttonColorIndex % numColors];
    FastLED.show();
}

/**
 * Unified button handling - FIXED
 * Distinguishes between short press (color change) and long press (mode change)
 */
void handleButton() {
    static unsigned long pressStartTime = 0;
    static bool wasPressed = false;
    static bool longPressHandled = false;
    
    bool isPressed = (digitalRead(BUTTON_PIN) == LOW);
    
    // Button just pressed
    if (isPressed && !wasPressed) {
        pressStartTime = millis();
        wasPressed = true;
        longPressHandled = false;
    }
    
    // Button being held - check for long press
    if (isPressed && wasPressed && !longPressHandled) {
        if (millis() - pressStartTime >= 500) {
            // LONG PRESS - Change mode
            longPressHandled = true;
            
            // Cycle through modes
            currentMode = (Mode)((currentMode + 1) % 4);
            
            // Print mode name
            const char* modeNames[] = {"Rainbow Cycle", "Breathing", 
                                      "Random Blink", "Button Control"};
            Serial.print("Mode changed to: ");
            Serial.println(modeNames[currentMode]);
            
            // Visual feedback - flash white
            leds[0] = CRGB::White;
            FastLED.show();
            delay(100);
            
            // Reset state for new mode
            hue = random8();
            lastUpdate = 0;
        }
    }
    
    // Button just released
    if (!isPressed && wasPressed) {
        unsigned long pressDuration = millis() - pressStartTime;
        
        // SHORT PRESS - Only in Button Control mode, change color
        if (pressDuration < 500 && !longPressHandled && currentMode == BUTTON_CONTROL) {
            buttonColorIndex++;
            
            const char* colorNames[] = {"Red", "Green", "Blue", "Yellow",
                                       "Purple", "Cyan", "Orange", "White"};
            Serial.print("Color: ");
            Serial.println(colorNames[buttonColorIndex % 8]);
        }
        
        wasPressed = false;
        longPressHandled = false;
    }
}

void loop() {
    // Handle button (both short and long press)
    handleButton();
    
    // Run current mode
    switch(currentMode) {
        case RAINBOW_CYCLE:
            rainbowCycle();
            break;
            
        case BREATH:
            breathingEffect();
            break;
            
        case RANDOM_BLINK:
            randomBlink();
            break;
            
        case BUTTON_CONTROL:
            buttonControl();
            break;
    }
}
