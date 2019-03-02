#include "data_dragon.h"
#include <cpr/cpr.h>

namespace leitse {

std::vector<Champion> const& DataDragon::champions()
{
    return champions_;
}

void DataDragon::populate()
{
    fetch_data("champions.json");
}

nlohmann::json DataDragon::fetch_data(std::string_view const& data)
{
    cpr::Response response = cpr::Get(cpr::Url{ "https://api.github.com/repos/whoshuu/cpr/contributors" });
    printf("%s\n", response.text.c_str());
    return {};
}

}
