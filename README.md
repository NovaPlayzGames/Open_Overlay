<div align="center">

# 🎨 Open Overlay

**The Local-First OBS Browser Source Designer**

[![Tauri](https://img.shields.io/badge/Tauri-v2-24C8DB?style=for-the-badge&logo=tauri&logoColor=white)](https://tauri.app)
[![React](https://img.shields.io/badge/React-19-61DAFB?style=for-the-badge&logo=react&logoColor=black)](https://react.dev)
[![TypeScript](https://img.shields.io/badge/TypeScript-5.8-3178C6?style=for-the-badge&logo=typescript&logoColor=white)](https://www.typescriptlang.org/)
[![Rust](https://img.shields.io/badge/Rust-1.77+-000000?style=for-the-badge&logo=rust&logoColor=white)](https://www.rust-lang.org/)
[![CI](https://github.com/NovaPlayzGames/Open_Overlay/actions/workflows/ci.yml/badge.svg)](https://github.com/NovaPlayzGames/Open_Overlay/actions/workflows/ci.yml)
[![Release](https://github.com/NovaPlayzGames/Open_Overlay/actions/workflows/release.yml/badge.svg)](https://github.com/NovaPlayzGames/Open_Overlay/actions/workflows/release.yml)

*Empowering streamers and creators to design, manage, and serve custom overlays directly from their local machine, ensuring maximum performance and full control over their broadcast visuals.*

[Features](#-features) • [Tech Stack](#-tech-stack) • [Getting Started](#-getting-started) • [Connecting to OBS](#-connecting-to-obs)

</div>

---

## ✨ Features

Open Overlay features a visual WYSIWYG editor and a built-in local web server that seamlessly injects your designs straight into an OBS Browser Source.

- **🖌️ Visual Designer**: An intuitive drag-and-drop canvas. Precisely move, resize, and rotate elements in your overlay in real-time.
- **↩️ Undo/Redo + Autosave**: Full edit history (`Ctrl+Z`/`Ctrl+Shift+Z`), debounced autosave, and a dirty-state indicator in the status bar.
- **👯 Multi-Select Power Tools**: Shift-click to select multiple elements — drag them together, align/distribute, group (`Ctrl+G`) and ungroup (`Ctrl+Shift+G`), flip, and reorder (`Ctrl+[`/`Ctrl+]`).
- **🧲 Smart Snapping**: Elements snap to sibling edges, centers, and canvas bounds with live guide lines. Hold `Shift` while resizing to lock aspect ratio, and `Alt` to bypass snapping.
- **� Zoom Controls**: Fit-to-view auto scaling plus manual zoom (`Ctrl+wheel`, `Ctrl+=`/`Ctrl+-`) for pixel-perfect work.
- **🌈 13 App Themes**: Midnight, Obsidian, Tokyo Night, Dracula, Nord, Catppuccin, Gruvbox, Cyberpunk, Forest, Ocean, Rose, Ember, and a light "Paper" theme — instant switching from the settings palette.
- **�📚 Layers & Grouping**: Comprehensive layer management to control the z-index and visibility of your elements. Group elements together to organize complex overlay structures.
- **🎭 Advanced Masking System**: Apply custom masks—such as clip masks, alpha layers, or inverted shapes—to achieve tailored visual effects.
- **🧩 Rich Elements**: Text, shapes, images, **videos**, **web embeds** (iframes), freehand paths, and solid color blocks — with gradient fills, gradient text, and drop shadows.
- **🎬 Animation System**: 18 built-in animation presets (glow, shake, spin, tada, heartbeat…) plus a keyframe timeline with easing for fully choreographed motion.
- **📐 Alignment Suite**: Align selections to each other or snap a single element to canvas edges/center in one click.
- **🗂️ Widget Management**: Save multiple "widgets" (independent overlay scenes). Duplicate, rename, or delete them — and spin up new ones instantly from the **template gallery**.
- **🚀 Embedded Local OBS Server**: Your desktop app runs a lightweight HTTP server in the background. Simply copy a widget's specialized local URL and paste it as a new Browser Source in OBS. No external hosting required!
- **🎨 Color Picker**: Included fully customized color picker featuring alpha controls, hex editing, and palette adjustments for streamlined styling.
- **⌨️ Pro Keyboard Shortcuts**: Everything is a hotkey — tools, nudging, grouping, zoom, save. Press `Ctrl+/` in-app for the full cheat sheet.

---

## 🛠 Tech Stack

Open Overlay bridges modern web capabilities with a sturdy Rust backend:

### Frontend
- **Core**: React 19, TypeScript, and Vite for blazing-fast development.
- **Styling**: Tailwind CSS (v4) paired with `clsx` and `tailwind-merge`.
- **Interactions**: Custom robust dragging systems, and `@dnd-kit` for smooth layer reordering and canvas operations.

### Backend
- **Desktop Application**: Tauri (v2) in Rust, leveraging local OS APIs like file system access and dialogs.
- **Built-in Server (Rust)**: Actix-Web powered by Tokio to serve overlay assets locally directly to OBS. Database persistence is handled via SQLite (`rusqlite`).

---

## 🚀 Getting Started

### Prerequisites

Ensure you have the following installed on your system:
- **[Node.js](https://nodejs.org/)** (v18+)
- **[Rust & Cargo](https://rustup.rs/)** (Latest stable)
- **Tauri Prerequisites**: Follow the [Tauri setup guide](https://tauri.app/v1/guides/getting-started/prerequisites) to install C++ Build Tools (Windows) or the necessary libraries on macOS/Linux.

### Installation & Running

1. **Clone the repository** (or navigate to the project directory):
   ```bash
   cd Open_Overlay
   ```

2. **Install frontend dependencies**:
   ```bash
   npm install
   ```

3. **Run the development server**:
   This command starts the Vite development server and launches the rust-based Tauri window:
   ```bash
   npm run dev
   ```

4. **Build for production**:
   To compile a native executable for your OS:
   ```bash
   npm run tauri:build
   ```

---

## � CI / CD

GitHub Actions workflows live in `.github/workflows/`:

| Workflow | Trigger | What it does |
|----------|---------|--------------|
| `ci.yml` | Push to `main` + PRs | `tsc --noEmit`, `vite build`, `cargo check`, `cargo clippy` |
| `release.yml` | Tag `v*` or manual dispatch | Builds Tauri bundles on **Windows** (`.msi`/`.exe`), **macOS** (`.dmg`), and **Linux** (`.AppImage`/`.deb`). Tag pushes create a draft GitHub Release with all installers attached; manual runs upload them as workflow artifacts. |

To cut a release:

```bash
git tag v0.1.0
git push origin v0.1.0
```

> Bundle signing (optional): add `TAURI_SIGNING_PRIVATE_KEY` and `TAURI_SIGNING_PRIVATE_KEY_PASSWORD` to your repo secrets and uncomment the env vars in `release.yml`.

---

## �📺 Connecting to OBS

Making your overlay live is completely frictionless:

1. 💻 Open up the **Open Overlay** application and create a new **Widget**.
2. 🎨 Design your widget using the canvas editor.
3. 🔗 Click the **OBS Link** option in the bottom left toolbar to show the local URL panel (e.g., `http://localhost:7878/overlay/<widget-id>`) and copy the URL.
4. 🎥 In **OBS Studio**, add a new **Browser Source**.
5. 📋 Paste the copied URL into the URL field. Set the width and height to match your widget's native bounds, and voilà! Your local overlay is now live on your stream.

---

<div align="center">
  <sub>Built with ❤️ for Creators.</sub>
</div>