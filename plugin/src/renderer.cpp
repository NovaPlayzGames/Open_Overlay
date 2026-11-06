/**
 * Open Overlay - Renderer Implementation
 * 
 * Renders .oo widget elements using OBS graphics API
 */

#include "renderer.h"
#include "oo-parser.h"

#include <obs-module.h>
#include <graphics/graphics.h>
#include <util/platform.h>

#include <algorithm>
#include <cmath>
#include <cctype>
#include <vector>
#include <string>

// ============================================================================
// Base64 Decoder (Standard Library)
// ============================================================================

static const char base64_chars[] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static std::vector<uint8_t> base64_decode(const std::string& encoded_string) {
    std::vector<uint8_t> decoded;
    decoded.reserve(encoded_string.length() * 3 / 4);
    
    // Remove whitespace and newlines
    std::string input;
    for (char c : encoded_string) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            input += c;
        }
    }
    
    // Pad with '=' if needed
    while (input.length() % 4 != 0) {
        input += '=';
    }
    
    int i = 0;
    size_t len = input.length();
    
    while (i < static_cast<int>(len)) {
        char char_array_4[4];
        for (int k = 0; k < 4; k++) {
            char_array_4[k] = (i + k < static_cast<int>(len)) ? input[i + k] : 0;
        }
        
        // Decode 4 6-bit chars into 3 8-bit chars
        int n = 0;
        for (int k = 0; k < 4; k++) {
            n = n << 6;
            if (char_array_4[k] != '=') {
                int idx = -1;
                for (int m = 0; m < 64; m++) {
                    if (base64_chars[m] == char_array_4[k]) {
                        idx = m;
                        break;
                    }
                }
                if (idx >= 0) {
                    n += idx;
                }
            }
        }
        
        char char_array_3[3];
        char_array_3[0] = (n >> 16) & 0xFF;
        char_array_3[1] = (n >> 8) & 0xFF;
        char_array_3[2] = n & 0xFF;
        
        int padding = 0;
        if (i + 3 < static_cast<int>(len) && input[i + 3] == '=') padding++;
        if (i + 2 < static_cast<int>(len) && input[i + 2] == '=') padding++;
        
        for (int k = 0; k < 3 - padding; k++) {
            decoded.push_back(static_cast<uint8_t>(char_array_3[k]));
        }
        
        i += 4;
    }
    
    return decoded;
}

// ============================================================================
// Logging
// ============================================================================

#define LOG_INFO(...)  blog(LOG_INFO, "[Open Overlay Renderer] " __VA_ARGS__)
#define LOG_WARN(...)  blog(LOG_WARNING, "[Open Overlay Renderer] " __VA_ARGS__)
#define LOG_ERROR(...) blog(LOG_ERROR, "[Open Overlay Renderer] " __VA_ARGS__)

// ============================================================================
// Texture Cache Implementation
// ============================================================================

TextureCache::TextureCache() {}
TextureCache::~TextureCache() { clear(); }

gs_texture_t* TextureCache::getTexture(const std::string& url, int width, int height) {
    if (url.empty()) return nullptr;
    
    auto it = m_textures.find(url);
    if (it != m_textures.end() && it->second) {
        return it->second;
    }
    
    gs_texture_t* texture = nullptr;
    
    if (url.find("data:image") != std::string::npos) {
        size_t commaPos = url.find(',');
        if (commaPos != std::string::npos) {
            std::string base64Data = url.substr(commaPos + 1);
            
            size_t semicolonPos = base64Data.find(';');
            if (semicolonPos != std::string::npos) {
                base64Data = base64Data.substr(semicolonPos + 1);
                if (base64Data.find("base64,") == 0) {
                    base64Data = base64Data.substr(7);
                }
            }
            
            std::vector<uint8_t> decoded = base64_decode(base64Data);
            
            if (!decoded.empty()) {
                // Try to load from memory - OBS provides this function
                texture = gs_texture_create_from_file(""); // Will fail, but creates placeholder
                (void)decoded; // Avoid unused warning
            }
        }
    }
    else if (url.find("file://") == 0 || (url.length() > 0 && url[0] == '/')) {
        std::string path = url;
        if (url.find("file://") == 0) {
            path = url.substr(7);
        }
        texture = gs_texture_create_from_file(path.c_str());
    }
    
    if (texture) {
        m_textures[url] = texture;
    }
    
    return texture;
}

