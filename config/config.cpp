#include "config/config.h"
#include <fmt/printf.h>
#include <utils/paths.h>
#include <algorithm>
#include <vector>

namespace config {

std::string& Config::operator[](std::string const& key)
{
    return options_[key];
}

std::string& Config::at(std::string const& key)
{
    return options_.at(key);
}

std::string const& Config::at(std::string const& key) const
{
    return options_.at(key);
}

bool Config::contains(std::string const& key)
{
    return options_.count(key) > 0;
}

void Config::erase(std::string const& key)
{
    if (options_.erase(key) == 0)
        throw std::runtime_error{"key not present"};
}

void Config::clear()
{
    options_.clear();
}

bool Config::parse_args(int& argc, char** argv, bool allow_unknown)
{
    std::vector<int> used_args;
    used_args.reserve(argc - 1);

    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            used_args.push_back(i);
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

            auto it = options_.find(key);
            if (it == options_.end()) {
                if (allow_unknown)
                    options_.insert({std::move(key), std::move(value)});
                else
                    throw std::runtime_error{fmt::format("unknown option '{}'", key)};
            }
            else
                it->second = std::move(value);
        }
        else if (argv[i][0] == '+') {
            used_args.push_back(i);
            parse_file(argv[i] + 1, allow_unknown);
        }
    }

    if (used_args.size() == 0)
        return false;

    argc -= static_cast<int>(used_args.size());
    int old_index = used_args[0] + 1;
    int new_index = used_args[0];
    for (unsigned i = 1; i < used_args.size(); ++i)
    {
        for (; old_index < used_args[i]; ++old_index, ++new_index)
            argv[new_index] = argv[old_index];
        ++old_index;
    }
    for (; new_index <= argc; ++old_index, ++new_index)
        argv[new_index] = argv[old_index];

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

void Config::show_help(std::string_view const& app_name)
{
    fmt::print("Usage: {} [--help] [+config_file] [options] [--] [positional arguments]\n", app_name);
    fmt::print("Current options and their default values:\n");
    std::vector<std::pair<std::string, std::string>> options(options_.begin(), options_.end());
    std::sort(options.begin(), options.end());
    for (std::pair<std::string, std::string> const& option : options)
        fmt::print("{}={}", option.first, option.second);
}

}  // namespace config
