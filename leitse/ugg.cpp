#include "ugg.h"
#include "download.h"
#include <fmt/format.h>
#include <algorithm>
#include <array>
#include <set>

namespace leitse::aggregators {
namespace {

struct ItemCandidate {
    int id;
    int wins;
    int matches;
};


struct Role {
    std::string id;
    std::string name;
};

}  // namespace


Ugg::Ugg()
{
    std::string response = simple_download("https://ddragon.leagueoflegends.com/api/versions.json");
    league_version_ = nlohmann::json::parse(response)[0];
    size_t dot = league_version_.find('.');
    league_version_[dot] = '_';
    league_version_.resize(league_version_.find('.', dot + 1));

    items_version_ = "1.2.6";

    response = simple_download(fmt::format("https://stats2.u.gg/lol/1.1/primary_roles/{}/{}.json",
                                           league_version_, items_version_));
    primary_roles_ = nlohmann::json::parse(response);
}

std::string const& Ugg::name() const
{
    static const std::string name_static = "u.gg";
    return name_static;
}

std::vector<std::pair<std::string, std::string>> Ugg::options() const
{
    return {{"aggregator.ugg.enabled", "true"}};
}

std::vector<ItemSet> Ugg::itemsets(Champion const& champion) const
{
    static const std::array<Role, 5> roles{{{"1", "Jungle"},
                                            {"2", "Support"},
                                            {"3", "ADC"},
                                            {"4", "Top"},
                                            {"5", "Mid"}}};

    std::string response = simple_download(fmt::format("https://stats2.u.gg/lol/1.1/table/items/{}/ranked_solo_5x5/{}/{}.json",
                                                       league_version_, champion.key, items_version_));
    nlohmann::json data = nlohmann::json::parse(response).at("12").at("10");  // 12=world 10=platinum+
    std::vector<ItemSet> item_sets;

    ItemSet::Block miscellaneous_block;
    miscellaneous_block.name = "Miscellaneous";
    miscellaneous_block.items.push_back({3340});  // yellow trinket
    miscellaneous_block.items.push_back({3364});  // red trinket
    miscellaneous_block.items.push_back({3363});  // blue trinket
    miscellaneous_block.items.push_back({2055});  // control ward
    miscellaneous_block.items.push_back({2003});  // health potion
    miscellaneous_block.items.push_back({2140});  // ad elixir
    miscellaneous_block.items.push_back({2139});  // ap elixir
    miscellaneous_block.items.push_back({2138});  // tank elixir
    miscellaneous_block.items.push_back({2047});  // oracle lens

    for (Role const& role : roles) {
        nlohmann::json const& data_role = data.at(role.id);
        ItemSet& item_set = item_sets.emplace_back(name(), ItemSet::Map::any, role.name);

        std::set<int> added_items;
        std::vector<ItemCandidate> item_candidates;
        auto add_candidate = [&](ItemCandidate&& candidate) {
            if (added_items.count(candidate.id) != 0)
                return false;
            auto it = std::find_if(item_candidates.begin(), item_candidates.end(), [&](ItemCandidate const& e) {
                return e.id == candidate.id;
            });
            if (it != item_candidates.end()) {
                it->wins += candidate.wins;
                it->matches += candidate.matches;
            }
            else
                item_candidates.push_back(std::move(candidate));
            return true;
        };

        auto add_items_to_block = [&](ItemSet::Block& block, nlohmann::json const& data_block) {
            int total_matches = 1;

            for (nlohmann::json const& data_candidate : data_block) {
                nlohmann::json const& data_candidate_items = data_candidate.at(0);
                if (data_candidate_items.is_array()) {
                    int wins = data_candidate.at(1).get<int>();
                    int matches = data_candidate.at(2).get<int>();
                    for (nlohmann::json const& data_candidate_item : data_candidate.at(0))
                        if (add_candidate({data_candidate_item.get<int>(), wins, matches}))
                            total_matches += matches;
                }
                else {
                    int matches = data_candidate.at(2).get<int>();
                    if (add_candidate({data_candidate_items.get<int>(),
                                       data_candidate.at(1).get<int>(),
                                       matches}))
                        total_matches += matches;
                }
            }

            std::sort(item_candidates.begin(), item_candidates.end(), [](ItemCandidate const& lhs, ItemCandidate const& rhs) {
                if (lhs.matches != rhs.matches)
                    return lhs.matches < rhs.matches;
                return lhs.wins < rhs.wins;
            });
            while (!item_candidates.empty()) {
                ItemCandidate item = std::move(item_candidates.back());
                item_candidates.pop_back();
                block.items.push_back({item.id, std::max(100 * item.matches / total_matches, 1)});
                added_items.insert(item.id);
            }
        };

        ItemSet::Block* block = &item_set.blocks.emplace_back();
        block->name = "Starters & boots";
        add_items_to_block(*block, data_role.at(0));
        add_items_to_block(*block, data_role.at(2));

        block = &item_set.blocks.emplace_back();
        block->name = "First items";
        add_items_to_block(*block, data_role.at(1));

        block = &item_set.blocks.emplace_back();
        block->name = "Second items";
        add_items_to_block(*block, data_role.at(3));

        block = &item_set.blocks.emplace_back();
        block->name = "Other items";
        add_items_to_block(*block, data_role.at(4));
        add_items_to_block(*block, data_role.at(5));
        add_items_to_block(*block, data_role.at(6));
        std::sort(block->items.begin(), block->items.end(), [](ItemSet::Item const& lhs, ItemSet::Item const& rhs) {
            return lhs.count > rhs.count;
        });

        item_set.blocks.push_back(miscellaneous_block);
    }

    for (unsigned i = 0; i < 5; ++i)
        item_sets.at(primary_roles_.at(champion.key).at(i).get<int>() - 1).priority = 14 - i;

    return item_sets;
}

}  // namespace leitse::aggregators
