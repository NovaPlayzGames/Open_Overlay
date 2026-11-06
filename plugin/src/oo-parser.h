/**
 * Open Overlay - Parser Header
 * 
 * Parses .oo overlay files and JSON content into Widget objects
 */

#ifndef OO_PARSER_H
#define OO_PARSER_H

#include <string>
#include <vector>
#include <variant>
#include <memory>
#include <functional>

// ============================================================================
// Animation Types
// ============================================================================

namespace oo {

enum class AnimationType {
    None,
    FadeIn,
    FadeOut,
    SlideIn,
    SlideOut,
    ZoomIn,
    ZoomOut,
    Pulse,
    Bounce,
    Shake
};

AnimationType parseAnimationType(const std::string& name);

// ============================================================================
// Shape Types
// ============================================================================

enum class ShapeType {
    Rectangle,
    Circle,
    Triangle
};

ShapeType parseShapeType(const std::string& type);

// ============================================================================
// Text Alignment
// ============================================================================

enum class TextAlign {
    Left,
    Center,
    Right
};

TextAlign parseTextAlign(const std::string& align);

// ============================================================================
// Font Weight
// ============================================================================

enum class FontWeight {
    Normal,
    Bold
};

FontWeight parseFontWeight(const std::string& weight);

// ============================================================================
// Base Element
// ============================================================================

struct Element {
    std::string id;
    double x = 0;
    double y = 0;
    double width = 100;
    double height = 100;
    double opacity = 1.0;
    double rotation = 0;
    bool visible = true;
    
    // Animation properties
    AnimationType animationName = AnimationType::None;
    double animationDuration = 1.0;
    double animationDelay = 0.0;
    int animationIterationCount = 1; // -1 for infinite
    
    Element() = default;
    virtual ~Element() = default;
};

// ============================================================================
// Shape Element
// ============================================================================

struct ShapeElement : Element {
    ShapeType shapeType = ShapeType::Rectangle;
    std::string fill = "#FFFFFF";
    double borderRadius = 0;
    
    ShapeElement() : Element() {}
};

// ============================================================================
// Text Element  
// ============================================================================

struct TextElement : Element {
    std::string content;
    double fontSize = 16;
    std::string color = "#000000";
    std::string fontFamily = "Arial";
    TextAlign textAlign = TextAlign::Left;
    FontWeight fontWeight = FontWeight::Normal;
    
    TextElement() : Element() {}
};

// ============================================================================
// Image Element
// ============================================================================

struct ImageElement : Element {
    std::string src;
    std::string alt;
    
    ImageElement() : Element() {}
};

// ============================================================================
// Widget (Root Element)
// ============================================================================

struct Widget {
    std::string name;
    int width = 1920;
    int height = 1080;
    std::string background = "transparent";
    
    // Elements as variant
    std::vector<std::variant<ShapeElement, TextElement, ImageElement>> elements;
};

// ============================================================================
// Parse Result
// ============================================================================

struct ParseResult {
    bool success = false;
    std::string error;
    Widget widget;
};

// ============================================================================
// OOParser Class
// ============================================================================

class OOParser {
public:
    /**
     * Initialize the parser
     */
    static void init();
    
    /**
     * Cleanup the parser
     */
    static void cleanup();
    
    /**
     * Parse a .oo file from disk
     */
    static ParseResult parseFile(const std::string& filepath);
    
    /**
     * Parse .oo JSON content from a string
     */
    static ParseResult parseString(const std::string& json);
    
private:
    static bool g_parserInitialized;
    
    // Private helpers
    static bool validateWidget(const Widget& widget);
    static void processElementJson(const std::string& json, Widget& widget);
};

// ============================================================================
// Utility Functions (in oo namespace)
// ============================================================================

/**
 * Parse a color hex string into RGBA components
 */
void parseColorHex(const std::string& hex, int& r, int& g, int& b, int& a);

/**
 * Check if a string is valid JSON
 */
bool isValidJson(const std::string& json);

/**
 * Load a file into a string
 */
std::string loadFileToString(const std::string& filepath);

} // namespace oo

// ============================================================================
// C-style API (for OBS plugin use)
// ============================================================================

/**
 * Initialize the parser
 */
inline void oo_parser_init() {
    oo::OOParser::init();
}

/**
 * Cleanup the parser
 */
inline void oo_parser_cleanup() {
    oo::OOParser::cleanup();
}

#endif // OO_PARSER_H