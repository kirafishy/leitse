#include "data_dragon.h"
#include <cpr/cpr.h>
#include <spdlog/spdlog.h>

#include <iostream>

namespace leitse {

Champion::Champion(nlohmann::json const& json)
{
    json.at("key").get_to(key);
    json.at("id").get_to(key);
    json.at("name").get_to(name);
}


std::vector<Champion> const& DataDragon::champions()
{
    return champions_;
}

void DataDragon::populate()
{
    update_version();

    nlohmann::json champions_json = fetch_data("champion");
    nlohmann::json const& champions_data_json = champions_json.at("data");
    champions_.clear();
    champions_.reserve(champions_data_json.size());
    for (auto const& champion_json : champions_json.at("data"))
        champions_.emplace_back(champion_json);
    spdlog::info("[data_dragon] found {} champions", champions_.size());
}

void DataDragon::update_version()
{
    cpr::Response response = cpr::Get(cpr::Url{"https://ddragon.leagueoflegends.com/api/versions.json"});
    if (response.status_code >= 400)
        throw std::runtime_error{"could not update data dragon version"};
    nlohmann::json json = nlohmann::json::parse(response.text);
    version_ = json[0].get<std::string>();
    spdlog::info("[data_dragon] using version {}", version_);
}

nlohmann::json DataDragon::fetch_data(std::string_view filename)
{
    cpr::Url url = fmt::format("http://ddragon.leagueoflegends.com/cdn/{}/data/en_US/{}.json", version_, filename);
    cpr::Response response = cpr::Get(url);
    if (response.status_code >= 400)
        throw std::runtime_error{fmt::format("could not update get '{}' from data dragon", filename)};
    return nlohmann::json::parse(response.text);
}

}  // namespace leitse
