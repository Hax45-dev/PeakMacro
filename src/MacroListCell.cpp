#include "MacroListCell.hpp"
#include "MacroManager.hpp"
#include <filesystem>

using namespace geode::prelude;
namespace fs = std::filesystem;

namespace PeakMacro {

MacroListCell* MacroListCell::create(std::string const& path, float width) {
    auto* ret = new MacroListCell();
    if (ret && ret->init(path, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MacroListCell::init(std::string const& path, float width) {
    if (!CCNode::init()) return false;

    m_path = path;

    // ── Background ───────────────────────────────────────────────
    auto* bg = CCScale9Sprite::create("GJ_square07.png", { 0, 0, 80, 80 });
    bg->setContentSize({ width - 10.f, 46.f });
    bg->setPosition({ (width - 10.f) / 2.f, 23.f });
    bg->setOpacity(180);
    this->addChild(bg);

    // ── Filename label ───────────────────────────────────────────
    auto name = fs::path(path).stem().string();
    auto* lbl = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
    lbl->setScale(0.45f);
    lbl->setAnchorPoint({ 0.f, 0.5f });
    lbl->setPosition({ 12.f, 23.f });
    lbl->setMaxLineWidth(width * 1.2f);
    this->addChild(lbl);

    // ── Load button ──────────────────────────────────────────────
    auto* loadBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Load", "goldFont.fnt", "GJ_button_01.png", 0.6f),
        this, menu_selector(MacroListCell::onLoad)
    );

    // ── Delete button ────────────────────────────────────────────
    auto* delBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Del", "bigFont.fnt", "GJ_button_06.png", 0.6f),
        this, menu_selector(MacroListCell::onDelete)
    );

    auto* menu = CCMenu::create(loadBtn, delBtn, nullptr);
    menu->setLayout(RowLayout::create()->setGap(6.f));
    menu->setPosition({ width - 70.f, 23.f });
    menu->updateLayout();
    this->addChild(menu);

    this->setContentSize({ width, 50.f });
    return true;
}

void MacroListCell::onLoad(CCObject*) {
    MacroManager::get().loadMacro(m_path);
}

void MacroListCell::onDelete(CCObject*) {
    createQuickPopup(
        "Delete Macro",
        "Are you sure you want to <cr>delete</c> this macro?",
        "Cancel", "Delete",
        [this](auto*, bool del) {
            if (!del) return;
            std::filesystem::remove(m_path);
            Notification::create("PeakMacro", "Macro deleted.", NotificationIcon::Success)->show();
            // Refresh parent layer – walk up to find a reload method if available
            if (auto* parent = this->getParent())
                parent->removeFromParent();
        }
    );
}

} // namespace PeakMacro
