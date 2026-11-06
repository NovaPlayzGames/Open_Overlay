/**
 * Open Overlay - Renderer Header
 * 
 * Renders .oo widget elements using OBS graphics API
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <obs.h>
#include <obs-source.h>
#include <graphics/graphics.h>
#include <graphics/matrix4.h>
#include <vector>
#include <memory>
#include <string>
#include <map>

#include "oo-parser.h"

// Forward declarations
namespace oo {
    class Renderer;
}

// ============================================================================
// Texture Cache
// ============================================================================

class TextureCache {
public:
    TextureCache();
    ~TextureCache();
    
    // Get or create a texture from URL or base64
    gs_texture_t* getTexture(const std::string& url, int width, int height);
    
    // Clear all cached textures
    void clear();
    
private:
    std::map<std::string, gs_texture_t*> m_textures;
};

// ============================================================================
// Animation State
// ============================================================================

struct AnimationState {
    double progress;      // 0.0 to 1.0
    double elapsed;       // Total elapsed time
    bool complete;
    
    AnimationState() : progress(0.0), elapsed(0.0), complete(false) {}
};

// ============================================================================
// Renderer Class
// ============================================================================

namespace oo {

class Renderer {
public:
    /**
     * Create a renderer for a widget
     */
    explicit Renderer(const Widget& widget);
    ~Renderer();
    
    /**
     * Render the widget at the given animation time
     */
    void render(double animationTime);
    
    /**
     * Get the widget dimensions
     */
    int getWidth() const { return m_widget.width; }
    int getHeight() const { return m_widget.height; }
    
    /**
     * Get the background color
     */
    std::string getBackground() const { return m_widget.background; }
    
private:
    // Widget data
    Widget m_widget;
    
    // Texture cache
    std::unique_ptr<TextureCache> m_textureCache;
    
    // Animation states per element
    std::map<std::string, AnimationState> m_animationStates;
    
    // Render helpers
    void renderElement(const Element& element, double animationTime);
    void renderShape(const ShapeElement& shape, double animationTime);
    void renderText(const TextElement& text, double animationTime);
    void renderImage(const ImageElement& image, double animationTime);
    
    // Animation helpers
    AnimationState calculateAnimation(const Element& element, double animationTime);
    void applyAnimation(AnimationState& state, const Element& element);
    
    // Graphics helpers
    void drawRectangle(double x, double y, double width, double height,
                       const std::string& fill, double borderRadius,
                       double opacity, double rotation);
    void drawCircle(double x, double y, double radius, 
                    const std::string& fill, double opacity);
    void drawTriangle(double x, double y, double width, double height,
                      const std::string& fill, double opacity);
    void drawText(const std::string& text, double x, double y,
                  double fontSize, const std::string& color,
                  const std::string& fontFamily, TextAlign align,
                  FontWeight weight, double width, double height);
    
    // Color helpers
    void setColor(const std::string& hex, double opacity = 1.0);
    void clearColor();
};

} // namespace oo

#endif // RENDERER_H
