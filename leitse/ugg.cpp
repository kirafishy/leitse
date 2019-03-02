#include "ugg.h"

namespace leitse::aggregators {

std::string_view Ugg::name() const
{
    static constexpr std::string_view name_static = "u.gg";
    return name_static;
}

std::vector<std::pair<std::string, std::string>> Ugg::options() const
{
    return {{"aggregator.ugg.enabled", "true"}};
}

std::vector<ItemSet> Ugg::itemsets(Champion const& champion) const
{
    (void)champion;
    return {};
}

}  // namespace leitse::aggregators
