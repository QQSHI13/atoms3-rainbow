# 🌈 AtomS3 Lite RGB Rainbow Demo

A colorful RGB LED demo for M5Stack AtomS3 Lite with 4 different lighting modes.

![Demo](demo.png)

## ✨ Features

| Mode | Description |
|------|-------------|
| 🌈 **Rainbow Cycle** | Smoothly cycles through all colors |
| 💨 **Breathing** | Fades in and out with current color |
| ✨ **Random Blink** | Changes to random colors every 200ms |
| 🎨 **Button Control** | Manually select from 8 preset colors |

## 🛠️ Hardware

- **M5Stack AtomS3 Lite**
- RGB LED: GPIO 35
- Button: GPIO 41

## 📦 Installation

### Prerequisites

- [PlatformIO](https://platformio.org/) installed
- USB-C cable

### Setup

1. Clone or download this repository:
```bash
git clone https://github.com/QQSHI13/atoms3-rainbow.git
cd atoms3-rainbow
```

2. Open in PlatformIO:
```bash
pio run --target upload
```

Or use VS Code with PlatformIO extension.

## 🎮 Usage

### Mode Switching
- **Long press button (>500ms)**: Switch between 4 modes
- **Serial Monitor (115200 baud)**: See current mode

### Mode Details

#### 🌈 Rainbow Cycle (Default)
- Automatically cycles through rainbow colors
- Updates every 20ms for smooth transition

#### 💨 Breathing
- Current color fades in and out
- Relaxing breathing-like effect

#### ✨ Random Blink
- Jumps to random color every 200ms
- Party mode!

#### 🎨 Button Control
- Short press: Cycle through 8 colors (Red, Green, Blue, Yellow, Purple, Cyan, Orange, White)
- Color name printed to Serial Monitor

## 🔧 Customization

### Change Rainbow Speed
```cpp
// In rainbowCycle()
if (millis() - lastUpdate > 10)  // Lower = faster
```

### Change Brightness
```cpp
// In setup()
FastLED.setBrightness(200);  // 0-255, default is 128
```

### Add More Colors
```cpp
// In buttonControl()
CRGB colors[] = {
    CRGB::Red,
    CRGB::Green,
    CRGB(255, 100, 0),  // Custom orange
    // Add more...
};
```

## 📁 Project Structure

```
atoms3-rainbow/
├── platformio.ini      # PlatformIO configuration
├── src/
│   └── main.cpp        # Main source code
└── README.md           # This file
```

## 📝 Code Explanation

### Key Functions

- `rainbowCycle()`: Increments hue value, creates smooth color transition
- `breathingEffect()`: Sine wave-like brightness modulation
- `randomBlink()`: Uses `random8()` for random hue
- `buttonControl()`: Debounced button reading with color cycling
- `checkModeButton()`: Long press detection for mode switching

### Libraries Used

- [FastLED](https://github.com/FastLED/FastLED): LED control library

## 🔌 Wiring

No wiring needed! AtomS3 Lite has built-in RGB LED on GPIO 35.

For external LED (optional):
- Connect SK6812/WS2812 LED data pin to GPIO 35
- Connect GND to GND
- Connect VCC to 3.3V or 5V

## 📄 License

GPL-3.0 License - See [LICENSE](LICENSE) for details.

## 🙏 Credits

Built by [QQ](https://github.com/QQSHI13) with [Nova ☄️](https://github.com/QQSHI13/nova)

---

**Enjoy your colorful AtomS3 Lite! 🦐✨**
