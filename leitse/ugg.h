#pragma once

#include "aggregator.h"

namespace leitse::aggregators {

struct Ugg : Aggregator {
    std::string const& name() const override;
    std::vector<std::pair<std::string, std::string>> options() const override;
    std::vector<ItemSet> itemsets(Champion const& champion) const override;
};

}  // namespace leitse::aggregators
