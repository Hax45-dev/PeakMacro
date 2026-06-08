#pragma once
#include <Geode/Geode.hpp>
#include "Macro.hpp"

using namespace geode::prelude;

namespace PeakMacro {

    enum class MacroState {
        Idle,
        Recording,
        Playing
    };

    // ─────────────────────────────────────────────────────────────────
    //  MacroManager  —  Central singleton that owns all macro logic
    // ─────────────────────────────────────────────────────────────────
    class MacroManager {
    public:
        static MacroManager& get() {
            static MacroManager instance;
            return instance;
        }

        // ── State ────────────────────────────────────────────────────
        MacroState getState() const { return m_state; }
        bool       isRecording() const { return m_state == MacroState::Recording; }
        bool       isPlaying()   const { return m_state == MacroState::Playing;   }
        bool       isIdle()      const { return m_state == MacroState::Idle;       }

        // ── Recording ────────────────────────────────────────────────
        void startRecording();
        void stopRecording();
        void recordFrame(int frame, bool p1, bool p2, bool hold1, bool hold2,
                         float x, float y);

        // ── Playback ─────────────────────────────────────────────────
        void startPlayback();
        void stopPlayback();
        void stepPlayback(int currentFrame);

        // ── File I/O ─────────────────────────────────────────────────
        bool saveMacro(std::string const& name = "");
        bool loadMacro(std::string const& path);
        std::vector<std::string> getSavedMacros() const;
        std::string getMacroDirectory() const;

        // ── Current macro access ─────────────────────────────────────
        Macro&       getMacro()       { return m_macro; }
        Macro const& getMacro() const { return m_macro; }
        void         setMacro(Macro const& m) { m_macro = m; }
        void         clearMacro();

        // ── Stats ────────────────────────────────────────────────────
        int  getPlaybackFrame()  const { return m_playbackFrame; }
        int  getTotalFrames()    const { return m_macro.totalFrames; }
        float getProgress()      const;

        // ── Level helpers ─────────────────────────────────────────────
        void setCurrentLevelName(std::string const& name) { m_levelName = name; }

    private:
        MacroManager()  = default;
        ~MacroManager() = default;
        MacroManager(MacroManager const&)            = delete;
        MacroManager& operator=(MacroManager const&) = delete;

        MacroState  m_state         = MacroState::Idle;
        Macro       m_macro;
        int         m_playbackFrame = 0;
        std::string m_levelName;

        // Returns a timestamp string like "2024-01-15_10-30-00"
        static std::string currentTimestamp();
    };

} // namespace PeakMacro
