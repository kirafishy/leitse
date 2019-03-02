#include "ugg.h"
#include <cpr/cpr.h>
#include <fmt/format.h>
#include <array>
#include <set>

namespace leitse::aggregators {

std::string const& Ugg::name() const
{
    static std::string name_static = "u.gg";
    return name_static;
}

std::vector<std::pair<std::string, std::string>> Ugg::options() const
{
    return {{"aggregator.ugg.enabled", "true"}};
}

std::vector<ItemSet> Ugg::itemsets(Champion const& champion) const
{
    struct ItemCandidate {
        int id;
        int wins;
        int matches;

        bool operator<(ItemCandidate const& other) const
        {
            if (matches != other.matches)
                return matches > other.matches;
            if (wins != other.wins)
                return wins > other.wins;
            return id > other.id;
        }
    };
    struct Role {
        std::string id;
        std::string name;
    };
    static const std::array<Role, 5> roles{{{"4", "Top"},
                                            {"1", "Jungle"},
                                            {"5", "Mid"},
                                            {"3", "ADC"},
                                            {"2", "Support"}}};

    cpr::Url url = fmt::format("https://stats2.u.gg/lol/1.1/table/items/9_4/ranked_solo_5x5/{}/1.2.5.json", champion.key);
    cpr::Response response = cpr::Get(url);
    if (response.status_code >= 400)
        throw std::runtime_error{"could not u.gg stats"};

    nlohmann::json data = nlohmann::json::parse(response.text).at("12").at("10");  // 12=world 10=platinum+
    std::vector<ItemSet> item_sets;
    std::set<int> added_items;
    std::set<ItemCandidate> item_candidates;
    for (Role const& role : roles) {
        if (role.id != "3")  // temporary
            continue;

        nlohmann::json const& data_role = data.at(role.id);
        ItemSet& item_set = item_sets.emplace_back(name(), ItemSet::Map::any, role.name);

        for (nlohmann::json const& data_block : data_role) {
            ItemSet::Block& block = item_set.blocks.emplace_back();

            for (nlohmann::json const& data_candidate : data_block) {
                nlohmann::json const& data_candidate_items = data_candidate.at(0);
                if (data_candidate_items.is_array()) {
                    int wins = data_candidate.at(1).get<int>();
                    int matches = data_candidate.at(2).get<int>();
                    for (nlohmann::json const& data_candidate_item : data_candidate.at(0)) {
                        int item_id = data_candidate_item.get<int>();
                        if (added_items.count(item_id) == 0)
                            item_candidates.insert({item_id, wins, matches});
                    }
                }
                else {
                    int item_id = data_candidate_items.get<int>();
                    if (added_items.count(item_id) == 0)
                        item_candidates.insert({item_id,
                                                data_candidate.at(1).get<int>(),
                                                data_candidate.at(2).get<int>()});
                }
            }

            for (int i = 0; i < 10 && !item_candidates.empty(); ++i) {
                int item_id = item_candidates.extract(item_candidates.begin()).value().id;
                block.items.push_back({item_id});
                added_items.insert(item_id);
            }
        }

        item_candidates.clear();
        item_set.blocks[0].name = "Starting items";
        item_set.blocks[1].name = "Boots";
        item_set.blocks[2].name = "First items";
        item_set.blocks[3].name = "Second items";
        item_set.blocks[4].name = "Third items";
        item_set.blocks[5].name = "Fourth items";
        item_set.blocks[6].name = "Fifth items";

        for (ItemSet::Block const& block : item_set.blocks) {
            fmt::print("{}: ", block.name);
            for (ItemSet::Item const& item : block.items)
                fmt::print("{}*{} ", item.id, item.count);
            fmt::print("\n");
        }
    }

    return item_sets;
}

}  // namespace leitse::aggregators
