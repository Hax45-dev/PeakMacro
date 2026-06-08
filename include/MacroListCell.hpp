#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace PeakMacro {

// ─────────────────────────────────────────────────────────────────
//  MacroListCell  —  one row in the "Load Macro" scroll list
// ─────────────────────────────────────────────────────────────────
class MacroListCell : public CCNode {
public:
    static MacroListCell* create(std::string const& path, float width);

    std::string const& getPath() const { return m_path; }

private:
    std::string m_path;

    bool init(std::string const& path, float width);

    void onLoad(CCObject*);
    void onDelete(CCObject*);
};

} // namespace PeakMacro
