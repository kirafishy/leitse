#pragma once

#include <nlohmann/json.hpp>
#include <string_view>
#include <vector>

namespace leitse {

struct Champion {};


struct DataDragon {
    std::vector<Champion> const& champions();

    void populate();

private:
    std::vector<Champion> champions_;

    nlohmann::json fetch_data(std::string_view const& data);
};

}  // namespace leitse
