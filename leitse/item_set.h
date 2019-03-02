#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace leitse {

struct ItemSet {
    void write(std::filesystem::path const& dir) const;

private:
    enum class Map {
        any,
        summoners_rift,
        howling_abyss,
        twisted_treeline,
        crystal_scar
    };

    struct Item {
        std::string id;
        int count = 1;
    };

    struct Block {
        std::string name;
        std::vector<Item> items;
    };

    std::string name_;
    Map map_;
    std::vector<Block> blocks_;

    std::string map_to_string() const;
};

}  // namespace leitse
