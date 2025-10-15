# System Patterns & Architecture

## Core Architecture

### **State-Driven UI Pattern**
The application uses a screen state enumeration that drives all UI and navigation logic:

```cpp
enum ScreenState {
    SCREEN_MAIN_MENU,
    SCREEN_TEMP_DISPLAY,
    SCREEN_TEMP_GAUGE,
    SCREEN_SETTINGS
};
```

Each state corresponds to a dedicated LVGL screen object loaded into the display buffer. State transitions are handled through the `switch_to_screen()` function which manages screen loading and initialization.

### **Interrupt-Driven Input Pattern**
Hardware button inputs use ESP32 GPIO interrupts rather than polled buttons:

```cpp
// Global flag variables set by interrupts
volatile bool button1_pressed = false;
volatile bool button2_pressed = false;
volatile bool key_pressed = false;

// ISR functions
void IRAM_ATTR button1_ISR() { button1_pressed = true; }
void IRAM_ATTR button2_ISR() { button2_pressed = true; }
void IRAM_ATTR key_ISR() { key_pressed = true; }
```

This pattern ensures responsive input handling without blocking the main loop.

### **Settings Tab Enumeration Pattern**
Settings tabs use a similar enumeration approach with looping navigation:

```cpp
enum SettingsTab {
    TAB_GENERAL,    // Temperature units
    TAB_DISPLAY,    // Brightness (future)
    TAB_SOUND,      // Audio settings
    TAB_ALERTS,     // Temperature thresholds
    TAB_ABOUT       // Device info
};

// Button handlers cycle through tabs
current_settings_tab = (SettingsTab)((current_settings_tab + 1) % 5);
```

## UI Component Patterns

### **Screen Creation Pattern**
Each screen follows a consistent creation pattern:

```cpp
void create_[screen_name]_ui() {
    [screen_name]_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color([screen_name]_screen, lv_color_hex(0x[background_color]), 0);
    
    // Title and layout elements
    // UI components assigned to global pointers
    // Event callbacks attached
}
```

### **LVGL Object Management**
UI objects are declared as global pointers and created once during setup:

```cpp
// Global UI object pointers
lv_obj_t *temp_display_screen;
lv_obj_t *temp_display_back_btn;
lv_obj_t *object_temp_label;
// ... etc
```

This avoids dynamic memory allocation during runtime and ensures consistent object references.

## Data Management Patterns

### **Sensor Reading Pattern**
Temperature readings use a timed update pattern to avoid overwhelming the sensor:

```cpp
void update_temperature_reading() {
    current_object_temp = mlx.readObjectTempC();
    current_ambient_temp = mlx.readAmbientTempC();
    
    static unsigned long last_debug = 0;
    if (millis() - last_debug >= 5000) {
        Serial.printf("Temps - Object: %.1f°C, Ambient: %.1f°C\n", 
                     current_object_temp, current_ambient_temp);
        last_debug = millis();
    }
}
```

### **Persistent Storage Pattern**
Settings are managed through ESP32's Preferences library:

```cpp
// Load during setup
preferences.begin("ncir_monitor", false);
use_celsius = preferences.getBool("use_celsius", true);
brightness_level = preferences.getInt("brightness", 128);
// ... etc

// Save when needed
preferences.putBool("use_celsius", use_celsius);
preferences.putInt("brightness", brightness_level);
// ... etc
```

## Timing Patterns

### **LVGL Refresh Timing**
LVGL updates happen on a fixed interval to balance responsiveness and performance:

```cpp
const uint32_t screenTickPeriod = 10;
uint32_t lastLvglTick = 0;

// In main loop
uint32_t current_time = millis();
if (current_time - lastLvglTick >= screenTickPeriod) {
    lvgl_tick_task(NULL);
    lastLvglTick = current_time;
}
```

### **Sensor Update Timing**
Temperature sensor readings are throttled to reasonable refresh rates:

```cpp
int update_rate = 500; // milliseconds
unsigned long last_update = 0;

if (millis() - last_update >= update_rate) {
    update_temperature_reading();
    last_update = millis();
}
```

## Alert System Patterns

### **Hysteresis-Based Alerts**
Temperature alerts use hysteresis to prevent rapid on/off cycling:

```cpp
if (current_object_temp <= low_temp_threshold && !low_alert_triggered) {
    play_beep(800, 300);
    low_alert_triggered = true;
} else if (current_object_temp > low_temp_threshold + 2.0) {
    low_alert_triggered = false; // Reset with 2°C hysteresis
}
```

## Current Architecture Challenges
- Screen state management becomes complex with settings navigation requirements
- Hardware button semantics need redesign for different screen contexts
- Visual highlighting system needs implementation
- Exit confirmation flow needs integration with state management
