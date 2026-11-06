/**
 * Open Overlay - Custom OBS Source Type
 * 
 * Defines the custom source type that OBS will use
 * to render .oo overlay files.
 */

#ifndef OPEN_OVERLAY_SOURCE_H
#define OPEN_OVERLAY_SOURCE_H

#include <obs.h>
#include <obs-source.h>
#include <obs-data.h>

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <mutex>

#include "oo-parser.h"

// Forward declarations
namespace oo {
    class Renderer;
}

// ============================================================================
// Source Context
// ============================================================================

struct OpenOverlaySource {
    // OBS source reference
    obs_source_t *self;
    
    // Widget data
    oo::Widget widget;
    std::string file_path;
    std::string json_content;
    
    // Render context
    std::unique_ptr<oo::Renderer> renderer;
    
    // State
    bool loaded;
    bool animating;
    double animation_time;
    
    // Settings
    int width;
    int height;
    bool enable_animations;
    double animation_speed;
    
    // Thread safety
    std::mutex mutex;
    
    OpenOverlaySource();
    ~OpenOverlaySource();
    
    // Load a .oo file
    bool loadFile(const std::string& path);
    
    // Load from JSON string
    bool loadJson(const std::string& json);
    
    // Reload current file
    bool reload();
    
    // Update dimensions
    void updateDimensions();
};

// ============================================================================
// OBS Source Callbacks
// ============================================================================

/**
 * Create the source
 */
void *open_overlay_create(obs_data_t *settings, obs_source_t *source);

/**
 * Destroy the source
 */
void open_overlay_destroy(void *data);

/**
 * Get the source name
 */
const char *open_overlay_get_name(void *data);

/**
 * Get default settings
 */
obs_data_t *open_overlay_get_defaults(const char *settings);

/**
 * Update source settings
 */
void open_overlay_update(void *data, obs_data_t *settings);

/**
 * Get source properties
 */
obs_properties_t *open_overlay_get_properties(void *data);

/**
 * Video render callback
 */
void open_overlay_video_render(void *data, gs_effect_t *effect);

/**
 * Audio render callback (not used, returns silence)
 */
uint32_t open_overlay_get_audio(void *data, uint64_t sys_time);

/**
 * Get width
 */
uint32_t open_overlay_get_width(void *data);

/**
 * Get height
 */
uint32_t open_overlay_get_height(void *data);

/**
 * Activate (shown)
 */
void open_overlay_activate(void *data);

/**
 * Deactivate (hidden)
 */
void open_overlay_deactivate(void *data);

/**
 * Video tick (for animations)
 */
void open_overlay_video_tick(void *data, float seconds);

// ============================================================================
// Registration Functions
// ============================================================================

/**
 * Register the source type with OBS
 */
void open_overlay_source_register(void);

/**
 * Shutdown and cleanup
 */
void open_overlay_source_shutdown(void);

// ============================================================================
// Utility Functions
// ============================================================================

/**
 * Get the data path for the plugin
 */
const char *get_plugin_data_path(void);

/**
 * Log a message
 */
void open_overlay_log(int level, const char *format, ...);

#endif // OPEN_OVERLAY_SOURCE_H