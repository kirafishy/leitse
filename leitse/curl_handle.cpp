#include "curl_handle.h"
#include <fmt/format.h>
#include <stdexcept>

namespace leitse::details_curl_handle {

void init()
{
    if (int err = curl_global_init(CURL_GLOBAL_ALL); err != CURLE_OK)
        throw std::runtime_error{fmt::format("could not initialize curl: error {}", err)};
}

}  // namespace leitse::details_curl_handle
