#pragma once

#include "data_dragon.h"
#include "item_set.h"
#include <string>
#include <utility>
#include <vector>

namespace leitse {

struct Aggregator {
    virtual ~Aggregator() = default;

    virtual std::vector<std::pair<std::string, std::string>> get_options() const = 0;
    virtual std::vector<ItemSet> get_itemsets(Champion const& champion) const = 0;
};

}  // namespace leitse
