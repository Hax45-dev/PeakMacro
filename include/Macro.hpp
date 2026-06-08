#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace PeakMacro {

    // ─────────────────────────────────────────────
    //  MacroFrame — one recorded input frame
    // ─────────────────────────────────────────────
    struct MacroFrame {
        int   frame   = 0;
        bool  player1 = false;
        bool  player2 = false;
        bool  hold1   = false;   // true = press, false = release
        bool  hold2   = false;
        float xPos    = 0.f;
        float yPos    = 0.f;

        // Serialise to JSON object
        matjson::Value toJSON() const {
            return matjson::makeObject({
                { "frame",   frame   },
                { "p1",      player1 },
                { "p2",      player2 },
                { "hold1",   hold1   },
                { "hold2",   hold2   },
                { "x",       xPos    },
                { "y",       yPos    }
            });
        }

        static MacroFrame fromJSON(matjson::Value const& v) {
            MacroFrame f;
            f.frame   = v["frame"].asInt().unwrapOr(0);
            f.player1 = v["p1"].asBool().unwrapOr(false);
            f.player2 = v["p2"].asBool().unwrapOr(false);
            f.hold1   = v["hold1"].asBool().unwrapOr(false);
            f.hold2   = v["hold2"].asBool().unwrapOr(false);
            f.xPos    = static_cast<float>(v["x"].asDouble().unwrapOr(0.0));
            f.yPos    = static_cast<float>(v["y"].asDouble().unwrapOr(0.0));
            return f;
        }
    };

    // ─────────────────────────────────────────────
    //  Macro — collection of frames + metadata
    // ─────────────────────────────────────────────
    struct Macro {
        std::string            name        = "Untitled";
        std::string            levelName   = "";
        std::string            author      = "Hax25-dev";
        std::string            createdAt   = "";
        int                    fps         = 60;
        int                    totalFrames = 0;
        std::vector<MacroFrame> frames;

        void clear() {
            frames.clear();
            totalFrames = 0;
        }

        bool isEmpty() const { return frames.empty(); }

        // Save to .peak file (JSON)
        bool saveToFile(std::string const& path) const {
            matjson::Array framesArr;
            framesArr.reserve(frames.size());
            for (auto const& f : frames)
                framesArr.push_back(f.toJSON());

            auto obj = matjson::makeObject({
                { "name",        name        },
                { "levelName",   levelName   },
                { "author",      author      },
                { "createdAt",   createdAt   },
                { "fps",         fps         },
                { "totalFrames", totalFrames },
                { "frames",      framesArr   }
            });

            std::ofstream file(path);
            if (!file.is_open()) return false;
            file << obj.dump(matjson::NO_INDENTATION);
            return true;
        }

        // Load from .peak file
        static std::optional<Macro> loadFromFile(std::string const& path) {
            std::ifstream file(path);
            if (!file.is_open()) return std::nullopt;

            std::string content((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());

            auto parsed = matjson::parse(content);
            if (!parsed) return std::nullopt;

            Macro m;
            m.name        = parsed->get<std::string>("name")      .unwrapOr("Untitled");
            m.levelName   = parsed->get<std::string>("levelName") .unwrapOr("");
            m.author      = parsed->get<std::string>("author")    .unwrapOr("Unknown");
            m.createdAt   = parsed->get<std::string>("createdAt") .unwrapOr("");
            m.fps         = parsed->get<int>("fps")               .unwrapOr(60);
            m.totalFrames = parsed->get<int>("totalFrames")       .unwrapOr(0);

            if (auto framesVal = parsed->get<matjson::Array>("frames")) {
                for (auto const& fv : *framesVal)
                    m.frames.push_back(MacroFrame::fromJSON(fv));
            }
            return m;
        }
    };

} // namespace PeakMacro
