#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <vector>

namespace leitse {

struct Champion {
    std::string key;
    std::string id;
    std::string name;

    Champion() = default;
    explicit Champion(nlohmann::json const& json);
};


struct DataDragon {
    std::vector<Champion> const& champions();

    void populate();

private:
    std::string version_;
    std::vector<Champion> champions_;

    void update_version();
    nlohmann::json fetch_data(std::string_view filename);
};

}  // namespace leitse
