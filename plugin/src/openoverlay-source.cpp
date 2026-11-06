/**
 * Open Overlay - Custom OBS Source Type Implementation
 */

#include "openoverlay-source.h"
#include "renderer.h"
#include "oo-parser.h"

#include <obs-module.h>
#include <util/platform.h>
#include <graphics/graphics.h>

#include <algorithm>
#include <cstring>
#include <thread>

// ============================================================================
// Logging
// ============================================================================

#define LOG_INFO(...)  blog(LOG_INFO, "[Open Overlay] " __VA_ARGS__)
#define LOG_WARN(...)  blog(LOG_WARNING, "[Open Overlay] " __VA_ARGS__)
#define LOG_ERROR(...) blog(LOG_ERROR, "[Open Overlay] " __VA_ARGS__)

// ============================================================================
// Source Implementation
// ============================================================================

OpenOverlaySource::OpenOverlaySource()
    : self(nullptr)
    , loaded(false)
    , animating(false)
    , animation_time(0.0)
    , width(1920)
    , height(1080)
    , enable_animations(true)
    , animation_speed(1.0)
{
}

OpenOverlaySource::~OpenOverlaySource()
{
    renderer.reset();
}

// Load a .oo file
bool OpenOverlaySource::loadFile(const std::string& path)
{
    std::lock_guard<std::mutex> lock(mutex);
    
    file_path = path;
    
    auto result = oo::OOParser::parseFile(path);
    
    if (!result.success) {
        LOG_ERROR("Failed to load .oo file: %s", result.error.c_str());
        loaded = false;
        return false;
    }
    
    widget = result.widget;
    
    renderer = std::make_unique<oo::Renderer>(widget);
    
    loaded = true;
    updateDimensions();
    
    LOG_INFO("Loaded .oo file: %s (%dx%d)", 
        widget.name.c_str(), width, height);
    
    return true;
}

// Load from JSON string
bool OpenOverlaySource::loadJson(const std::string& json)
{
    std::lock_guard<std::mutex> lock(mutex);
    
    json_content = json;
    
    auto result = oo::OOParser::parseString(json);
    
    if (!result.success) {
        LOG_ERROR("Failed to parse .oo JSON: %s", result.error.c_str());
        loaded = false;
        return false;
    }
    
    widget = result.widget;
    
    renderer = std::make_unique<oo::Renderer>(widget);
    
    loaded = true;
    updateDimensions();
    
    LOG_INFO("Loaded .oo from JSON: %s (%dx%d)", 
        widget.name.c_str(), width, height);
    
    return true;
}

bool OpenOverlaySource::reload()
{
    if (!file_path.empty()) {
        return loadFile(file_path);
    } else if (!json_content.empty()) {
        return loadJson(json_content);
    }
    return false;
}

void OpenOverlaySource::updateDimensions()
{
    width = widget.width > 0 ? widget.width : 1920;
    height = widget.height > 0 ? widget.height : 1080;
}

// ============================================================================
// OBS Source Callbacks
// ============================================================================

void *open_overlay_create(obs_data_t *settings, obs_source_t *source)
{
    LOG_INFO("Creating Open Overlay source...");
    
    auto *data = new OpenOverlaySource();
    data->self = source;
    
    obs_source_set_flags(source, OBS_SOURCE_VIDEO | OBS_SOURCE_CUSTOM_DRAW);
    
    open_overlay_update(data, settings);
    
    LOG_INFO("Open Overlay source created successfully");
    
    return data;
}

void open_overlay_destroy(void *data)
{
    if (!data) return;
    
    LOG_INFO("Destroying Open Overlay source...");
    
    auto *source = static_cast<OpenOverlaySource*>(data);
    delete source;
}

const char *open_overlay_get_name(void *data)
{
    return obs_module_text("Open Overlay");
}

obs_data_t *open_overlay_get_defaults(const char *settings)
{
    obs_data_t *defaults = obs_data_create();
    
    obs_data_set_string(defaults, "file", "");
    obs_data_set_string(defaults, "json", "");
    obs_data_set_bool(defaults, "enable_animations", true);
    obs_data_set_double(defaults, "animation_speed", 1.0);
    obs_data_set_int(defaults, "width", 1920);
    obs_data_set_int(defaults, "height", 1080);
    
    return defaults;
}

void open_overlay_update(void *data, obs_data_t *settings)
{
    if (!data) return;
    
    auto *source = static_cast<OpenOverlaySource*>(data);
    std::lock_guard<std::mutex> lock(source->mutex);
    
    const char *file = obs_data_get_string(settings, "file");
    const char *json = obs_data_get_string(settings, "json");
    source->enable_animations = obs_data_get_bool(settings, "enable_animations");
    source->animation_speed = obs_data_get_double(settings, "animation_speed");
    
    source->width = static_cast<int>(obs_data_get_int(settings, "width"));
    source->height = static_cast<int>(obs_data_get_int(settings, "height"));
    
    if (file && strlen(file) > 0) {
        source->loadFile(file);
    } else if (json && strlen(json) > 0) {
        source->loadJson(json);
    } else {
        LOG_WARN("No file or JSON provided for Open Overlay source");
    }
}