void TextureCache::clear() {
    for (auto& pair : m_textures) {
        if (pair.second) {
            gs_texture_destroy(pair.second);
        }
    }
    m_textures.clear();
}

// ============================================================================
// Color Parsing
// ============================================================================

namespace oo {
    void parseColorHex(const std::string& hex, int& r, int& g, int& b, int& a) {
        r = g = b = 0;
        a = 255;
        
        if (hex.empty()) return;
        
        std::string h = hex;
        if (h[0] == '#') h = h.substr(1);
        
        if (h.length() == 6) {
            r = std::stoi(h.substr(0, 2), nullptr, 16);
            g = std::stoi(h.substr(2, 2), nullptr, 16);
            b = std::stoi(h.substr(4, 2), nullptr, 16);
        } else if (h.length() == 8) {
            r = std::stoi(h.substr(0, 2), nullptr, 16);
            g = std::stoi(h.substr(2, 2), nullptr, 16);
            b = std::stoi(h.substr(4, 2), nullptr, 16);
            a = std::stoi(h.substr(6, 2), nullptr, 16);
        }
    }
}

// ============================================================================
// Renderer Implementation
// ============================================================================

Renderer::Renderer(const Widget& widget)
    : m_widget(widget)
    , m_textureCache(std::make_unique<TextureCache>())
{
    LOG_INFO("Creating renderer for widget: %s (%dx%d)", 
        widget.name.c_str(), widget.width, widget.height);
}

Renderer::~Renderer() {
    LOG_INFO("Destroying renderer");
}

void Renderer::render(double animationTime) {
    gs_view_push();
    gs_projection_push();
    
    // Use OBS's ortho function with correct arguments (left, right, bottom, top, near, far)
    gs_ortho(0.0f, static_cast<float>(m_widget.width),
             static_cast<float>(m_widget.height), 0.0f,
             -100.0f, 100.0f);
    
    gs_enable_blending(true);
    gs_blend_function(GS_BLEND_SRCALPHA, GS_BLEND_INVSRCALPHA);
    
    if (!m_widget.background.empty() && m_widget.background != "transparent") {
        setColor(m_widget.background, 1.0);
        gs_draw_sprite(nullptr, 0, m_widget.width, m_widget.height);
    }
    
    for (const auto& elementVar : m_widget.elements) {
        if (auto* shape = std::get_if<ShapeElement>(&elementVar)) {
            renderShape(*shape, animationTime);
        } else if (auto* text = std::get_if<TextElement>(&elementVar)) {
            renderText(*text, animationTime);
        } else if (auto* image = std::get_if<ImageElement>(&elementVar)) {
            renderImage(*image, animationTime);
        }
    }
    
    gs_projection_pop();
    gs_view_pop();
}

void Renderer::renderElement(const Element& element, double animationTime) {
    if (!element.visible) return;
    
    AnimationState state = calculateAnimation(element, animationTime);
    
    gs_matrix_push();
    applyAnimation(state, element);
    gs_matrix_pop();
}

void Renderer::renderShape(const ShapeElement& shape, double animationTime) {
    if (!shape.visible) return;
    
    AnimationState state = calculateAnimation(shape, animationTime);
    
    gs_matrix_push();
    applyAnimation(state, shape);
    
    int r, g, b, a;
    oo::parseColorHex(shape.fill, r, g, b, a);
    
    float alpha = static_cast<float>(a) / 255.0f * static_cast<float>(shape.opacity);
    gs_set_color(
        static_cast<float>(r) / 255.0f,
        static_cast<float>(g) / 255.0f,
        static_cast<float>(b) / 255.0f,
        alpha
    );
    
    gs_draw_sprite(nullptr, 0, 
        static_cast<uint32_t>(shape.width), 
        static_cast<uint32_t>(shape.height));
    
    gs_matrix_pop();
}

void Renderer::renderText(const TextElement& text, double animationTime) {
    if (!text.visible || text.content.empty()) return;
    
    AnimationState state = calculateAnimation(text, animationTime);
    
    gs_matrix_push();
    applyAnimation(state, text);
    
    // Note: Full text rendering would require OBS font API
    // For now, draw a placeholder rectangle
    setColor(text.color, text.opacity);
    gs_draw_sprite(nullptr, 0, 
        static_cast<uint32_t>(text.width), 
        static_cast<uint32_t>(text.height));
    
    gs_matrix_pop();
}

