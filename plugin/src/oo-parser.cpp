/**
 * Open Overlay - Parser Implementation
 * 
 * Parses .oo overlay files and JSON content into Widget objects
 */

#include "oo-parser.h"

#include <obs-module.h>
#include <util/platform.h>

#include <algorithm>
#include <cstring>
#include <fstream>
#include <sstream>

// ============================================================================
// Logging
// ============================================================================

#define LOG_INFO(...)  blog(LOG_INFO, "[Open Overlay Parser] " __VA_ARGS__)
#define LOG_WARN(...)  blog(LOG_WARNING, "[Open Overlay Parser] " __VA_ARGS__)
#define LOG_ERROR(...) blog(LOG_ERROR, "[Open Overlay Parser] " __VA_ARGS__)

// ============================================================================
// Global State
// ============================================================================

namespace oo {

bool OOParser::g_parserInitialized = false;

// ============================================================================
// Parser Initialization
// ============================================================================

void OOParser::init() {
    g_parserInitialized = true;
    LOG_INFO("OOParser initialized");
}

void OOParser::cleanup() {
    g_parserInitialized = false;
    LOG_INFO("OOParser cleaned up");
}

// ============================================================================
// Animation Type Parsing
// ============================================================================

AnimationType parseAnimationType(const std::string& name) {
    std::string lower = name;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    
    if (lower == "fadein" || lower == "fade_in") return AnimationType::FadeIn;
    if (lower == "fadeout" || lower == "fade_out") return AnimationType::FadeOut;
    if (lower == "slidein" || lower == "slide_in") return AnimationType::SlideIn;
    if (lower == "slideout" || lower == "slide_out") return AnimationType::SlideOut;
    if (lower == "zoomin" || lower == "zoom_in") return AnimationType::ZoomIn;
    if (lower == "zoomout" || lower == "zoom_out") return AnimationType::ZoomOut;
    if (lower == "pulse") return AnimationType::Pulse;
    if (lower == "bounce") return AnimationType::Bounce;
    if (lower == "shake") return AnimationType::Shake;
    
    return AnimationType::None;
}

// ============================================================================
// Shape Type Parsing
// ============================================================================

ShapeType parseShapeType(const std::string& type) {
    std::string lower = type;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    
    if (lower == "circle") return ShapeType::Circle;
    if (lower == "triangle") return ShapeType::Triangle;
    
    return ShapeType::Rectangle;
}

// ============================================================================
// Text Align Parsing
// ============================================================================

TextAlign parseTextAlign(const std::string& align) {
    std::string lower = align;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    
    if (lower == "left") return TextAlign::Left;
    if (lower == "right") return TextAlign::Right;
    
    return TextAlign::Center;
}

// ============================================================================
// Font Weight Parsing
// ============================================================================

FontWeight parseFontWeight(const std::string& weight) {
    std::string lower = weight;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    
    if (lower == "bold") return FontWeight::Bold;
    
    return FontWeight::Normal;
}

// ============================================================================
// Widget Validation
// ============================================================================

bool OOParser::validateWidget(const Widget& widget) {
    if (widget.width <= 0 || widget.height <= 0) {
        LOG_ERROR("Invalid widget dimensions: %dx%d", widget.width, widget.height);
        return false;
    }
    return true;
}

// ============================================================================
// JSON Parsing Helpers
// ============================================================================

static std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

static std::string getJsonValue(const std::string& json, const std::string& key) {
    std::string searchKey = "\"" + key + "\"";
    size_t pos = json.find(searchKey);
    if (pos == std::string::npos) return "";
    
    pos = json.find(":", pos);
    if (pos == std::string::npos) return "";
    
    pos++;
    while (pos < json.length() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    
    size_t start = pos;
    char quote = json[pos];
    
    if (quote == '"') {
        pos++;
        start = pos;
        pos = json.find('"', pos);
        if (pos == std::string::npos) return "";
        return json.substr(start, pos - start);
    }
    
    // Number or boolean
    size_t end = pos;
    while (end < json.length() && json[end] != ',' && json[end] != '}' && json[end] != ' ') end++;
    return json.substr(pos, end - pos);
}

static bool getJsonBool(const std::string& json, const std::string& key, bool defaultVal) {
    std::string value = getJsonValue(json, key);
    if (value.empty()) return defaultVal;
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    return value == "true";
}

static double getJsonNumber(const std::string& json, const std::string& key, double defaultVal) {
    std::string value = getJsonValue(json, key);
    if (value.empty()) return defaultVal;
    try {
        return std::stod(value);
    } catch (...) {
        return defaultVal;
    }
}

static int getJsonInt(const std::string& json, const std::string& key, int defaultVal) {
    std::string value = getJsonValue(json, key);
    if (value.empty()) return defaultVal;
    try {
        return std::stoi(value);
    } catch (...) {
        return defaultVal;
    }
}

static std::string extractArrayElement(const std::string& json, int index) {
    int depth = 0;
    int current = 0;
    bool inString = false;
    char prevChar = 0;
    
    for (size_t i = 0; i < json.length(); i++) {
        char c = json[i];
        
        if (c == '"' && prevChar != '\\') {
            inString = !inString;
        }
        
        if (!inString) {
            if (c == '{') {
                if (depth == 0) current++;
                if (current == index + 1 && depth == 0) {
                    size_t start = i;
                    depth = 1;
                    size_t j = i + 1;
                    while (j < json.length() && depth > 0) {
                        if (json[j] == '{' && !inString) depth++;
                        if (json[j] == '}' && !inString) depth--;
                        if (json[j] == '"' && prevChar != '\\') inString = !inString;
                        prevChar = json[j];
                        j++;
                    }
                    return json.substr(start, j - start);
                }
                depth++;
            }
        }
        prevChar = c;
    }
    return "";
}

void OOParser::processElementJson(const std::string& json, Widget& widget) {
    std::string type = getJsonValue(json, "type");
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);
    
    if (type == "shape" || type == "rectangle" || type == "circle" || type == "triangle") {
        ShapeElement element;
        element.id = getJsonValue(json, "id");
        element.x = getJsonNumber(json, "x", 0);
        element.y = getJsonNumber(json, "y", 0);
        element.width = getJsonNumber(json, "width", 100);
        element.height = getJsonNumber(json, "height", 100);
        element.fill = getJsonValue(json, "fill");
        if (element.fill.empty()) element.fill = "#FFFFFF";
        element.borderRadius = getJsonNumber(json, "borderRadius", 0);
        element.opacity = getJsonNumber(json, "opacity", 1.0);
        element.visible = getJsonBool(json, "visible", true);
        element.shapeType = parseShapeType(getJsonValue(json, "shapeType"));
        
        widget.elements.push_back(element);
    }
    else if (type == "text") {
        TextElement element;
        element.id = getJsonValue(json, "id");
        element.x = getJsonNumber(json, "x", 0);
        element.y = getJsonNumber(json, "y", 0);
        element.width = getJsonNumber(json, "width", 200);
        element.height = getJsonNumber(json, "height", 50);
        element.content = getJsonValue(json, "content");
        element.fontSize = getJsonNumber(json, "fontSize", 16);
        element.color = getJsonValue(json, "color");
        if (element.color.empty()) element.color = "#000000";
        element.fontFamily = getJsonValue(json, "fontFamily");
        if (element.fontFamily.empty()) element.fontFamily = "Arial";
        element.textAlign = parseTextAlign(getJsonValue(json, "textAlign"));
        element.fontWeight = parseFontWeight(getJsonValue(json, "fontWeight"));
        element.opacity = getJsonNumber(json, "opacity", 1.0);
        element.visible = getJsonBool(json, "visible", true);
        
        widget.elements.push_back(element);
    }
    else if (type == "image") {
        ImageElement element;
        element.id = getJsonValue(json, "id");
        element.x = getJsonNumber(json, "x", 0);
        element.y = getJsonNumber(json, "y", 0);
        element.width = getJsonNumber(json, "width", 100);
        element.height = getJsonNumber(json, "height", 100);
        element.src = getJsonValue(json, "src");
        element.alt = getJsonValue(json, "alt");
        element.opacity = getJsonNumber(json, "opacity", 1.0);
        element.visible = getJsonBool(json, "visible", true);
        
        widget.elements.push_back(element);
    }
}

// ============================================================================
// Parse File
// ============================================================================

ParseResult OOParser::parseFile(const std::string& filepath) {
    ParseResult result;
    
    if (!g_parserInitialized) {
        init();
    }
    
    if (filepath.empty()) {
        result.error = "Empty filepath";
        return result;
    }
    
    std::string content = loadFileToString(filepath);
    if (content.empty()) {
        result.error = "Failed to read file: " + filepath;
        return result;
    }
    
    return parseString(content);
}

// ============================================================================
// Parse String
// ============================================================================

ParseResult OOParser::parseString(const std::string& json) {
    ParseResult result;
    
    if (!g_parserInitialized) {
        init();
    }
    
    if (json.empty()) {
        result.error = "Empty JSON content";
        return result;
    }
    
    // Simple JSON parsing (not a full parser)
    // Extract widget properties
    result.widget.name = getJsonValue(json, "name");
    if (result.widget.name.empty()) {
        result.widget.name = "Unnamed";
    }
    
    result.widget.width = getJsonInt(json, "width", 1920);
    result.widget.height = getJsonInt(json, "height", 1080);
    
    std::string bg = getJsonValue(json, "background");
    if (!bg.empty()) {
        result.widget.background = bg;
    }
    
    // Process elements array
    std::string elementsStr = getJsonValue(json, "elements");
    if (!elementsStr.empty()) {
        // Count elements
        int count = 0;
        for (size_t i = 0; i < elementsStr.length(); i++) {
            if (elementsStr[i] == '{') count++;
        }
        
        // Parse each element
        for (int i = 0; i < count; i++) {
            std::string elemJson = extractArrayElement(elementsStr, i);
            if (!elemJson.empty()) {
                processElementJson(elemJson, result.widget);
            }
        }
    }
    
    if (!validateWidget(result.widget)) {
        result.error = "Invalid widget structure";
        return result;
    }
    
    result.success = true;
    LOG_INFO("Parsed widget: %s (%dx%d) with %zu elements",
            result.widget.name.c_str(), result.widget.width, result.widget.height,
            result.widget.elements.size());
    
    return result;
}

// ============================================================================
// Color Parsing
// ============================================================================

void parseColorHex(const std::string& hex, int& r, int& g, int& b, int& a) {
    r = g = b = 0;
    a = 255;
    
    if (hex.empty()) return;
    
    std::string h = hex;
    if (h[0] == '#') h = h.substr(1);
    
    // Remove quotes if present
    if (h[0] == '"') h = h.substr(1);
    if (!h.empty() && h[h.length()-1] == '"') h = h.substr(0, h.length()-1);
    
    if (h.length() == 6) {
        try {
            r = std::stoi(h.substr(0, 2), nullptr, 16);
            g = std::stoi(h.substr(2, 2), nullptr, 16);
            b = std::stoi(h.substr(4, 2), nullptr, 16);
        } catch (...) {
            // Ignore parse errors
        }
    } else if (h.length() == 8) {
        try {
            r = std::stoi(h.substr(0, 2), nullptr, 16);
            g = std::stoi(h.substr(2, 2), nullptr, 16);
            b = std::stoi(h.substr(4, 2), nullptr, 16);
            a = std::stoi(h.substr(6, 2), nullptr, 16);
        } catch (...) {
            // Ignore parse errors
        }
    }
}

// ============================================================================
// JSON Validation
// ============================================================================

bool isValidJson(const std::string& json) {
    if (json.empty()) return false;
    
    // Check for basic JSON structure
    const char* str = json.c_str();
    size_t len = json.length();
    
    // Must start with { or [
    if (str[0] != '{' && str[0] != '[') return false;
    
    // Count braces/brackets
    int depth = 0;
    bool inString = false;
    char prevChar = 0;
    
    for (size_t i = 0; i < len; i++) {
        char c = str[i];
        
        if (c == '"' && prevChar != '\\') {
            inString = !inString;
        }
        
        if (!inString) {
            if (c == '{' || c == '[') depth++;
            if (c == '}' || c == ']') depth--;
        }
        
        prevChar = c;
    }
    
    return depth == 0;
}

// ============================================================================
// File Loading
// ============================================================================

std::string loadFileToString(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open file: %s", filepath.c_str());
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

} // namespace oo