obs_properties_t *open_overlay_get_properties(void *data)
{
    obs_properties_t *props = obs_properties_create();
    
    obs_properties_add_path(props, "file", 
        obs_module_text("Overlay File"),
        OBS_PATH_FILE, 
        "Open Overlay Files (*.oo);;All Files (*.*)",
        NULL);
    
    obs_properties_add_text(props, "json",
        obs_module_text("JSON Content"),
        OBS_TEXT_DEFAULT);
    
    obs_properties_add_bool(props, "enable_animations",
        obs_module_text("Enable Animations"));
    
    obs_properties_add_float(props, "animation_speed",
        obs_module_text("Animation Speed"), 0.1, 10.0, 0.1);
    
    obs_properties_add_int(props, "width",
        obs_module_text("Width"), 1, 7680, 1);
    
    obs_properties_add_int(props, "height",
        obs_module_text("Height"), 1, 4320, 1);
    
    obs_properties_add_button(props, "reload", 
        obs_module_text("Reload Overlay"),
        [](obs_properties_t *props, obs_property_t *property, void *data) {
            if (data) {
                auto *source = static_cast<OpenOverlaySource*>(data);
                source->reload();
            }
            return true;
        });
    
    return props;
}

void open_overlay_video_render(void *data, gs_effect_t *effect)
{
    if (!data) return;
    
    auto *source = static_cast<OpenOverlaySource*>(data);
    
    if (!source->loaded || !source->renderer) {
        gs_effect_t *default_effect = gs_get_effect();
        gs_technique_t *tech = gs_effect_get_technique(default_effect, "Draw");
        
        if (tech) {
            gs_technique_begin(tech);
            gs_technique_begin_pass(tech, 0);
            
            struct vec4 color;
            vec4_from_rgba(&color, 0xFF333333);
            gs_effect_set_vec4(gs_effect_get_param_by_name(default_effect, "color"), &color);
            
            gs_draw_sprite(0, 0, source->width, source->height);
            
            gs_technique_end_pass(tech);
            gs_technique_end(tech);
        }
        return;
    }
    
    {
        std::lock_guard<std::mutex> lock(source->mutex);
        source->renderer->render(source->animation_time);
    }
}

uint32_t open_overlay_get_audio(void *data, uint64_t sys_time)
{
    return 0;
}

uint32_t open_overlay_get_width(void *data)
{
    if (!data) return 1920;
    
    auto *source = static_cast<OpenOverlaySource*>(data);
    return static_cast<uint32_t>(source->width);
}

uint32_t open_overlay_get_height(void *data)
{
    if (!data) return 1080;
    
    auto *source = static_cast<OpenOverlaySource*>(data);
    return static_cast<uint32_t>(source->height);
}

void open_overlay_activate(void *data)
{
    if (!data) return;
    
    auto *source = static_cast<OpenOverlaySource*>(data);
    source->animating = true;
}

void open_overlay_deactivate(void *data)
{
    if (!data) return;
    
    auto *source = static_cast<OpenOverlaySource*>(data);
    source->animating = false;
}

void open_overlay_video_tick(void *data, float seconds)
{
    if (!data) return;
    
    auto *source = static_cast<OpenOverlaySource*>(data);
    
    if (source->animating && source->enable_animations) {
        source->animation_time += seconds * source->animation_speed;
    }
}

// ============================================================================
// Source Type Definition
// ============================================================================

static obs_source_info g_open_overlay_source = {
    .id = "open_overlay",
    .type = OBS_SOURCE_TYPE_INPUT,
    .output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_CUSTOM_DRAW,
    .get_name = open_overlay_get_name,
    .create = open_overlay_create,
    .destroy = open_overlay_destroy,
    .get_defaults = open_overlay_get_defaults,
    .get_properties = open_overlay_get_properties,
    .update = open_overlay_update,
    .video_render = open_overlay_video_render,
    .video_tick = open_overlay_video_tick,
    .get_width = open_overlay_get_width,
    .get_height = open_overlay_get_height,
    .activate = open_overlay_activate,
    .deactivate = open_overlay_deactivate,
    .icon_type = OBS_ICON_TYPE_IMAGE,
};

// ============================================================================
// Registration Functions
// ============================================================================

void open_overlay_source_register(void)
{
    LOG_INFO("Registering Open Overlay source type...");
    
    obs_register_source(&g_open_overlay_source);
    
    LOG_INFO("Open Overlay source type registered successfully");
}

void open_overlay_source_shutdown(void)
{
    LOG_INFO("Shutting down Open Overlay source...");
}

// ============================================================================
// Module Information
// ============================================================================

extern "C" {
    
    const char *obs_module_name(void)
    {
        return "Open Overlay";
    }
    
    const char *obs_module_description(void)
    {
        return "Load and render .oo overlay files in OBS Studio";
    }
    
}