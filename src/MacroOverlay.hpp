#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "MacroManager.hpp"

using namespace geode::prelude;

namespace PeakMacro {

// ─────────────────────────────────────────────────────────────────
//  MacroOverlay  —  HUD node added directly to PlayLayer
// ─────────────────────────────────────────────────────────────────
class MacroOverlay : public CCNode {
public:
    static MacroOverlay* create() {
        auto* ret = new MacroOverlay();
        if (ret && ret->init()) { ret->autorelease(); return ret; }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init() override {
        if (!CCNode::init()) return false;

        // ── Background pill ───────────────────────────────────────
        m_bg = CCScale9Sprite::create("square02_small.png");
        m_bg->setContentSize({ 140.f, 28.f });
        m_bg->setOpacity(160);
        m_bg->setColor({ 0, 0, 0 });
        this->addChild(m_bg);

        // ── Dot indicator ─────────────────────────────────────────
        m_dot = CCLabelBMFont::create("●", "bigFont.fnt");
        m_dot->setScale(0.45f);
        m_dot->setPosition({ 14.f, 0.f });
        this->addChild(m_dot);

        // ── Status text ───────────────────────────────────────────
        m_label = CCLabelBMFont::create("IDLE", "chatFont.fnt");
        m_label->setScale(0.55f);
        m_label->setAnchorPoint({ 0.f, 0.5f });
        m_label->setPosition({ 26.f, 0.f });
        this->addChild(m_label);

        // ── Frame counter ─────────────────────────────────────────
        m_frames = CCLabelBMFont::create("0", "chatFont.fnt");
        m_frames->setScale(0.45f);
        m_frames->setAnchorPoint({ 1.f, 0.5f });
        m_frames->setPosition({ 136.f, 0.f });
        m_frames->setColor({ 200, 200, 200 });
        this->addChild(m_frames);

        this->scheduleUpdate();
        return true;
    }

    void update(float) override {
        auto& mgr = MacroManager::get();

        bool show = Mod::get()->getSettingValue<bool>("show-overlay");
        float opacity = Mod::get()->getSettingValue<float>("overlay-opacity");

        this->setVisible(show && (mgr.isRecording() || mgr.isPlaying()));
        if (!this->isVisible()) return;

        m_bg->setOpacity(static_cast<GLubyte>(opacity * 200.f));

        if (mgr.isRecording()) {
            m_dot->setColor({ 255, 60, 60 });
            m_label->setString("REC");
            m_frames->setString(
                fmt::format("{}", mgr.getMacro().frames.size()).c_str()
            );
        } else if (mgr.isPlaying()) {
            m_dot->setColor({ 60, 255, 120 });
            m_label->setString("PLAY");
            m_frames->setString(
                fmt::format("{}/{}", mgr.getPlaybackFrame(), mgr.getTotalFrames()).c_str()
            );
        }

        // Blink dot while recording
        if (mgr.isRecording()) {
            float t = CCDirector::get()->getTotalFrames() % 60;
            m_dot->setOpacity(t < 30 ? 255 : 100);
        } else {
            m_dot->setOpacity(255);
        }
    }

private:
    CCScale9Sprite* m_bg     = nullptr;
    CCLabelBMFont*  m_dot    = nullptr;
    CCLabelBMFont*  m_label  = nullptr;
    CCLabelBMFont*  m_frames = nullptr;
};

} // namespace PeakMacro

// ─────────────────────────────────────────────────────────────────
//  PlayLayer hook — injects overlay into scene
// ─────────────────────────────────────────────────────────────────
class $modify(OverlayPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        auto* overlay = PeakMacro::MacroOverlay::create();
        if (overlay) {
            overlay->setPosition({ 8.f, CCDirector::get()->getWinSize().height - 22.f });
            overlay->setZOrder(100);
            this->addChild(overlay);
        }
        return true;
    }
};
