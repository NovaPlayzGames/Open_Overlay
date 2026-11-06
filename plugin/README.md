# Open Overlay OBS Plugin

A native OBS Studio plugin for loading and rendering `.oo` overlay files directly in OBS Studio.

## Features

- ✅ Load `.oo` files directly in OBS
- ✅ Render shapes, text, and images
- ✅ Support for animations (fade, slide, zoom, bounce, pulse, shake)
- ✅ Custom source type in OBS
- ✅ Real-time preview

## Requirements

### Windows
- **OBS Studio 28+** (source code)
- **Visual Studio 2022** (Community edition works)
- **CMake 3.16+**
- **Git** (with LFS for large files)

### Build OBS Studio

Before building this plugin, you need to have OBS Studio source code:

```powershell
# 1. Clone OBS Studio (this takes a while!)
cd C:\
git clone --recursive https://github.com/obsproject/obs-studio.git

# 2. Build OBS Studio (required for the libraries)
cd obs-studio
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release --parallel
```

> **Note**: Building OBS takes 30-60 minutes on first build. The OBS build folder will be used by this plugin.

## Building the Plugin

### Step 1: Update OBS Path

Edit `plugin/CMakeLists.txt` and set the OBS path:

```cmake
# Around line 25:
set(OBS_STUDIO_PATH "C:/obs-studio" CACHE PATH "Path to OBS Studio source")
```

### Step 2: Build

```powershell
cd C:\Users\20leo\Desktop\Open_Overlay\plugin
.\build.bat
```

Or manually:

```powershell
cd C:\Users\20leo\Desktop\Open_Overlay\plugin
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64 -DOBS_STUDIO_PATH="C:\obs-studio" -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

### Step 3: Install

Copy the output DLL to your OBS plugins folder:

```
build\bin\Release\open_overlay_plugin.dll
→ C:\Program Files\obs-studio\obs-plugins\64bit\
```

## Using in OBS

1. Start OBS Studio
2. Add Source → **Open Overlay**
3. Browse to your `.oo` file
4. The overlay will render in OBS!

## .oo File Format

Your `.oo` files are JSON with this structure:

```json
{
  "id": "widget-123",
  "name": "My Overlay",
  "width": 1920,
  "height": 1080,
  "background": "transparent",
  "elements": [
    {
      "type": "shape",
      "shapeType": "rectangle",
      "x": 100,
      "y": 100,
      "width": 200,
      "height": 100,
      "fill": "#3b82f6",
      "borderRadius": 8,
      "animationName": "fadeIn",
      "animationDuration": 1.0
    },
    {
      "type": "text",
      "content": "Hello World!",
      "x": 100,
      "y": 50,
      "fontSize": 32,
      "color": "#ffffff",
      "fontFamily": "Inter"
    }
  ]
}
```

### Supported Element Types

| Type | Properties |
|------|------------|
| `shape` | `shapeType` (rectangle, circle, triangle), `fill`, `borderRadius` |
| `text` | `content`, `fontSize`, `color`, `fontFamily`, `textAlign` |
| `image` | `src` (URL or base64), `objectFit`, `borderRadius` |

### Supported Animations

- `fadeIn` / `fadeOut`
- `slideIn` / `slideOut`
- `zoomIn` / `zoomOut`
- `bounce`
- `pulse`
- `shake`

## Troubleshooting

### "libobs not found" error
- Make sure you've built OBS Studio at least once
- Verify `OBS_STUDIO_PATH` points to your obs-studio folder

### Plugin doesn't appear in OBS
- Check that the DLL is in the correct folder: `C:\Program Files\obs-studio\obs-plugins\64bit\`
- Make sure it matches your OBS architecture (64-bit)

### Build errors
- Ensure Visual Studio 2022 is installed
- Run from "Developer Command Prompt for VS2022"

## Development

### Project Structure

```
plugin/
├── CMakeLists.txt      # Build configuration
├── build.bat           # Windows build script
├── README.md           # This file
└── src/
    ├── plugin-main.cpp       # Plugin entry point
    ├── openoverlay-source.h  # OBS source type header
    ├── openoverlay-source.cpp # OBS source implementation
    ├── oo-parser.h           # .oo file parser header
    ├── oo-parser.cpp         # .oo file parser
    ├── renderer.h            # Renderer header
    └── renderer.cpp          # Graphics rendering
```

## License

MIT License - See LICENSE file for details

---

**Note**: This plugin is in development. For a production-ready solution, you may also consider using the Browser Source approach with your app's built-in HTTP server.
