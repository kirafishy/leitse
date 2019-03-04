#pragma once

#include "aggregator.h"
#include <nlohmann/json.hpp>

namespace leitse::aggregators {

struct Ugg : Aggregator {
    Ugg();

    std::string const& name() const override;
    std::vector<std::pair<std::string, std::string>> options() const override;
    std::vector<ItemSet> itemsets(Champion const& champion) const override;

private:
    std::string league_version_;
    std::string items_version_;
    nlohmann::json primary_roles_;
};

}  // namespace leitse::aggregators
