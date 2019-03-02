#pragma once

#include <vector>
#include <string_view>

namespace leitse {

struct Champion {};


struct DataDragon {
    std::vector<Champion> const& champions();

    void populate();

private:
    std::vector<Champion> champions_;

    void fetch_url(std::string_view const& url);
    void fetch_data(std::string_view const& data);
};

}  // namespace leitse
