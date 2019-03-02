#pragma once

#include <filesystem>

namespace leitse {

struct ItemSet {
    void write(std::filesystem::path const& dir) const;
};

}  // namespace leitse
