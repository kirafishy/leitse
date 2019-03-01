#pragma once

#include "aggregator.h"

namespace leitse::aggregators {

struct Ugg : Aggregator {
    std::vector<std::pair<std::string, std::string>> get_options() const override;
    std::vector<ItemSet> get_itemsets(Champion const& champion) const override;
};

}  // namespace leitse::aggregators
