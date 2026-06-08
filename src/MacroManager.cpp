#include "MacroManager.hpp"
#include <Geode/Geode.hpp>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>

using namespace geode::prelude;
namespace fs = std::filesystem;

namespace PeakMacro {

    // ─────────────────────────────────────────────────────────────────
    //  Helpers
    // ─────────────────────────────────────────────────────────────────

    std::string MacroManager::currentTimestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm tm{};
#ifdef _WIN32
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif
        std::ostringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
        return ss.str();
    }

    std::string MacroManager::getMacroDirectory() const {
        auto dir = Mod::get()->getSaveDir() / "macros";
        if (!fs::exists(dir))
            fs::create_directories(dir);
        return dir.string();
    }

    // ─────────────────────────────────────────────────────────────────
    //  Recording
    // ─────────────────────────────────────────────────────────────────

    void MacroManager::startRecording() {
        if (m_state != MacroState::Idle) return;

        m_macro.clear();
        m_macro.createdAt = currentTimestamp();
        m_macro.levelName = m_levelName;
        m_macro.author    = "Hax25-dev";
        m_macro.fps       = 60;
        m_state           = MacroState::Recording;

        log::info("[PeakMacro] Recording started.");
    }

    void MacroManager::stopRecording() {
        if (m_state != MacroState::Recording) return;

        m_state = MacroState::Idle;
        m_macro.totalFrames = static_cast<int>(m_macro.frames.size());

        log::info("[PeakMacro] Recording stopped. Frames: {}", m_macro.totalFrames);

        if (Mod::get()->getSettingValue<bool>("auto-save-macro"))
            saveMacro();
    }

    void MacroManager::recordFrame(int frame, bool p1, bool p2,
                                    bool hold1, bool hold2,
                                    float x,   float y) {
        if (m_state != MacroState::Recording) return;

        // Only record when there is actual input
        if (!p1 && !p2) return;

        MacroFrame f;
        f.frame   = frame;
        f.player1 = p1;
        f.player2 = p2;
        f.hold1   = hold1;
        f.hold2   = hold2;
        f.xPos    = x;
        f.yPos    = y;
        m_macro.frames.push_back(f);
    }

    // ─────────────────────────────────────────────────────────────────
    //  Playback
    // ─────────────────────────────────────────────────────────────────

    void MacroManager::startPlayback() {
        if (m_macro.isEmpty()) {
            Notification::create("PeakMacro", "No macro loaded!", NotificationIcon::Warning)->show();
            return;
        }
        if (m_state != MacroState::Idle) return;

        m_playbackFrame = 0;
        m_state         = MacroState::Playing;
        log::info("[PeakMacro] Playback started. Total frames: {}", m_macro.totalFrames);
    }

    void MacroManager::stopPlayback() {
        if (m_state != MacroState::Playing) return;
        m_state         = MacroState::Idle;
        m_playbackFrame = 0;
        log::info("[PeakMacro] Playback stopped.");
    }

    void MacroManager::stepPlayback(int currentFrame) {
        if (m_state != MacroState::Playing) return;

        // Check if we've reached the end
        if (m_playbackFrame >= static_cast<int>(m_macro.frames.size())) {
            stopPlayback();
            return;
        }

        // Execute all frames that match the current game frame
        while (m_playbackFrame < static_cast<int>(m_macro.frames.size()) &&
               m_macro.frames[m_playbackFrame].frame <= currentFrame) {
            ++m_playbackFrame;
        }
    }

    // ─────────────────────────────────────────────────────────────────
    //  File I/O
    // ─────────────────────────────────────────────────────────────────

    bool MacroManager::saveMacro(std::string const& name) {
        auto filename = name.empty()
            ? m_macro.levelName + "_" + m_macro.createdAt
            : name;

        // Sanitise filename
        for (char& c : filename)
            if (c == '/' || c == '\\' || c == ':' || c == '*' ||
                c == '?' || c == '"'  || c == '<' || c == '>' || c == '|')
                c = '_';

        auto path = getMacroDirectory() + "/" + filename + ".peak";
        m_macro.name = filename;

        if (m_macro.saveToFile(path)) {
            log::info("[PeakMacro] Macro saved: {}", path);
            Notification::create("PeakMacro",
                fmt::format("Saved: {}", filename), NotificationIcon::Success)->show();
            return true;
        }

        log::error("[PeakMacro] Failed to save macro: {}", path);
        Notification::create("PeakMacro", "Failed to save macro!", NotificationIcon::Error)->show();
        return false;
    }

    bool MacroManager::loadMacro(std::string const& path) {
        auto result = Macro::loadFromFile(path);
        if (!result) {
            log::error("[PeakMacro] Failed to load macro: {}", path);
            Notification::create("PeakMacro", "Failed to load macro!", NotificationIcon::Error)->show();
            return false;
        }

        m_macro = std::move(*result);
        log::info("[PeakMacro] Macro loaded: {} ({} frames)", m_macro.name, m_macro.totalFrames);
        Notification::create("PeakMacro",
            fmt::format("Loaded: {}", m_macro.name), NotificationIcon::Success)->show();
        return true;
    }

    std::vector<std::string> MacroManager::getSavedMacros() const {
        std::vector<std::string> macros;
        auto dir = getMacroDirectory();
        for (auto const& entry : fs::directory_iterator(dir)) {
            if (entry.path().extension() == ".peak")
                macros.push_back(entry.path().string());
        }
        std::sort(macros.begin(), macros.end());
        return macros;
    }

    void MacroManager::clearMacro() {
        m_macro.clear();
        m_macro.name = "Untitled";
        m_playbackFrame = 0;
        log::info("[PeakMacro] Macro cleared.");
    }

    float MacroManager::getProgress() const {
        if (m_macro.totalFrames == 0) return 0.f;
        return static_cast<float>(m_playbackFrame) /
               static_cast<float>(m_macro.totalFrames);
    }

} // namespace PeakMacro
