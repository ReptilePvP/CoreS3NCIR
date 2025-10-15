# Technical Context

## Technology Stack

### **Hardware Platform**
- **Microcontroller**: ESP32 (M5Stack CoreS3)
  - Dual-core Tensilica LX6 processor @ 240MHz
  - 520KB SRAM, 16MB Flash storage
  - Integrated WiFi and Bluetooth
  - 25 GPIO pins available
- **Display**: 320x240 TFT LCD touchscreen (ILI9342C controller)
- **Sensor**: MLX90614 Non-Contact Infrared Thermometer
  - I2C interface (0x5A default address)
  - -70°C to +380°C measurement range
  - 0.5°C accuracy, 0.02°C resolution
- **Audio**: Built-in speaker with PWM output
- **Power**: Lithium-polymer battery with charging circuit

### **Software Stack**

#### **Core Framework**
- **Arduino Framework**: Provides GPIO control, serial communication, and hardware abstraction
- **M5Unified Library**: Hardware abstraction layer for M5Stack CoreS3
  - Button input handling
  - Display driver management
  - Power management
  - Speaker control

#### **Graphics & UI**
- **LVGL v8.3.9**: Lightweight Vector Graphics Library
  - Widget-based UI components
  - Event-driven interaction model
  - Customizable styling system
  - Low resource footprint optimized for embedded systems
- **m5gfx_lvgl**: M5Stack graphics adapter for LVGL
  - Hardware-accelerated rendering
  - Display buffer management
  - Touch input integration (disabled for this project)

#### **Sensor Integration**
- **Adafruit_MLX90614**: Arduino library for MLX90614 sensor
  - Temperature conversion functions
  - Ambient and object temperature readings
  - Basic sensor configuration

#### **Persistent Storage**
- **ESP32 Preferences**: Non-volatile key-value storage
  - Automatic wear leveling
  - Multiple namespaces to avoid conflicts
  - Simple API for basic data types

## Development Environment

### **PlatformIO IDE**
- **Framework**: Arduino
- **Platform**: Espressif32 v6.4.0
- **Board**: M5Stack CoreS3
- **Build Configuration**:
  ```ini
  [env:m5stack-cores3]
  platform = espressif32
  board = m5stack-cores3
  framework = arduino
  lib_deps =
      m5stack/M5Unified@^0.1.12
      adafruit/Adafruit MLX90614 Library@^2.1.1
      lvgl/lvgl@^8.3.9
  build_flags = -DBOARD_HAS_PSRAM
  monitor_speed = 115200
  ```

### **Core Constraints**
- **Memory**: Limited RAM requires static UI object allocation
- **Storage**: Limited flash space constrains UI complexity
- **Processing**: Single-core for LVGL (1 of 2 cores available)
- **Power**: Battery operation requires efficient refresh rates

## Current Implementation Details

### **LVGL Configuration**
- **Color Depth**: 16-bit RGB565
- **Buffer Strategy**: Double buffering for smooth animations
- **Theme**: Custom dark theme with green accents
- **Touch Input**: Disabled completely for hardware-only navigation

### **Sensor Configuration**
- **Update Rate**: 500ms refresh for real-time monitoring
- **Units**: Dual support for Celsius/Fahrenheit
- **Calibration**: Factory calibration assumed valid

### **Button Hardware**
- **Physical Pins**: GPIO 17 (Btn1), GPIO 18 (Btn2), GPIO 8 (Key)
- **Interrupt Types**: Falling edge detection with pull-up resistors
- **Debouncing**: Hardware-based through pull-up configuration

### **Audio System**
- **Driver**: M5Unified Speaker class
- **Output**: PWM modulation through built-in speaker
- **Feedback**: Configurable volume and tone patterns

## Critical Technical Decisions

### **Hardware-Only Navigation**
Decision: Disable touchscreen completely and rely entirely on three hardware buttons
Rationale: Enables operation with gloves, wet hands, and in various environmental conditions

### **LVGL over Custom Graphics**
Decision: Use LVGL despite complexity rather than custom graphics rendering
Rationale: LVGL provides robust widget system with built-in styling and event handling

### **GPIO Interrupts over Polling**
Decision: Use hardware interrupts for button inputs
Rationale: Prevents missed button presses and provides immediate responsiveness

### **Preferences over File System**
Decision: Use ESP32 Preferences library for settings storage
Rationale: Simpler API, automatic wear leveling, no file system overhead

## Build & Deployment Process

### **Build Pipeline**
1. **Source Compilation**: PlatformIO compiles C++ source
2. **Library Linking**: Resolves dependencies automatically
3. **Resource Optimization**: Compiler optimizes for size
4. **Flash Programming**: OTA/Serial upload to ESP32

### **Performance Optimization**
- **LVGL Refresh Rate**: 10ms update cycle
- **Sensor Sampling**: 2Hz update rate
- **Memory Management**: Static allocation for all UI objects
- **Power Efficiency**: Sleep modes available for future battery optimization

## Testing & Debugging

### **Hardware Testing**
- **Button Continuity**: GPIO pin testing with multimeter
- **Sensor Accuracy**: Comparison with known reference temperatures
- **Audio Output**: Tone verification at different volumes
- **Display Functionality**: LVGL rendering verification

### **Software Testing**
- **Unit Testing**: Sensor reading accuracy
- **Integration Testing**: Full navigation flow testing
- **Performance Testing**: Memory and CPU usage profiling
- **User Testing**: Hardware button navigation usability

## Future Technical Enhancements
- **LVGL v9 Migration**: Updated widget system and performance improvements
- **Sensor Calibration**: User-accessible offset adjustment
- **Battery Monitoring**: Power management and fuel gauge integration
- **Wireless Features**: WiFi connectivity for remote monitoring
