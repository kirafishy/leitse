#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace leitse {

struct ItemSet {
    enum class Map {
        any,
        summoners_rift,
        howling_abyss,
        twisted_treeline,
        crystal_scar
    };

    struct Item {
        int id;
        int count = 1;
    };

    struct Block {
        std::string name;
        std::vector<Item> items;
    };

    std::vector<Block> blocks;
    int priority;

    ItemSet() = default;
    ItemSet(std::string aggregator, Map map, std::string role);

    void write(std::filesystem::path const& dir) const;

private:
    std::string aggregator_;
    std::string role_;
    Map map_;

    std::string map_to_string() const;
};

}  // namespace leitse
