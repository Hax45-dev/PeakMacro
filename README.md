<div align="center">

# ⛰️ PeakMacro

**Frame-perfect macro recorder & player for Geometry Dash**  
*A professional Geode mod by [@Hax25-dev](https://github.com/Hax25-dev)*

[![Version](https://img.shields.io/badge/version-1.0.0-blueviolet?style=flat-square)](https://github.com/Hax25-dev/PeakMacro/releases)
[![Geode](https://img.shields.io/badge/Geode-4.0.0+-purple?style=flat-square)](https://geode-sdk.org)
[![GD Version](https://img.shields.io/badge/GD-2.2074-orange?style=flat-square)](https://store.steampowered.com/app/322170)
[![License](https://img.shields.io/badge/license-MIT-green?style=flat-square)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Mac%20%7C%20Android-blue?style=flat-square)]()

</div>

---

## ✨ Features

| Feature | Description |
|---|---|
| 🔴 **Record** | Capture every input frame-by-frame with full precision |
| ▶️ **Playback** | Replay recorded macros with frame-perfect accuracy |
| 💾 **Save / Load** | Save macros as `.peak` files and load them anytime |
| 🎛️ **HUD Overlay** | Live recording/playback indicator inside the level |
| ⌨️ **Keybinds** | Control everything without opening the menu |
| 📋 **Macro Manager** | Browse, load, and delete all your saved macros |
| ⚙️ **Settings** | Configurable overlay, auto-save, and FPS bypass options |

---

## 🎮 Keybinds

| Key | Action |
|---|---|
| `P` | Open PeakMacro popup (in-level) |
| `R` | Toggle recording on/off |
| `Y` | Toggle playback on/off |

You can also access everything from the **Pause Menu → PeakMacro** button.

---

## 📦 Installation

### Via Geode Index *(recommended)*
1. Open Geometry Dash with [Geode](https://geode-sdk.org) installed
2. Go to **Mods → Search** and search for `PeakMacro`
3. Click **Install**

### Manual Installation
1. Download the latest `.geode` file from [Releases](https://github.com/Hax25-dev/PeakMacro/releases)
2. Place it in your Geode mods folder:
   - **Windows:** `%LOCALAPPDATA%\GeometryDash\geode\mods\`
   - **Mac:** `~/Library/Application Support/GeometryDash/geode/mods/`
   - **Android:** `/sdcard/Android/data/com.robtopx.geometryjump/geode/mods/`
3. Launch the game — PeakMacro will load automatically

---

## 🔧 Building from Source

### Prerequisites
- [CMake](https://cmake.org/) 3.21+
- [Geode SDK](https://docs.geode-sdk.org/getting-started/)
- C++20 compatible compiler (MSVC, Clang, or GCC)

### Steps

```bash
# 1. Clone the repository
git clone https://github.com/Hax25-dev/PeakMacro.git
cd PeakMacro

# 2. Set the Geode SDK environment variable
# Windows (PowerShell):
$env:GEODE_SDK = "C:/path/to/geode-sdk"
# Linux/Mac:
export GEODE_SDK="/path/to/geode-sdk"

# 3. Configure with CMake
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 4. Build
cmake --build build --config Release
```

The compiled `.geode` file will be in `build/`.

---

## 📁 File Format

PeakMacro saves macros as `.peak` files (JSON-based):

```json
{
  "name": "MyMacro",
  "levelName": "Stereo Madness",
  "author": "Hax25-dev",
  "createdAt": "2024-01-15_10-30-00",
  "fps": 60,
  "totalFrames": 3240,
  "frames": [
    { "frame": 12, "p1": true, "p2": false, "hold1": true, "hold2": false, "x": 120.5, "y": 205.3 }
  ]
}
```

Macro files are stored in:  
`%LOCALAPPDATA%\GeometryDash\geode\mods\hax25dev.peak_macro\macros\`

---

## ⚙️ Settings

| Setting | Default | Description |
|---|---|---|
| Show Overlay | ✅ On | Display HUD during recording/playback |
| Overlay Opacity | 0.8 | Transparency of the HUD |
| Auto Save Macro | ✅ On | Automatically save when recording stops |
| FPS Bypass | ❌ Off | Enable high-FPS mode during playback |

---

## 🗺️ Roadmap

- [ ] Frame advance / step-by-step playback  
- [ ] Macro editor (trim, merge, splice)  
- [ ] Speed multiplier during playback  
- [ ] Export to video  
- [ ] Cloud sync  

---

## 📄 License

This project is licensed under the **MIT License** — see [LICENSE](LICENSE) for details.

---

## 🙏 Credits

| | |
|---|---|
| **Author** | [@Hax25-dev](https://github.com/Hax25-dev) |
| **Framework** | [Geode SDK](https://geode-sdk.org) by the Geode Team |
| **Game** | Geometry Dash by RobTop Games |

---

<div align="center">
Made with ❤️ by <b>@Hax25-dev</b>
</div>
