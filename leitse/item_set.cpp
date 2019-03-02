#include "item_set.h"
#include <date/date.h>
#include <fmt/format.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>

namespace leitse {

ItemSet::ItemSet(std::string aggregator, Map map, std::string role) :
    aggregator_{std::move(aggregator)},
    role_{std::move(role)},
    map_{map}
{}

void ItemSet::write(std::filesystem::path const& dir) const
{
    nlohmann::json blocks_json;
    for (Block const& block : blocks) {
        nlohmann::json items_json;
        for (Item const& item : block.items) {
            nlohmann::json item_json;
            item_json["id"] = std::to_string(item.id);
            item_json["count"] = item.count;
            items_json.push_back(std::move(item_json));
        }

        nlohmann::json block_json;
        block_json["type"] = block.name;
        block_json["items"] = std::move(items_json);
        blocks_json.push_back(std::move(block_json));
    }

    std::string map_str = map_to_string();
    std::stringstream date_ss;
    date_ss << date::year_month_day{date::floor<date::days>(std::chrono::system_clock::now())};
    std::string date = date_ss.str();

    nlohmann::json json;
    json["title"] = fmt::format("{} {} ({})", aggregator_, role_, date);
    json["type"] = "custom";
    json["map"] = map_str;
    json["mode"] = "any";
    json["blocks"] = std::move(blocks_json);

    std::filesystem::path path = dir / fmt::format("leitse_{}_{}_{}.json", map_str, aggregator_, role_);
    std::ofstream ofs{path};
    if (!ofs)
        throw std::runtime_error{fmt::format("could not open item set: {}", path.string())};
    ofs << json.dump();
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
