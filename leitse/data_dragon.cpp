#include "data_dragon.h"

namespace leitse {

std::vector<Champion> const& DataDragon::champions()
{
    return champions_;
}

void DataDragon::populate()
{
}

void DataDragon::fetch_url(std::string_view const& url)
{
}

void DataDragon::fetch_data(std::string_view const& data)
{
}

}
