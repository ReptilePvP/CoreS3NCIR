# Project Brief: NCIR Monitor Device

## Project Overview
Development of a handheld ESP32-based temperature monitoring device using NCIR (Non-Contact Infrared) sensing technology. The device features an M5Stack CoreS3 microcontroller with LCD touchscreen, MLX90614 temperature sensor, and hardware buttons for navigation.

## Core Requirements
- **Hardware Integration**: M5Stack CoreS3, MLX90614 NCIR sensor, LCD display
- **User Interface**: Multi-screen LVGL-based interface (Main Menu, Temperature Display, Temperature Gauge, Settings)
- **Navigation**: Hardware button-only control (no touchscreen interaction)
- **Functionality**:
  - Real-time temperature monitoring (object and ambient)
  - Visual temperature gauge with needle indicator
  - Audio and visual alerts for temperature thresholds
  - Persistent settings storage
- **User Experience**: Intuitive hardware button navigation with visual feedback

## Technical Specifications
- **Platform**: ESP32 (M5Stack CoreS3)
- **Display**: 320x240 LCD with LVGL graphics library
- **Sensor**: MLX90614 NCIR temperature sensor (-70°C to +380°C range)
- **Input**: 3 hardware buttons (Blue Button 1, Red Button 2, Key Button)
- **Output**: Built-in speaker for audio alerts, LED indicator
- **Storage**: ESP32 Preferences library for settings persistence

## Current Status
- Basic multi-screen UI implemented and functional
- Sensor integration working with real-time readings
- Settings system with temperature units, audio controls, and alert thresholds
- Hardware button interrupts implemented but require behavior redesign

## Remaining Work
- Complete hardware button layout redesign according to user specifications
- Implement settings navigation with hardware controls only
- Add visual selection indicators and highlighting
- Create exit confirmation system

## Success Criteria
- Device operates entirely through hardware buttons (no touch required)
- Intuitive navigation between all screens and settings
- Reliable temperature monitoring with appropriate alerts
- Clean, responsive user interface with clear visual feedback
