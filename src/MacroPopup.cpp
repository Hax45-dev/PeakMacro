#include "MacroPopup.hpp"
#include "MacroManager.hpp"
#include "MacroListCell.hpp"
#include <Geode/ui/ScrollLayer.hpp>

using namespace geode::prelude;

namespace PeakMacro {

static constexpr float POP_W = 420.f;
static constexpr float POP_H = 280.f;

// ─────────────────────────────────────────────────────────────────
MacroPopup* MacroPopup::create() {
    auto* ret = new MacroPopup();
    if (ret && ret->initAnchored(POP_W, POP_H)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

// ─────────────────────────────────────────────────────────────────
bool MacroPopup::setup() {
    this->setTitle("PeakMacro v1.0.0");

    // ── Logo icon ─────────────────────────────────────────────────
    auto* logo = CCSprite::create("peak_logo.png"_spr);
    if (logo) {
        logo->setScale(0.45f);
        logo->setPosition({ 38.f, POP_H - 16.f });
        m_mainLayer->addChild(logo);
    }

    // ── Credit label ─────────────────────────────────────────────
    auto* credit = CCLabelBMFont::create("by @Hax25-dev", "chatFont.fnt");
    credit->setScale(0.55f);
    credit->setColor({ 180, 220, 255 });
    credit->setAnchorPoint({ 1.f, 1.f });
    credit->setPosition({ POP_W - 10.f, POP_H - 5.f });
    m_mainLayer->addChild(credit);

    // ── Tabs ──────────────────────────────────────────────────────
    buildTabBar(m_mainLayer);

    // ── Pages ─────────────────────────────────────────────────────
    buildRecordPage();
    buildListPage();
    buildSettingsPage();

    // Default page
    showPage(m_recordPage);

    // Per-frame update for live status
    this->scheduleUpdate();

    return true;
}

// ─────────────────────────────────────────────────────────────────
//  Tab Bar
// ─────────────────────────────────────────────────────────────────
void MacroPopup::buildTabBar(CCNode* parent) {
    auto makeTab = [](std::string const& label, SEL_MenuHandler sel, MacroPopup* self) {
        auto* sprite = ButtonSprite::create(label.c_str(), "bigFont.fnt", "GJ_button_04.png", 0.7f);
        return CCMenuItemSpriteExtra::create(sprite, self, sel);
    };

    m_tabRecord   = makeTab("Record",  menu_selector(MacroPopup::onTabRecord),   this);
    m_tabList     = makeTab("Macros",  menu_selector(MacroPopup::onTabList),     this);
    m_tabSettings = makeTab("Settings",menu_selector(MacroPopup::onTabSettings), this);

    auto* menu = CCMenu::create(m_tabRecord, m_tabList, m_tabSettings, nullptr);
    menu->setLayout(RowLayout::create()->setGap(8.f));
    menu->setPosition({ POP_W / 2.f, POP_H - 38.f });
    menu->updateLayout();
    parent->addChild(menu);
}

// ─────────────────────────────────────────────────────────────────
//  Record Page
// ─────────────────────────────────────────────────────────────────
void MacroPopup::buildRecordPage() {
    m_recordPage = CCNode::create();
    m_recordPage->setContentSize({ POP_W, POP_H });
    m_mainLayer->addChild(m_recordPage);

    float cx = POP_W / 2.f;
    float cy = POP_H / 2.f - 10.f;

    // ── Status label ──────────────────────────────────────────────
    m_statusLabel = CCLabelBMFont::create("IDLE", "bigFont.fnt");
    m_statusLabel->setScale(0.65f);
    m_statusLabel->setPosition({ cx, cy + 60.f });
    m_statusLabel->setColor({ 255, 255, 100 });
    m_recordPage->addChild(m_statusLabel);

    // ── Frames label ─────────────────────────────────────────────
    m_framesLabel = CCLabelBMFont::create("Frames: 0", "chatFont.fnt");
    m_framesLabel->setScale(0.6f);
    m_framesLabel->setPosition({ cx, cy + 38.f });
    m_recordPage->addChild(m_framesLabel);

    // ── Progress bar ─────────────────────────────────────────────
    m_progressBar = CCProgressBar::create();
    m_progressBar->setPosition({ cx, cy + 18.f });
    m_progressBar->setScale(1.1f);
    m_recordPage->addChild(m_progressBar);

    // ── Action buttons ───────────────────────────────────────────
    auto makeBtn = [](std::string const& lbl, std::string const& skin,
                      SEL_MenuHandler sel, MacroPopup* self) {
        return CCMenuItemSpriteExtra::create(
            ButtonSprite::create(lbl.c_str(), "goldFont.fnt", skin.c_str(), 0.8f),
            self, sel
        );
    };

    auto* btnRecord = makeBtn("● REC",  "GJ_button_06.png", menu_selector(MacroPopup::onRecord),     this);
    auto* btnStop   = makeBtn("■ STOP", "GJ_button_02.png", menu_selector(MacroPopup::onStopRecord), this);
    auto* btnPlay   = makeBtn("▶ PLAY", "GJ_button_01.png", menu_selector(MacroPopup::onPlay),       this);
    auto* btnStopPl = makeBtn("■ STOP", "GJ_button_02.png", menu_selector(MacroPopup::onStopPlay),   this);
    auto* btnSave   = makeBtn("💾 Save","GJ_button_04.png", menu_selector(MacroPopup::onSave),        this);
    auto* btnClear  = makeBtn("🗑 Clear","GJ_button_05.png", menu_selector(MacroPopup::onClear),      this);

    auto* row1 = CCMenu::create(btnRecord, btnStop, nullptr);
    row1->setLayout(RowLayout::create()->setGap(10.f));
    row1->setPosition({ cx, cy - 14.f });
    row1->updateLayout();
    m_recordPage->addChild(row1);

    auto* row2 = CCMenu::create(btnPlay, btnStopPl, nullptr);
    row2->setLayout(RowLayout::create()->setGap(10.f));
    row2->setPosition({ cx, cy - 48.f });
    row2->updateLayout();
    m_recordPage->addChild(row2);

    auto* row3 = CCMenu::create(btnSave, btnClear, nullptr);
    row3->setLayout(RowLayout::create()->setGap(10.f));
    row3->setPosition({ cx, cy - 80.f });
    row3->updateLayout();
    m_recordPage->addChild(row3);
}

// ─────────────────────────────────────────────────────────────────
//  Macro List Page
// ─────────────────────────────────────────────────────────────────
void MacroPopup::buildListPage() {
    m_listPage = CCNode::create();
    m_listPage->setContentSize({ POP_W, POP_H });
    m_listPage->setVisible(false);
    m_mainLayer->addChild(m_listPage);

    refreshMacroList();
}

void MacroPopup::refreshMacroList() {
    m_listPage->removeAllChildren();

    auto macros = MacroManager::get().getSavedMacros();
    float listW = POP_W - 30.f;

    if (macros.empty()) {
        auto* lbl = CCLabelBMFont::create("No macros saved yet.", "chatFont.fnt");
        lbl->setScale(0.7f);
        lbl->setPosition({ POP_W / 2.f, POP_H / 2.f });
        m_listPage->addChild(lbl);
        return;
    }

    // Build scroll list
    auto* scroll = ScrollLayer::create({ listW, POP_H - 80.f });
    scroll->setPosition({ 15.f, 30.f });

    float yOff = 0.f;
    for (auto it = macros.rbegin(); it != macros.rend(); ++it) {
        auto* cell = MacroListCell::create(*it, listW);
        if (!cell) continue;
        cell->setPositionY(yOff);
        scroll->m_contentLayer->addChild(cell);
        yOff += 54.f;
    }

    scroll->m_contentLayer->setContentHeight(yOff);
    scroll->moveToTop();
    m_listPage->addChild(scroll);

    // Refresh button
    auto* refreshBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Refresh", "bigFont.fnt", "GJ_button_04.png", 0.65f),
        this,
        menu_selector(MacroPopup::onTabList)
    );
    auto* menu = CCMenu::create(refreshBtn, nullptr);
    menu->setPosition({ POP_W / 2.f, 18.f });
    m_listPage->addChild(menu);
}

// ─────────────────────────────────────────────────────────────────
//  Settings Page
// ─────────────────────────────────────────────────────────────────
void MacroPopup::buildSettingsPage() {
    m_settingsPage = CCNode::create();
    m_settingsPage->setContentSize({ POP_W, POP_H });
    m_settingsPage->setVisible(false);
    m_mainLayer->addChild(m_settingsPage);

    float cx = POP_W / 2.f;
    float startY = POP_H - 90.f;
    float gap = 36.f;

    auto addRow = [&](std::string const& text, float y) {
        auto* lbl = CCLabelBMFont::create(text.c_str(), "chatFont.fnt");
        lbl->setScale(0.62f);
        lbl->setAnchorPoint({ 0.f, 0.5f });
        lbl->setPosition({ 30.f, y });
        m_settingsPage->addChild(lbl);
    };

    addRow("Show Overlay",           startY);
    addRow("Auto Save After Record",  startY - gap);
    addRow("FPS Bypass on Playback",  startY - gap * 2.f);

    // ── Open Geode settings shortcut ─────────────────────────────
    auto* openBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Open Geode Settings", "bigFont.fnt", "GJ_button_04.png", 0.65f),
        this,
        [](CCObject*) {
            // Open Geode mod settings overlay
            geode::openSettingsPopup(Mod::get());
        }
    );
    auto* menu = CCMenu::create(openBtn, nullptr);
    menu->setPosition({ cx, 40.f });
    m_settingsPage->addChild(menu);

    // Credits section
    auto* credLbl = CCLabelBMFont::create(
        "PeakMacro v1.0.0  |  @Hax25-dev", "chatFont.fnt");
    credLbl->setScale(0.5f);
    credLbl->setColor({ 150, 200, 255 });
    credLbl->setPosition({ cx, 18.f });
    m_settingsPage->addChild(credLbl);
}

// ─────────────────────────────────────────────────────────────────
//  Tab switching
// ─────────────────────────────────────────────────────────────────
void MacroPopup::showPage(CCNode* page) {
    if (m_recordPage)   m_recordPage->setVisible(false);
    if (m_listPage)     m_listPage->setVisible(false);
    if (m_settingsPage) m_settingsPage->setVisible(false);
    if (page) page->setVisible(true);
    m_activePage = page;
}

void MacroPopup::onTabRecord(CCObject*)   { showPage(m_recordPage);   }
void MacroPopup::onTabList(CCObject*)     { refreshMacroList(); showPage(m_listPage); }
void MacroPopup::onTabSettings(CCObject*) { showPage(m_settingsPage); }

// ─────────────────────────────────────────────────────────────────
//  Record / Play callbacks
// ─────────────────────────────────────────────────────────────────
void MacroPopup::onRecord(CCObject*) {
    auto& mgr = MacroManager::get();
    if (!PlayLayer::get()) {
        FLAlertLayer::create("PeakMacro", "Enter a level first!", "OK")->show();
        return;
    }
    if (mgr.isRecording()) return;
    mgr.startRecording();
    Notification::create("PeakMacro", "Recording started!", NotificationIcon::Success)->show();
}

void MacroPopup::onStopRecord(CCObject*) {
    auto& mgr = MacroManager::get();
    if (!mgr.isRecording()) return;
    mgr.stopRecording();
    refreshStatus();
}

void MacroPopup::onPlay(CCObject*) {
    auto& mgr = MacroManager::get();
    if (!PlayLayer::get()) {
        FLAlertLayer::create("PeakMacro", "Enter a level first!", "OK")->show();
        return;
    }
    if (mgr.getMacro().isEmpty()) {
        FLAlertLayer::create("PeakMacro", "No macro loaded! Load one from the <cy>Macros</c> tab.", "OK")->show();
        return;
    }
    mgr.startPlayback();
    // Reset level to start from beginning
    if (auto* pl = PlayLayer::get())
        pl->resetLevel();
}

void MacroPopup::onStopPlay(CCObject*) {
    MacroManager::get().stopPlayback();
}

void MacroPopup::onSave(CCObject*) {
    auto& mgr = MacroManager::get();
    if (mgr.getMacro().isEmpty()) {
        FLAlertLayer::create("PeakMacro", "Nothing to save!", "OK")->show();
        return;
    }

    // Simple name input via Geode's createQuickPopup isn't ideal — use text input
    auto input = geode::TextInput::create(260.f, "Macro Name");
    // Save with default name
    mgr.saveMacro();
}

void MacroPopup::onClear(CCObject*) {
    createQuickPopup(
        "Clear Macro",
        "This will <cr>clear</c> the current macro from memory. Are you sure?",
        "Cancel", "Clear",
        [](auto*, bool yes) {
            if (yes) MacroManager::get().clearMacro();
        }
    );
}

// ─────────────────────────────────────────────────────────────────
//  Per-frame status refresh
// ─────────────────────────────────────────────────────────────────
void MacroPopup::update(float dt) {
    Popup<>::update(dt);
    refreshStatus();
}

void MacroPopup::refreshStatus() {
    auto& mgr = MacroManager::get();

    if (!m_statusLabel || !m_framesLabel) return;

    if (mgr.isRecording()) {
        m_statusLabel->setString("● RECORDING");
        m_statusLabel->setColor({ 255, 80, 80 });
    } else if (mgr.isPlaying()) {
        m_statusLabel->setString("▶ PLAYING");
        m_statusLabel->setColor({ 80, 255, 120 });
    } else {
        m_statusLabel->setString("■ IDLE");
        m_statusLabel->setColor({ 255, 255, 100 });
    }

    auto total = mgr.getTotalFrames();
    auto current = mgr.getPlaybackFrame();

    m_framesLabel->setString(
        fmt::format("Frames: {} / {}", current, total).c_str()
    );

    if (m_progressBar && total > 0)
        m_progressBar->setPercentage(mgr.getProgress() * 100.f);
}

} // namespace PeakMacro
