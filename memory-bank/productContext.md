# Product Context: NCIR Monitor

## Problem Statement
Users need a reliable, handheld device for non-contact temperature monitoring in various scenarios:
- Cooking and food preparation (measuring surface temperatures without contact)
- Industrial equipment monitoring (checking machinery temperatures safely)
- Personal health monitoring (measuring body temperature from a distance)
- HVAC system diagnostics (checking vent and duct temperatures)

Current solutions are often:
- Too expensive (professional IR thermometers)
- Inaccurate for specific use cases
- Lack proper user interface and alerts
- Require constant attention and manual recording

## Solution Overview
The NCIR Monitor is a dedicated handheld device that provides:
- **Accurate NCIR sensing** with MLX90614 sensor (-70°C to +380°C range)
- **Real-time monitoring** with continuous display updates
- **Multiple visualization modes** (digital display, analog gauge)
- **Intelligent alerts** with customizable temperature thresholds
- **Hardware-only navigation** for use in various environments (gloved hands, wet conditions, etc.)
- **Persistent settings** and intuitive interface

## User Experience Goals
1. **Simple Operation**: Device works immediately after power-on with no complex setup
2. **Clear Feedback**: Visual indicators and audio cues guide user interaction
3. **Reliable Performance**: Consistent temperature readings regardless of environmental conditions
4. **Intuitive Controls**: Hardware buttons map to logical functions throughout the device
5. **Safety First**: Non-contact measurement prevents burns and contamination

## Key User Scenarios

### **Chef Monitoring Food Temperatures**
- Quickly check if cooking surfaces are at safe temperatures
- Monitor multiple food items without touching contaminated surfaces
- Receive alerts when temperatures exceed food safety thresholds

### **Mechanic Checking Equipment**
- Non-contact measurement of machinery components
- Immediate alerts for equipment running too hot/cold
- Visual gauge provides quick reference for normal operating ranges

### **Field Service Technician**
- Hands-free operation while carrying tools
- Hardware buttons work with work gloves
- Persistent settings maintain calibration between jobs

## Design Principles

### **Hardware-First Navigation**
- **Main Menu**: Simple access to all primary functions
- **Temperature Modes**: Dedicated screens for different viewing preferences
- **Settings**: Comprehensive configuration without requiring menus
- **No Touch Required**: All functions accessible via three hardware buttons

### **Visual Design**
- **Clean Interface**: Information presented clearly and concisely
- **Consistent Layout**: Similar patterns across all screens
- **Status Indicators**: Clear feedback about device state and alerts
- **Accessible Colors**: High contrast for various lighting conditions

### **Audio Design**
- **Contextual Alerts**: Different tones for temperature thresholds, errors, and confirmations
- **Configurable Volume**: Adjustable audio feedback levels
- **Non-intrusive**: Alerts get attention without being disruptive

## Success Metrics
- **Usability**: Users can navigate all functions within 30 seconds of first use
- **Accuracy**: Temperature readings within ±1°C of reference measurements
- **Reliability**: Device operates for 8+ hours on single charge under normal use
- **Safety**: Eliminates need for contact temperature measurement in hazardous scenarios
