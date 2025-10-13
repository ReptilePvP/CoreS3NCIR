#ifndef __M5GFX_LVGL_H__
#define __M5GFX_LVGL_H__

#include "lvgl.h"
#include "M5Unified.h"
#include "M5GFX.h"

// Declare xGuiSemaphore as extern
extern SemaphoreHandle_t xGuiSemaphore;

void m5gfx_lvgl_init(void);

#endif  // __M5GFX_LVGL_H__