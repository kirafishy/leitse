#pragma once

#include <cpr/response.h>
#include <string_view>

namespace leitse {

cpr::Response checked_download(std::string_view url);

};  // namespace leitse
