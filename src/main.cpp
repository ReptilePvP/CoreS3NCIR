#include <Arduino.h>
#include <M5Unified.h>
#include <Adafruit_MLX90614.h>
#include "m5gfx_lvgl.hpp"

// NCIR Sensor
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Screen dimensions for CoreS3
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// LVGL Refresh time
static const uint32_t screenTickPeriod = 10;  // Increased to 10ms for better stability
static uint32_t lastLvglTick = 0;

// Screen dimensions for CoreS3
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define LV_TICK_PERIOD_MS 10

// LVGL task parameters
#define LVGL_TASK_CORE 1
#define LVGL_TASK_PRIORITY 5
#define LVGL_STACK_SIZE 32768

// UI ObjectsSUCCESS
lv_obj_t *main_screen;
lv_obj_t *temp_label;
lv_obj_t *ambient_label;
lv_obj_t *object_label;
lv_obj_t *status_label;
lv_obj_t *unit_btn;
lv_obj_t *calibrate_btn;
lv_obj_t *settings_btn;
lv_obj_t *chart;
lv_chart_series_t *chart_series;

// Settings screen
lv_obj_t *settings_screen;
lv_obj_t *back_btn;
lv_obj_t *temp_unit_switch;
lv_obj_t *update_rate_slider;
lv_obj_t *update_rate_label;

// Variables
bool use_celsius = true;
int update_rate = 1000; // milliseconds
unsigned long last_update = 0;
float temp_history[50];
int32_t temp_history_int[50]; // Integer version for chart
int history_index = 0;

// Function declarations
void setup_lvgl();
void create_main_ui();
void create_settings_ui();
void update_temperature_display();
void update_chart();
void temp_unit_btn_event_cb(lv_event_t *e);
void calibrate_btn_event_cb(lv_event_t *e);
void settings_btn_event_cb(lv_event_t *e);
void back_btn_event_cb(lv_event_t *e);
void temp_unit_switch_event_cb(lv_event_t *e);
void update_rate_slider_event_cb(lv_event_t *e);

// LVGL tick task from CoreS3 User Demo (modified for compatibility)
static void lvgl_tick_task(void *arg) {
  (void)arg;
  // Use lv_tick_get instead of lv_tick_inc which is not available in this LVGL version
  static uint32_t last_tick = 0;
  uint32_t current_tick = millis();
  if (current_tick - last_tick > LV_TICK_PERIOD_MS) {
      last_tick = current_tick;
      lv_task_handler(); // Process LVGL tasks
  }
}

