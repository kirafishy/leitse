#include "ugg.h"
#include <cpr/cpr.h>
#include <fmt/format.h>
#include <array>
#include <set>

namespace leitse::aggregators {
namespace {

struct ItemCandidate {
    int id;
    int wins;
    int matches;
};


bool operator<(ItemCandidate const& lhs, ItemCandidate const& rhs)  // sort most interesting items first
{
    if (lhs.matches != rhs.matches)
        return lhs.matches > rhs.matches;
    if (lhs.wins != rhs.wins)
        return lhs.wins > rhs.wins;
    return lhs.id > rhs.id;
}


struct ItemCandidateComparator {
    using is_transparent = std::true_type;

    bool operator()(ItemCandidate const& lhs, ItemCandidate const& rhs) const
    {
        return lhs < rhs;
    }

    bool operator()(ItemCandidate const& lhs, int rhs) const
    {
        return lhs.id > rhs;
    }

    bool operator()(int lhs, ItemCandidate const& rhs) const
    {
        return lhs > rhs.id;
    }
};


struct Role {
    std::string id;
    std::string name;
};

}  // namespace


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
    std::set<ItemCandidate, ItemCandidateComparator> item_candidates;

    auto add_candidate = [&](ItemCandidate candidate) {
        if (added_items.count(candidate.id) != 0)
            return;
        auto it = item_candidates.find(candidate.id);  // we are comparing only ids
        if (it != item_candidates.end())
            if (candidate < *it)
                item_candidates.erase(it);
            else
                return;
        item_candidates.insert(std::move(candidate));
    };

    for (Role const& role : roles) {
        if (role.id != "3")  // temporary
            continue;

        nlohmann::json const& data_role = data.at(role.id);
        ItemSet& item_set = item_sets.emplace_back(name(), ItemSet::Map::any, role.name);

        auto add_items_to_block = [&](ItemSet::Block& block,
                                      nlohmann::json const& data_block,
                                      bool add_candidates = true) {
            int total_matches = 0;

            for (nlohmann::json const& data_candidate : data_block) {
                nlohmann::json const& data_candidate_items = data_candidate.at(0);
                if (data_candidate_items.is_array()) {
                    int wins = data_candidate.at(1).get<int>();
                    int matches = data_candidate.at(2).get<int>();
                    for (nlohmann::json const& data_candidate_item : data_candidate.at(0))
                        add_candidate({data_candidate_item.get<int>(), wins, matches});
                    total_matches += matches;
                }
                else {
                    int matches = data_candidate.at(2).get<int>();
                    add_candidate({data_candidate_items.get<int>(),
                                   data_candidate.at(1).get<int>(),
                                   matches});
                    total_matches += matches;
                }
            }

            while (!item_candidates.empty()) {
                ItemCandidate item = item_candidates.extract(item_candidates.begin()).value();
                block.items.push_back({item.id, std::min(100 * item.matches / total_matches, 1)});
                added_items.insert(item.id);
            }
        };

        ItemSet::Block* block = &item_set.blocks.emplace_back();
        block->name = "Starters & boots";
        add_items_to_block(*block, data_role.at(0));
        add_items_to_block(*block, data_role.at(2));
        item_candidates.clear();

        block = &item_set.blocks.emplace_back();
        block->name = "First items";
        add_items_to_block(*block, data_role.at(1));

        block = &item_set.blocks.emplace_back();
        block->name = "Second items";
        add_items_to_block(*block, data_role.at(3));

        block = &item_set.blocks.emplace_back();
        block->name = "Other items";
        add_items_to_block(*block, data_role.at(4), false);
        add_items_to_block(*block, data_role.at(5), false);
        add_items_to_block(*block, data_role.at(6));

        item_candidates.clear();

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
