#include "item_set.h"
#include <fmt/format.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>

namespace leitse {

ItemSet::ItemSet(std::string aggregator, Map map, std::string role) :
    aggregator_{std::move(aggregator)},
    map_{map},
    role_{std::move(role)}
{}

void ItemSet::write(std::filesystem::path const& dir) const
{
    nlohmann::json blocks_json;
    for (Block const& block : blocks) {
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

    std::string map_str = map_to_string();
    time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string date = (std::stringstream{} << std::put_time(std::localtime(&time), "%Y-%m-%d")).str();

    nlohmann::json json;
    json["title"] = fmt::format("{} {} {} ({})", map_str, aggregator_, role_, date);
    json["type"] = "custom";
    json["map"] = map_str;
    json["mode"] = "any";
    json["blocks"] = std::move(blocks_json);

    std::ofstream{dir / fmt::format("{}_{}_{}.json", map_str, aggregator_, role_)} << json.dump();
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
