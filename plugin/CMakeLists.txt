cmake_minimum_required(VERSION 3.16)
project(OpenOverlayPlugin)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OBS Studio
if(NOT OBS_STUDIO_PATH)
    set(OBS_STUDIO_PATH "C:/obs-studio" CACHE PATH "Path to OBS Studio source")
endif()

# OBS Dependencies
set(OBS_DEPS_PATH "${OBS_STUDIO_PATH}/.deps/obs-deps-2025-08-23-x64")
set(OBS_DEPS_INCLUDE "${OBS_DEPS_PATH}/include")

# Include directories - order matters!
# build/config must come FIRST because obs-config.h includes generated headers
include_directories(
    # OBS build config (contains generated headers)
    ${OBS_STUDIO_PATH}/build/config
    
    # OBS libobs (main library) - this is the critical one
    ${OBS_STUDIO_PATH}/libobs
    
    # OBS dependencies
    ${OBS_DEPS_INCLUDE}
    ${OBS_DEPS_INCLUDE}/libavutil
    ${OBS_DEPS_INCLUDE}/libpng16
    
    # pthread headers
    ${OBS_STUDIO_PATH}/deps/w32-pthreads
)

# Source files
set(SOURCE_FILES
    src/plugin-main.cpp
    src/oo-parser.cpp
    src/openoverlay-source.cpp
    src/renderer.cpp
)

# Create shared library
add_library(open_overlay_plugin SHARED ${SOURCE_FILES})

# Windows-specific settings
if(WIN32)
    target_link_libraries(open_overlay_plugin ws2_32 winmm)
    set_target_properties(open_overlay_plugin PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin/Release"
        OUTPUT_NAME "open_overlay_plugin"
    )
endif()

# Install target
install(TARGETS open_overlay_plugin
    RUNTIME DESTINATION "obs-plugins/64bit"
)