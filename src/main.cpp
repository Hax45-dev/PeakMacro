#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

#include "MacroManager.hpp"
#include "MacroPopup.hpp"
#include "MacroOverlay.hpp"

using namespace geode::prelude;
using namespace PeakMacro;

// ═══════════════════════════════════════════════════════════════════
//  PauseLayer Hook  —  inject PeakMacro button into the pause menu
// ═══════════════════════════════════════════════════════════════════
class $modify(PeakPauseLayer, PauseLayer) {

    void customSetup() {
        PauseLayer::customSetup();

        // ── PeakMacro button ──────────────────────────────────────
        auto* btn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("PeakMacro", "goldFont.fnt", "GJ_button_01.png", 0.75f),
            this,
            menu_selector(PeakPauseLayer::onPeakMacro)
        );

        // Place it below the standard pause buttons
        auto* menu = CCMenu::create(btn, nullptr);
        menu->setPosition({
            CCDirector::get()->getWinSize().width / 2.f,
            36.f
        });
        this->addChild(menu, 10);
    }

    void onPeakMacro(CCObject*) {
        auto* popup = MacroPopup::create();
        if (popup) popup->show();
    }
};

// ═══════════════════════════════════════════════════════════════════
//  Keyboard shortcuts (in-level)
//  P = open popup | R = toggle recording | Space = toggle playback
// ═══════════════════════════════════════════════════════════════════
class $modify(PeakKeyboardLayer, GJBaseGameLayer) {

    void keyDown(cocos2d::enumKeyCodes key) {
        auto& mgr = MacroManager::get();
        bool inLevel = PlayLayer::get() != nullptr;

        // Open PeakMacro popup
        if (key == cocos2d::enumKeyCodes::KEY_P && inLevel) {
            auto* popup = MacroPopup::create();
            if (popup) popup->show();
            return;
        }

        // Toggle recording (R)
        if (key == cocos2d::enumKeyCodes::KEY_R && inLevel) {
            if (mgr.isRecording()) {
                mgr.stopRecording();
                Notification::create("PeakMacro", "Recording saved!", NotificationIcon::Success)->show();
            } else if (mgr.isIdle()) {
                mgr.startRecording();
                Notification::create("PeakMacro", "Recording...", NotificationIcon::Info)->show();
            }
            return;
        }

        // Toggle playback (Y)
        if (key == cocos2d::enumKeyCodes::KEY_Y && inLevel) {
            if (mgr.isPlaying()) {
                mgr.stopPlayback();
            } else if (mgr.isIdle() && !mgr.getMacro().isEmpty()) {
                mgr.startPlayback();
                if (auto* pl = PlayLayer::get()) pl->resetLevel();
                Notification::create("PeakMacro", "Playback started!", NotificationIcon::Info)->show();
            }
            return;
        }

        GJBaseGameLayer::keyDown(key);
    }
};

// ═══════════════════════════════════════════════════════════════════
//  $on_mod(Loaded) — runs once when Geode loads the mod
// ═══════════════════════════════════════════════════════════════════
$on_mod(Loaded) {
    log::info("PeakMacro v1.0.0 loaded — by @Hax25-dev");
    log::info("Macro directory: {}", MacroManager::get().getMacroDirectory());
    log::info("Keybinds: [P] Open UI | [R] Toggle Recording | [Y] Toggle Playback");
}
