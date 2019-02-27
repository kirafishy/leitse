#pragma once

#include <unordered_map>

namespace config {

struct Config {
private:
    std::unordered_map<std::string, std::string> storage_;
};

}  // namespace config
