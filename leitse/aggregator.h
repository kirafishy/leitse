#pragma once

#include "data_dragon.h"
#include "item_set.h"
#include <string>
#include <utility>
#include <vector>

namespace leitse {

struct Aggregator {
    Aggregator() = default;
    virtual ~Aggregator();
    Aggregator(Aggregator const&) = delete;
    Aggregator& operator=(Aggregator const&) = delete;
    Aggregator(Aggregator&&) = delete;
    Aggregator& operator=(Aggregator&&) = delete;

    virtual std::string const& name() const = 0;
    virtual std::vector<std::pair<std::string, std::string>> options() const = 0;
    virtual std::vector<ItemSet> itemsets(Champion const& champion) const = 0;
};

}  // namespace leitse
