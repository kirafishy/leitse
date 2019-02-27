#pragma once

#include <cstdlib>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>

namespace config {

struct Config {
    std::string& operator[](std::string const& key);
    std::string& at(std::string const& key);
    std::string const& at(std::string const& key) const;
    bool exists(std::string const& key);
    void remove(std::string const& key);
    void clear();

    /// Parse command-line arguments (and config file if explicitly given).
    /// Parsed arguments will be removed.
    /// Return true if application should show help (all arguments may not be parsed, but you can call again).
    bool parse_args(int& argc, char** argv, bool allow_unkown = false);

    void parse_global_config(std::string_view const& app_name);
    void parse_file(std::string_view const& path, bool allow_unkown = false);
    void show_help();

    template <typename T>
    T get(std::string const& key) const;

private:
    std::unordered_map<std::string, std::string> map_;
};


template <typename T>
T Config::get(std::string const& key) const
{
    std::string const& value = at(key);
    if constexpr (std::is_arithmetic_v<T>) {
        if (value.length() == 0)
            throw std::runtime_error{"numeric value is empty string"};

        char* end;
        T result;
        if constexpr (std::is_integral_v<T>) {
            if constexpr (std::is_unsigned_v<T>)
                result = static_cast<T>(std::strtoull(value.c_str(), &end, 0));
            else
                result = static_cast<T>(std::strtoll(value.c_str(), &end, 0));
        }
        else
            result = static_cast<T>(std::strtold(value.c_str(), &end));

        if (end != value.c_str() + value.length())
            throw std::runtime_error{"numeric value is malformed"};
        return result;
    }
    else
        return T{value};
}

}  // namespace config
