/**
 * AtomS3 Lite RGB Rainbow Demo
 * 
 * Features:
 * - 4 LED modes: Rainbow cycle, Breathing, Random blink, Button control
 * - Click button to switch modes
 * - Serial output for debugging
 * 
 * Hardware: M5Stack AtomS3 Lite
 * - RGB LED: GPIO 35
 * - Button: GPIO 41
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
    BUTTON_CONTROL    // Manual color selection with button
};

// Global variables
Mode currentMode = RAINBOW_CYCLE;
uint8_t hue = 0;              // Current hue for rainbow
uint8_t brightness = 128;     // Default brightness
unsigned long lastUpdate = 0;

void setup() {
    // Initialize serial for debugging
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n========================================");
    Serial.println("  AtomS3 Lite RGB Rainbow Demo");
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
    
    Serial.println("Started! Click button to change modes.");
    Serial.println("Modes: Rainbow → Breath → Random → Manual");
}

/**
 * Mode 1: Rainbow Cycle
 * Smoothly cycles through all colors
 */
void rainbowCycle() {
    if (millis() - lastUpdate > 20) {  // Update every 20ms
        hue++;  // Increment hue
        leds[0] = CHSV(hue, 255, 255);  // Full saturation and brightness
        FastLED.show();
        lastUpdate = millis();
    }
}

/**
 * Mode 2: Breathing Effect
 * Current color fades in and out
 */
void breathingEffect() {
    static uint8_t breatheBrightness = 0;
    static int8_t direction = 1;
    
    if (millis() - lastUpdate > 10) {  // Smooth update
        breatheBrightness += direction * 2;  // Step size
        
        // Bounce at boundaries
        if (breatheBrightness >= 255) {
            breatheBrightness = 255;
            direction = -1;
        } else if (breatheBrightness <= 0) {
            breatheBrightness = 0;
            direction = 1;
        }
        
        leds[0] = CHSV(hue, 255, breatheBrightness);
        FastLED.show();
        lastUpdate = millis();
    }
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
 * Mode 4: Button Control
 * Each click changes to next preset color
 */
void buttonControl() {
    static uint8_t colorIndex = 0;
    static bool lastButtonState = HIGH;
    bool currentButtonState = digitalRead(BUTTON_PIN);
    
    // Detect button press (falling edge)
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
    const uint8_t numColors = sizeof(colors) / sizeof(colors[0]);
    
    if (lastButtonState == HIGH && currentButtonState == LOW) {
        delay(50);  // Debounce
        if (digitalRead(BUTTON_PIN) == LOW) {
            colorIndex = (colorIndex + 1) % numColors;
            leds[0] = colors[colorIndex];
            FastLED.show();
            
            // Print color name
            const char* colorNames[] = {"Red", "Green", "Blue", "Yellow", 
                                       "Purple", "Cyan", "Orange", "White"};
            Serial.print("Color: ");
            Serial.println(colorNames[colorIndex]);
            
            // Wait for release
            while (digitalRead(BUTTON_PIN) == LOW) delay(10);
        }
    }
    lastButtonState = currentButtonState;
}

/**
 * Check for mode change button press
 * Long press (>500ms) changes mode
 */
void checkModeButton() {
    static unsigned long buttonPressTime = 0;
    static bool buttonPressed = false;
    
    if (digitalRead(BUTTON_PIN) == LOW) {
        if (!buttonPressed) {
            buttonPressed = true;
            buttonPressTime = millis();
        } else if (millis() - buttonPressTime > 500) {
            // Long press detected - change mode
            currentMode = (Mode)((currentMode + 1) % 4);
            
            // Print mode name
            const char* modeNames[] = {"Rainbow Cycle", "Breathing", 
                                      "Random Blink", "Button Control"};
            Serial.print("Mode: ");
            Serial.println(modeNames[currentMode]);
            
            // Visual feedback - flash white
            leds[0] = CRGB::White;
            FastLED.show();
            delay(100);
            
            // Reset for new mode
            hue = random8();
            lastUpdate = 0;
            
            // Wait for button release
            while (digitalRead(BUTTON_PIN) == LOW) delay(10);
            buttonPressed = false;
        }
    } else {
        buttonPressed = false;
    }
}

void loop() {
    // Check for mode change (long press)
    checkModeButton();
    
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