// LVGL task
void lvgl_task(void *pvParameters) {
  while (1) {
    lv_timer_handler();
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void setup() {
  Serial.begin(115200);
  // Initialize M5Stack
  auto cfg = M5.config();
  M5.begin(cfg);
  
  Serial.println("M5Stack CoreS3 initialized");
  
  // Initialize NCIR sensor
  if (!mlx.begin()) {
    Serial.println("Error initializing MLX90614 sensor!");
    while (1);
  }
  Serial.println("NCIR sensor initialized");
  
  // Test sensor reading
  float test_obj = mlx.readObjectTempC();
  float test_amb = mlx.readAmbientTempC();
  Serial.printf("Sensor test - Object: %.1f°C, Ambient: %.1f°C\n", test_obj, test_amb);
  
  // Initialize LVGL
  Serial.println("Before LVGL init");
  lv_init();
  Serial.println("After LVGL init");
  Serial.println("Before m5gfx_lvgl_init");
  m5gfx_lvgl_init();
  Serial.println("After m5gfx_lvgl_init");
  Serial.println("LVGL setup complete");

  // LVGL task creation removed - using main loop refresh instead
  Serial.println("LVGL refresh will be handled in main loop");
  
  // Create UI
  create_main_ui();
  Serial.println("UI created");
  
  Serial.println("M5Stack CoreS3 NCIR UI Ready!");
}

void loop() {
  M5.update();
  
  // Improved LVGL refresh timing
  uint32_t current_time = millis();
  if (current_time - lastLvglTick >= screenTickPeriod) {
    lvgl_tick_task(NULL);
    lastLvglTick = current_time;
  }
  
  // Update temperature display at specified rate
  if (millis() - last_update >= update_rate) {
    update_temperature_display();
    last_update = millis();
    
    // Debug: Print temperature every 5 seconds
    static unsigned long last_debug = 0;
    if (millis() - last_debug >= 5000) {
      float obj_temp = mlx.readObjectTempC();
      float amb_temp = mlx.readAmbientTempC();
      Serial.printf("Temps - Object: %.1f°C, Ambient: %.1f°C\n", obj_temp, amb_temp);
      last_debug = millis();
    }
  }
  
  // Small delay to prevent watchdog issues
  delay(1);
}


void create_main_ui() {
  Serial.println("Creating main UI...");
  
  // Test display first
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setTextColor(TFT_WHITE);
  M5.Display.setTextSize(2);
  M5.Display.setCursor(10, 10);
  M5.Display.println("Display Test");
  delay(2000);
  
  // Create main screen
  main_screen = lv_obj_create(NULL);
  Serial.println("Main screen created");
  
  // Set background
  lv_obj_set_style_bg_color(main_screen, lv_color_hex(0x000000), 0); // Black background
  lv_obj_set_style_border_width(main_screen, 0, 0);
  lv_obj_set_style_pad_all(main_screen, 0, 0);
  lv_screen_load(main_screen);
  Serial.println("Main screen loaded");
  
  // Create a simple test label first
  lv_obj_t *test_label = lv_label_create(main_screen);
  lv_label_set_text(test_label, "NCIR Ready");
  lv_obj_set_style_text_color(test_label, lv_color_hex(0x00FF00), 0);
  lv_obj_align(test_label, LV_ALIGN_TOP_MID, 0, 20);
  Serial.println("Test label created");
  
  // Title
  lv_obj_t *title = lv_label_create(main_screen);
  lv_label_set_text(title, "NCIR Monitor");
  lv_obj_set_style_text_color(title, lv_color_hex(0x00ff00), 0);
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 50);
  Serial.println("Title created");
  
  // Object temperature (main display) - simplified
  object_label = lv_label_create(main_screen);
  lv_label_set_text(object_label, "Object: --°C");
  lv_obj_set_style_text_color(object_label, lv_color_hex(0xffffff), 0);
  lv_obj_align(object_label, LV_ALIGN_CENTER, 0, -20);
  Serial.println("Object label created");
  
  // Ambient temperature
  ambient_label = lv_label_create(main_screen);
  lv_label_set_text(ambient_label, "Ambient: --°C");
  lv_obj_set_style_text_color(ambient_label, lv_color_hex(0xcccccc), 0);
  lv_obj_align(ambient_label, LV_ALIGN_CENTER, 0, 0);
  Serial.println("Ambient label created");
  
  // Status
  status_label = lv_label_create(main_screen);
  lv_label_set_text(status_label, "Status: Ready");
  lv_obj_set_style_text_color(status_label, lv_color_hex(0x00ff00), 0);
  lv_obj_align(status_label, LV_ALIGN_CENTER, 0, 20);
  Serial.println("Status label created");
  
  // Simple test button
  unit_btn = lv_btn_create(main_screen);
  lv_obj_set_size(unit_btn, 100, 40);
  lv_obj_align(unit_btn, LV_ALIGN_CENTER, 0, 50);
  lv_obj_add_event_cb(unit_btn, temp_unit_btn_event_cb, LV_EVENT_CLICKED, NULL);
  
  lv_obj_t *unit_label = lv_label_create(unit_btn);
  lv_label_set_text(unit_label, "Test");
  lv_obj_center(unit_label);
  Serial.println("Test button created");
  
  // Initialize temperature history
  for (int i = 0; i < 50; i++) {
    temp_history[i] = 0;
    temp_history_int[i] = 0;
  }
  
  // Force a refresh and give time for initialization
  lv_refr_now(NULL);
  delay(500); // Give time for display to settle
  Serial.println("UI creation complete - forced refresh");
}

void create_settings_ui() {
  // Create settings screen
  settings_screen = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(settings_screen, lv_color_hex(0x1a1a1a), 0);
  lv_obj_set_style_border_width(settings_screen, 0, 0);
  lv_obj_set_style_pad_all(settings_screen, 0, 0);
  
  // Title
  lv_obj_t *title = lv_label_create(settings_screen);
  lv_label_set_text(title, "Settings");
  lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
  lv_obj_set_style_text_color(title, lv_color_hex(0x00ff00), 0);
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);
  
  // Temperature unit switch
  lv_obj_t *unit_label = lv_label_create(settings_screen);
  lv_label_set_text(unit_label, "Temperature Unit:");
  lv_obj_set_style_text_color(unit_label, lv_color_hex(0xffffff), 0);
  lv_obj_align(unit_label, LV_ALIGN_TOP_LEFT, 20, 50);
  
  temp_unit_switch = lv_switch_create(settings_screen);
  lv_obj_align(temp_unit_switch, LV_ALIGN_TOP_RIGHT, -20, 50);
  lv_obj_add_event_cb(temp_unit_switch, temp_unit_switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_state(temp_unit_switch, LV_STATE_CHECKED); // Default to Celsius
  
  lv_obj_t *unit_c_label = lv_label_create(settings_screen);
  lv_label_set_text(unit_c_label, "°C");
  lv_obj_set_style_text_color(unit_c_label, lv_color_hex(0xffffff), 0);
  lv_obj_align(unit_c_label, LV_ALIGN_TOP_LEFT, 20, 80);
  
  lv_obj_t *unit_f_label = lv_label_create(settings_screen);
  lv_label_set_text(unit_f_label, "°F");
  lv_obj_set_style_text_color(unit_f_label, lv_color_hex(0xffffff), 0);
  lv_obj_align(unit_f_label, LV_ALIGN_TOP_RIGHT, -20, 80);
  
  // Update rate slider
  lv_obj_t *rate_label = lv_label_create(settings_screen);
  lv_label_set_text(rate_label, "Update Rate:");
  lv_obj_set_style_text_color(rate_label, lv_color_hex(0xffffff), 0);
  lv_obj_align(rate_label, LV_ALIGN_TOP_LEFT, 20, 120);
  
  update_rate_slider = lv_slider_create(settings_screen);
  lv_obj_set_size(update_rate_slider, 200, 20);
  lv_obj_align(update_rate_slider, LV_ALIGN_TOP_MID, 0, 120);
  lv_slider_set_range(update_rate_slider, 100, 5000);
  lv_slider_set_value(update_rate_slider, 1000, LV_ANIM_OFF);
  lv_obj_add_event_cb(update_rate_slider, update_rate_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  
  update_rate_label = lv_label_create(settings_screen);
  lv_label_set_text(update_rate_label, "1000 ms");
  lv_obj_set_style_text_color(update_rate_label, lv_color_hex(0xffffff), 0);
  lv_obj_align(update_rate_label, LV_ALIGN_TOP_MID, 0, 150);
  
  // Back button
  back_btn = lv_btn_create(settings_screen);
  lv_obj_set_size(back_btn, 100, 40);
  lv_obj_align(back_btn, LV_ALIGN_BOTTOM_MID, 0, -20);
  lv_obj_add_event_cb(back_btn, back_btn_event_cb, LV_EVENT_CLICKED, NULL);
  
  lv_obj_t *back_label = lv_label_create(back_btn);
  lv_label_set_text(back_label, "Back");
  lv_obj_center(back_label);
}

void update_temperature_display() {
  static int update_count = 0;
  update_count++;
  
  float object_temp = mlx.readObjectTempC();
  float ambient_temp = mlx.readAmbientTempC();
  
  // Debug every 10 updates
  if (update_count % 10 == 0) {
    Serial.printf("Update #%d - Object: %.1f°C, Ambient: %.1f°C\n", update_count, object_temp, ambient_temp);
  }
  
  // Convert to Fahrenheit if needed
  if (!use_celsius) {
    object_temp = object_temp * 9.0 / 5.0 + 32.0;
    ambient_temp = ambient_temp * 9.0 / 5.0 + 32.0;
  }
  
  // Update object temperature display
  char temp_str[20];
  snprintf(temp_str, sizeof(temp_str), "Object: %.1f°%c", object_temp, use_celsius ? 'C' : 'F');
  lv_label_set_text(object_label, temp_str);
  
  // Update ambient temperature display
  snprintf(temp_str, sizeof(temp_str), "Ambient: %.1f°%c", ambient_temp, use_celsius ? 'C' : 'F');
  lv_label_set_text(ambient_label, temp_str);
  
  // Update status
  lv_label_set_text(status_label, "Status: Reading");
  
  // Store temperature in history
  temp_history[history_index] = object_temp;
  temp_history_int[history_index] = (int32_t)(object_temp * 10); // Store as integer * 10 for precision
  history_index = (history_index + 1) % 50;
}

void update_chart() {
  // Update chart with temperature history
  lv_chart_set_ext_y_array(chart, chart_series, temp_history_int);
  lv_chart_refresh(chart);
}

// Event handlers
void temp_unit_btn_event_cb(lv_event_t *e) {
  use_celsius = !use_celsius;
  update_temperature_display();
}

void calibrate_btn_event_cb(lv_event_t *e) {
  lv_label_set_text(status_label, "Status: Calibrating...");
  // Add calibration logic here if needed
  delay(1000);
  lv_label_set_text(status_label, "Status: Calibrated");
}

void settings_btn_event_cb(lv_event_t *e) {
  lv_screen_load(settings_screen);
}

void back_btn_event_cb(lv_event_t *e) {
  lv_screen_load(main_screen);
}

void temp_unit_switch_event_cb(lv_event_t *e) {
  lv_obj_t *sw = (lv_obj_t*)lv_event_get_target(e);
  use_celsius = lv_obj_has_state(sw, LV_STATE_CHECKED);
  update_temperature_display();
}

void update_rate_slider_event_cb(lv_event_t *e) {
  lv_obj_t *slider = (lv_obj_t*)lv_event_get_target(e);
  update_rate = lv_slider_get_value(slider);
  
  char rate_str[20];
  snprintf(rate_str, sizeof(rate_str), "%d ms", update_rate);
  lv_label_set_text(update_rate_label, rate_str);
}