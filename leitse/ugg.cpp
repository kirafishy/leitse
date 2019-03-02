#include "ugg.h"

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
    (void)champion;
    std::vector<ItemSet> item_sets;
    item_sets.emplace_back(std::string{name()}, ItemSet::Map::any, "tmp");
    return item_sets;
}

}  // namespace leitse::aggregators
