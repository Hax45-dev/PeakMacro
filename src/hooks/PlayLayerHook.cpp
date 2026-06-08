#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "MacroManager.hpp"

using namespace geode::prelude;
using namespace PeakMacro;

// ═══════════════════════════════════════════════════════════════════
//  PlayLayer Hook  —  lifecycle events (init, reset, quit)
// ═══════════════════════════════════════════════════════════════════
class $modify(PeakPlayLayer, PlayLayer) {

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        // Store level name for macro metadata
        MacroManager::get().setCurrentLevelName(
            level ? level->m_levelName : "Unknown"
        );

        return true;
    }

    void resetLevel() {
        auto& mgr = MacroManager::get();

        // Stop any active state before reset
        if (mgr.isRecording()) mgr.stopRecording();
        if (mgr.isPlaying())   mgr.stopPlayback();

        PlayLayer::resetLevel();

        // Auto-start playback on reset if a macro is loaded
        if (!mgr.getMacro().isEmpty() && mgr.isIdle()) {
            // Playback is triggered manually via UI; do nothing here
        }
    }

    void onQuit() {
        auto& mgr = MacroManager::get();
        if (mgr.isRecording()) mgr.stopRecording();
        if (mgr.isPlaying())   mgr.stopPlayback();

        PlayLayer::onQuit();
    }
};

// ═══════════════════════════════════════════════════════════════════
//  GJBaseGameLayer Hook  —  per-frame update for input capture
// ═══════════════════════════════════════════════════════════════════
class $modify(PeakGameLayer, GJBaseGameLayer) {

    void handleButton(bool hold, int button, bool isPlayer1) {
        auto& mgr = MacroManager::get();

        // ── Recording ───────────────────────────────────────────────
        if (mgr.isRecording()) {
            auto* pl = PlayLayer::get();
            float x = pl ? pl->m_player1->getPositionX() : 0.f;
            float y = pl ? pl->m_player1->getPositionY() : 0.f;

            mgr.recordFrame(
                m_gameState.m_currentProgress,  // current frame counter
                isPlayer1,                       // which player
                !isPlayer1,
                hold && isPlayer1,
                hold && !isPlayer1,
                x, y
            );
        }

        // ── Playback — block real input ──────────────────────────────
        if (mgr.isPlaying()) {
            // During playback we suppress actual user input
            return;
        }

        GJBaseGameLayer::handleButton(hold, button, isPlayer1);
    }

    void update(float dt) {
        auto& mgr = MacroManager::get();

        // ── Playback — inject recorded frames ───────────────────────
        if (mgr.isPlaying()) {
            int frame = m_gameState.m_currentProgress;
            auto const& frames = mgr.getMacro().frames;

            for (auto const& f : frames) {
                if (f.frame == frame) {
                    if (f.player1)
                        GJBaseGameLayer::handleButton(f.hold1, 1, true);
                    if (f.player2)
                        GJBaseGameLayer::handleButton(f.hold2, 1, false);
                }
            }

            mgr.stepPlayback(frame);
        }

        GJBaseGameLayer::update(dt);
    }
};
