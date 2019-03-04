#include "curl_handle.h"
#include <fmt/format.h>
#include <stdexcept>

namespace leitse::details_curl_handle {

void init()
{
    if (CURLcode err = curl_global_init(CURL_GLOBAL_ALL); err != CURLE_OK)
        throw std::runtime_error{fmt::format("could not initialize curl: {}", curl_easy_strerror(err))};
}

}  // namespace leitse::details_curl_handle
