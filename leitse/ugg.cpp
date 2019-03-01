#include "ugg.h"

namespace leitse::aggregators {

std::vector<std::pair<std::string, std::string>> Ugg::get_options() const
{
    return {{"aggregator.ugg.enabled", "true"}};
}

std::vector<ItemSet> Ugg::get_itemsets(Champion const& champion) const
{
    (void)champion;
    return {};
}

}  // namespace leitse::aggregators
