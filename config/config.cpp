#include "config/config.h"

namespace config {

std::string& Config::operator[](std::string const& key)
{
    return map_[key];
}

std::string& Config::at(std::string const& key)
{
    return map_.at(key);
}

std::string const& Config::at(std::string const& key) const
{
    return map_.at(key);
}

bool Config::exists(std::string const& key)
{
    return map_.count(key) > 0;
}

void Config::remove(std::string const& key)
{
    if (map_.erase(key) == 0)
        throw std::runtime_error{"key not present"};
}

void Config::clear()
{
    map_.clear();
}

int Config::parse(int argc, char const* const* argv, bool allow_unkown)
{
    (void)argc;
    (void)argv;
    (void)allow_unkown;
    return 0;
}

void Config::parse_file(std::string_view const& path, bool allow_unkown)
{
    (void)path;
    (void)allow_unkown;
}

}  // namespace config
