<div align="center">

<img src="https://raw.githubusercontent.com/Hax45-dev/PeakMacro/refs/heads/main/resources/Peak_logo.png" alt="PeakMacro Logo" width="160">

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
git clone [https://github.com/Hax25-dev/PeakMacro.git](https://github.com/Hax25-dev/PeakMacro.git)
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
