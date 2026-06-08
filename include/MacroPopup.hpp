#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

namespace PeakMacro {

// ─────────────────────────────────────────────────────────────────
//  MacroPopup  —  main PeakMacro UI popup
//  Tabs: Record | Saved Macros | Settings
// ─────────────────────────────────────────────────────────────────
class MacroPopup : public Popup<> {
public:
    static MacroPopup* create();

protected:
    bool setup() override;

private:
    // Tab pages
    CCNode* m_recordPage  = nullptr;
    CCNode* m_listPage    = nullptr;
    CCNode* m_settingsPage = nullptr;
    CCNode* m_activePage  = nullptr;

    // Status labels (updated per-frame)
    CCLabelBMFont* m_statusLabel  = nullptr;
    CCLabelBMFont* m_framesLabel  = nullptr;
    CCProgressBar* m_progressBar  = nullptr;

    // Tab buttons
    CCMenuItemSpriteExtra* m_tabRecord   = nullptr;
    CCMenuItemSpriteExtra* m_tabList     = nullptr;
    CCMenuItemSpriteExtra* m_tabSettings = nullptr;

    void buildTabBar(CCNode* parent);
    void buildRecordPage();
    void buildListPage();
    void buildSettingsPage();

    void showPage(CCNode* page);

    // Button callbacks
    void onTabRecord(CCObject*);
    void onTabList(CCObject*);
    void onTabSettings(CCObject*);

    void onRecord(CCObject*);
    void onStopRecord(CCObject*);
    void onPlay(CCObject*);
    void onStopPlay(CCObject*);
    void onSave(CCObject*);
    void onClear(CCObject*);

    void update(float dt) override;
    void refreshStatus();
    void refreshMacroList();
};

} // namespace PeakMacro
