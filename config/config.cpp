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

bool Config::contains(std::string const& key)
{
    return map_.count(key) > 0;
}

void Config::erase(std::string const& key)
{
    if (map_.erase(key) == 0)
        throw std::runtime_error{"key not present"};
}

void Config::clear()
{
    map_.clear();
}

bool Config::parse_args(int& argc, char** argv, bool allow_unknown)
{
    std::vector<int> used_args_cont;
    used_args_cont.reserve(argc - 1);
    std::stack used_args{std::move(used_args_cont)};

    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            std::string_view arg = argv[i] + 1;
            if (arg == "-")
                break;
            if (arg == "-help")
                return true;

            size_t eq = arg.find('=');
            std::string key{arg.substr(0, eq)};
            std::string value;
            if (eq == std::string::npos)
                value = "true";
            else
                value = std::string{arg.substr(eq + 1)};

            auto it = map_.find(key);
            if (it == map_.end()) {
                if (allow_unknown)
                    map_.insert({std::move(key), std::move(value)});
                else
                    throw std::runtime_error{fmt::format("unknown option '{}'", key)};
            }
            else
                it->second = std::move(value);
        }
        else if (argv[i][0] == '+')
            parse_file(argv[i] + 1, allow_unknown);
    }
    return false;
}

void Config::parse_global_config(std::string_view const& app_name)
{
    auto parse_if_exists = [&](std::filesystem::path const& path) {
        if (std::filesystem::exists(path))
            parse_file(path.string());
    };

    constexpr std::string_view file_name = "config.ini";
    parse_if_exists(utils::get_kiwixz_home(app_name) / file_name);
    parse_if_exists(file_name);
}

void Config::parse_file(std::string_view const& path, bool allow_unknown)
{
    (void)path;
    (void)allow_unknown;
}

void Config::show_help()
{
    fmt::print("help\n");
}

}  // namespace config
