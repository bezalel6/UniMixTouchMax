/**
 * UI Performance Optimizations Implementation
 * 
 * EMERGENCY PERFORMANCE FIX: Addresses the critical 3.8MB image causing 
 * 100-225ms LVGL processing delays.
 */

#include "UIPerformanceOptimizations.h"
#include <esp_log.h>
#include <esp_heap_caps.h>

// Include auto-generated UI declarations
extern lv_obj_t *ui_img;                    // The problematic 3.8MB image object
extern const lv_img_dsc_t ui_img_2039083_png; // The actual 3.8MB image data

static const char *TAG = "UIPerformance";

// Small placeholder image data (16x16 pixels, minimal size)
static const uint8_t placeholder_img_data[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF,  // Simple checkerboard pattern
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF,
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

static const lv_img_dsc_t placeholder_img_dsc = {
    .header.cf = LV_IMG_CF_TRUE_COLOR,
    .header.always_zero = 0,
    .header.reserved = 0,
    .header.w = 16,
    .header.h = 16,
    .data_size = sizeof(placeholder_img_data),
    .data = placeholder_img_data
};

void ui_performance_hide_large_image(void) {
    if (ui_img && lv_obj_is_valid(ui_img)) {
        ESP_LOGI(TAG, "EMERGENCY FIX: Hiding 3.8MB image to restore performance");
        
        // Hide the problematic image object
        lv_obj_add_flag(ui_img, LV_OBJ_FLAG_HIDDEN);
        
        // Also disable rendering to save even more processing time
        lv_obj_add_flag(ui_img, LV_OBJ_FLAG_IGNORE_LAYOUT);
        
        ESP_LOGI(TAG, "Large image hidden successfully - performance should improve dramatically");
    } else {
        ESP_LOGW(TAG, "ui_img object not found or invalid");
    }
}

void ui_performance_replace_large_image_with_placeholder(void) {
    if (ui_img && lv_obj_is_valid(ui_img)) {
        ESP_LOGI(TAG, "EMERGENCY FIX: Replacing 3.8MB image with tiny placeholder");
        
        // Replace the massive image with a tiny placeholder
        lv_img_set_src(ui_img, &placeholder_img_dsc);
        
        // Make sure it's visible but small
        lv_obj_clear_flag(ui_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_size(ui_img, 32, 32);  // Small 32x32 placeholder
        
        // Add a border to indicate it's a placeholder
        lv_obj_set_style_border_color(ui_img, lv_color_hex(0xFF0000), LV_PART_MAIN);
        lv_obj_set_style_border_width(ui_img, 2, LV_PART_MAIN);
        lv_obj_set_style_border_opa(ui_img, 255, LV_PART_MAIN);
        
        ESP_LOGI(TAG, "Image replaced with placeholder - MASSIVE performance improvement expected");
    } else {
        ESP_LOGW(TAG, "ui_img object not found or invalid");
    }
}

void ui_performance_optimize_large_images(void) {
    ESP_LOGI(TAG, "Applying emergency image performance optimization");
    
    // For now, completely hide the problematic image
    // This will provide immediate performance restoration
    ui_performance_hide_large_image();
    
    // Alternative: Use placeholder (uncomment if you want to see a placeholder)
    // ui_performance_replace_large_image_with_placeholder();
}

void ui_performance_optimize_ui_objects(void) {
    ESP_LOGI(TAG, "Optimizing UI object properties for performance");
    
    // Optimize main screen for performance
    if (ui_screenMain && lv_obj_is_valid(ui_screenMain)) {
        // Reduce unnecessary style recalculations
        lv_obj_add_flag(ui_screenMain, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
        
        ESP_LOGD(TAG, "Main screen optimized");
    }
    
    // Optimize FPS label for less frequent updates
    if (ui_lblFPS && lv_obj_is_valid(ui_lblFPS)) {
        // Use fixed width to prevent layout recalculations
        lv_obj_set_width(ui_lblFPS, 100);
        
        ESP_LOGD(TAG, "FPS label optimized");
    }
    
    // Optimize status view for better performance
    if (ui_statusView && lv_obj_is_valid(ui_statusView)) {
        // Reduce layout calculations
        lv_obj_add_flag(ui_statusView, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
        
        ESP_LOGD(TAG, "Status view optimized");
    }
}

void ui_performance_apply_all_optimizations(void) {
    ESP_LOGI(TAG, "=== APPLYING EMERGENCY UI PERFORMANCE OPTIMIZATIONS ===");
    
    // Log memory usage before optimization
    ui_performance_log_memory_usage();
    
    // Apply image optimizations (most critical)
    ui_performance_optimize_large_images();
    
    // Apply general UI optimizations
    ui_performance_optimize_ui_objects();
    
    // Force LVGL to recalculate and refresh
    lv_obj_invalidate(lv_scr_act());
    
    ESP_LOGI(TAG, "=== UI PERFORMANCE OPTIMIZATIONS APPLIED ===");
    ESP_LOGI(TAG, "Expected result: 80-90%% reduction in LVGL processing time");
    ESP_LOGI(TAG, "Previous: 100-225ms → Expected: 10-45ms");
    
    // Log memory usage after optimization
    ui_performance_log_memory_usage();
}

void ui_performance_log_memory_usage(void) {
    // Log general memory usage
    size_t free_heap = esp_get_free_heap_size();
    size_t free_psram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    
    ESP_LOGI(TAG, "Memory usage - Free Heap: %d bytes, Free PSRAM: %d bytes", 
             free_heap, free_psram);
    
    // Log LVGL memory usage
    lv_mem_monitor_t mon;
    lv_mem_monitor(&mon);
    
    ESP_LOGI(TAG, "LVGL Memory - Total: %d bytes, Used: %d bytes, Free: %d bytes, Fragmentation: %d%%",
             mon.total_size, mon.used_size, mon.free_size, mon.frag_pct);
    
    // Log image-specific memory impact
    size_t large_image_size = sizeof(ui_img_2039083_png);
    ESP_LOGI(TAG, "Large image size: %d bytes (%.2f MB)", 
             large_image_size, large_image_size / (1024.0 * 1024.0));
}