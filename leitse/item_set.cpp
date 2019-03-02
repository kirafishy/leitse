#include "item_set.h"
#include <fmt/format.h>
#include <nlohmann/json.hpp>

namespace leitse {

void ItemSet::write(std::filesystem::path const& dir) const
{
    nlohmann::json blocks_json;
    for (Block const& block : blocks_) {
        nlohmann::json items_json;
        for (Item const& item : block.items) {
            nlohmann::json item_json;
            item_json["id"] = item.id;
            item_json["count"] = item.count;
            items_json.push_back(std::move(item_json));
        }

        nlohmann::json block_json;
        block_json["type"] = block.name;
        block_json["items"] = std::move(items_json);
        blocks_json.push_back(std::move(block_json));
    }

    nlohmann::json json;
    json["title"] = name_;
    json["type"] = "custom";
    json["map"] = map_to_string();
    json["mode"] = "any";
    json["blocks"] = std::move(blocks_json);
}

std::string ItemSet::map_to_string() const
{
    switch (map_) {
    case Map::any:
        return "any";
    case Map::summoners_rift:
        return "SR";
    case Map::howling_abyss:
        return "HA";
    case Map::twisted_treeline:
        return "TT";
    case Map::crystal_scar:
        return "CS";
    }
    throw std::runtime_error{fmt::format("unknown map id '{}'", static_cast<int>(map_))};
}

}  // namespace leitse
