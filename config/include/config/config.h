#pragma once

#include <unordered_map>

namespace config {

struct Config {
    /// Parse command-line arguments and look for an config file.
    /// Return the number of parsed arguments, so you can handle positional arguments.
    int parse(int argc, char const* const* argv, bool allow_unkown = false);

    void parse_file(std::string_view const& path, bool allow_unkown = false);

private:
    std::unordered_map<std::string, std::string> map_;
};

}  // namespace config
