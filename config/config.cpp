#include "config/config.h"

namespace config {

int Config::parse(int argc, char const* const* argv, bool allow_unkown)
{
    return 0;
}

void Config::parse_file(std::string_view const& path, bool allow_unkown)
{}

}  // namespace config
