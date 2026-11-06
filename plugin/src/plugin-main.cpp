/**
 * Open Overlay Plugin - Main Entry Point
 * OBS Studio plugin for loading and rendering .oo overlay files
 * 
 * This plugin adds a custom source type to OBS that can load
 * Open Overlay .oo files and render them directly.
 */

#include <obs-module.h>
#include <obs.h>
#include <util/platform.h>
#include <memory>
#include <string>
#include <vector>
#include <map>

#include "openoverlay-source.h"
#include "oo-parser.h"

// Plugin info - used by OBS to identify the plugin
OBS_MODULE_USE_DEFAULT_LOCALE("open_overlay_plugin", "en-US")

// Global module reference
obs_module_t *g_module = nullptr;

// Plugin data directory
static std::string g_data_path;

// ============================================================================
// OBS Plugin Entry Points
// ============================================================================

extern "C" OBS_MODULE_EXPORT obs_module_t *obs_module(void)
{
    return g_module;
}

extern "C" OBS_MODULE_EXPORT void obs_module_free(void)
{
    open_overlay_source_shutdown();
    oo_parser_cleanup();
    g_module = nullptr;
}

extern "C" OBS_MODULE_EXPORT void obs_module_set_pointer(obs_module_t *module)
{
    g_module = module;
    
    char *data_path = obs_module_get_data_path(module);
    if (data_path) {
        g_data_path = data_path;
        bfree(data_path);
    }
}

extern "C" OBS_MODULE_EXPORT bool obs_module_load(void)
{
    blog(LOG_INFO, "===========================================");
    blog(LOG_INFO, "Open Overlay Plugin v%s - Loading...", 
             obs_module_get_version());
    blog(LOG_INFO, "===========================================");
    
    if (!g_data_path.empty()) {
        blog(LOG_INFO, "Data path: %s", g_data_path.c_str());
    }

    // Initialize the .oo parser
    oo_parser_init();
    
    // Register our custom source type with OBS
    open_overlay_source_register();

    blog(LOG_INFO, "Open Overlay Plugin loaded successfully!");

    return true;
}

extern "C" OBS_MODULE_EXPORT void obs_module_unload(void)
{
    blog(LOG_INFO, "Open Overlay Plugin - Unloading...");

    open_overlay_source_shutdown();
    oo_parser_cleanup();

    blog(LOG_INFO, "Open Overlay Plugin unloaded.");
}

// ============================================================================
// Module Info Accessors
// ============================================================================

extern "C" OBS_MODULE_EXPORT const char *obs_module_get_name(void)
{
    return obs_module_text("OpenOverlay");
}

const char *obs_module_get_description(void)
{
    return obs_module_text("Load and render .oo overlay files in OBS Studio");
}

extern "C" OBS_MODULE_EXPORT const char *obs_module_get_config_path(void)
{
    static std::string config_path;
    
    char *app_data = getenv("APPDATA");
    if (!app_data) {
        app_data = (char*)".";
    }
    
    config_path = std::string(app_data) + "/obs-studio/plugins/open_overlay_plugin";
    
    os_mkdirs(config_path.c_str());
    
    return config_path.c_str();
}

// ============================================================================
// Plugin Info Accessors
// ============================================================================

const char *open_overlay_plugin_get_version(void)
{
    return obs_module_get_version();
}

const char *open_overlay_plugin_get_data_path(void)
{
    return g_data_path.c_str();
}