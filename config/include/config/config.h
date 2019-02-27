#pragma once

#include <unordered_map>

namespace config {

struct Config {
    std::string& operator[](std::string& const key);
    std::string& at(std::string& const key);
    std::string const& at(std::string& const key) const;
    bool exists(std::string& const key);
    void remove(std::string& const key);

    /// Parse command-line arguments and look for an config file.
    /// Return the number of parsed arguments, so you can handle positional arguments.
    int parse(int argc, char const* const* argv, bool allow_unkown = false);

    void parse_file(std::string_view const& path, bool allow_unkown = false);

    template <typename T>
    T get(std::string& const key) const;

private:
    std::unordered_map<std::string, std::string> map_;
};


template <typename T>
T Config::get(std::string& const key) const
{
    return T{key};
}

}  // namespace config