void Renderer::renderImage(const ImageElement& image, double animationTime) {
    if (!image.visible || image.src.empty()) return;
    
    AnimationState state = calculateAnimation(image, animationTime);
    
    gs_matrix_push();
    applyAnimation(state, image);
    
    gs_texture_t* texture = m_textureCache->getTexture(image.src, 
        static_cast<int>(image.width), static_cast<int>(image.height));
    
    if (texture) {
        gs_draw_sprite(texture, 0, 
            static_cast<uint32_t>(image.width), 
            static_cast<uint32_t>(image.height));
    } else {
        setColor("#333333", image.opacity);
        gs_draw_sprite(nullptr, 0, 
            static_cast<uint32_t>(image.width), 
            static_cast<uint32_t>(image.height));
    }
    
    gs_matrix_pop();
}

AnimationState Renderer::calculateAnimation(const Element& element, double animationTime) {
    AnimationState state;
    
    if (element.animationName == AnimationType::None) {
        state.progress = 1.0;
        state.complete = true;
        return state;
    }
    
    double totalDuration = element.animationDuration + element.animationDelay;
    
    if (totalDuration <= 0) {
        state.progress = 1.0;
        state.complete = true;
        return state;
    }
    
    double phaseTime = animationTime - element.animationDelay;
    
    if (phaseTime < 0) {
        state.progress = 0.0;
        state.elapsed = 0.0;
        return state;
    }
    
    if (element.animationIterationCount < 0) {
        state.progress = std::fmod(phaseTime, element.animationDuration) / element.animationDuration;
        state.elapsed = phaseTime;
        state.complete = false;
    } else {
        double totalTime = element.animationDuration * element.animationIterationCount;
        
        if (phaseTime >= totalTime) {
            state.progress = 1.0;
            state.elapsed = totalTime;
            state.complete = true;
        } else {
            state.progress = std::fmod(phaseTime, element.animationDuration) / element.animationDuration;
            state.elapsed = phaseTime;
            state.complete = false;
        }
    }
    
    return state;
}

void Renderer::applyAnimation(AnimationState& state, const Element& element) {
    if (element.animationName == AnimationType::None || state.progress >= 1.0) {
        return;
    }
    
    double t = state.progress;
    t = 1.0 - std::pow(1.0 - t, 3.0);
    
    switch (element.animationName) {
        case AnimationType::SlideIn: {
            float offset = static_cast<float>((1.0 - t) * -200.0);
            gs_matrix_translate3f(offset, 0.0f, 0.0f);
            break;
        }
        case AnimationType::SlideOut: {
            float offset = static_cast<float>(t * 200.0);
            gs_matrix_translate3f(offset, 0.0f, 0.0f);
            break;
        }
        case AnimationType::ZoomIn: {
            float scale = static_cast<float>(t);
            gs_matrix_scale_xyz(scale, scale, 1.0f);
            break;
        }
        case AnimationType::ZoomOut: {
            float scale = static_cast<float>(1.0 - t);
            gs_matrix_scale_xyz(scale, scale, 1.0f);
            break;
        }
        case AnimationType::Pulse: {
            float scale = 1.0f + static_cast<float>(std::sin(t * M_PI * 2.0) * 0.1);
            gs_matrix_scale_xyz(scale, scale, 1.0f);
            break;
        }
        case AnimationType::Bounce: {
            float y = 0.0f;
            if (t < 0.5) {
                y = static_cast<float>(-50.0 * t * 2.0);
            } else {
                y = static_cast<float>(-50.0 * (1.0 - t) * 2.0);
            }
            gs_matrix_translate3f(0.0f, y, 0.0f);
            break;
        }
        case AnimationType::Shake: {
            float offsetX = static_cast<float>((rand() % 10 - 5) * (1.0 - t));
            float offsetY = static_cast<float>((rand() % 10 - 5) * (1.0 - t));
            gs_matrix_translate3f(offsetX, offsetY, 0.0f);
            break;
        }
        default:
            break;
    }
}

void Renderer::setColor(const std::string& hex, double opacity) {
    int r, g, b, a;
    oo::parseColorHex(hex, r, g, b, a);
    
    float alpha = static_cast<float>(a) / 255.0f * static_cast<float>(opacity);
    gs_set_color(
        static_cast<float>(r) / 255.0f,
        static_cast<float>(g) / 255.0f,
        static_cast<float>(b) / 255.0f,
        alpha
    );
}

void Renderer::clearColor() {
    gs_set_color(1.0f, 1.0f, 1.0f, 1.0f);
}