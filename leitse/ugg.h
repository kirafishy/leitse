#pragma once

#include "aggregator.h"

namespace leitse::aggregators {

struct Ugg : Aggregator {
    virtual std::vector<std::pair<std::string, std::string>> get_options() const override;
    virtual std::vector<ItemSet> get_itemsets(Champion const& champion) const override;
};

}  // namespace leitse::aggregators
