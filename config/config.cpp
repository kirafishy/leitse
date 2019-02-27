#include "config/config.h"
#include <fmt/printf.h>
#include <utils/paths.h>
#include <stack>
#include <vector>

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

bool Config::parse_args(int& argc, char** argv, bool allow_unkown)
{
    constexpr std::string_view help_string = "-help";

    std::vector<int> used_args_cont;
    used_args_cont.reserve(argc - 1);
    std::stack used_args{std::move(used_args_cont)};

    for (int i = 1; i < argc; ++i) {
        if (argv[i] == help_string)
            return true;
    }

    (void)argc;
    (void)argv;
    (void)allow_unkown;
    return false;
}

void Config::parse_global_config(std::string_view const& app_name)
{
    constexpr std::string_view file_name = "config.ini";

    auto parse_if_exists = [&](std::filesystem::path const& path) {
        if (std::filesystem::exists(path))
            parse_file(path.string());
    };

    parse_if_exists(utils::get_kiwixz_home(app_name) / file_name);
    parse_if_exists(file_name);
}

void Config::parse_file(std::string_view const& path, bool allow_unkown)
{
    (void)path;
    (void)allow_unkown;
}

void Config::show_help()
{
    fmt::print("help\n");
}

}  // namespace config
