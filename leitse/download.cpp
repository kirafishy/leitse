#include "download.h"
#include <cpr/api.h>
#include <fmt/format.h>

namespace leitse {

cpr::Response checked_download(std::string_view url)
{
    cpr::Response response = cpr::Get(cpr::Url{url});
    if (response.error)
        throw std::runtime_error{fmt::format("could not download '{}': {}", url, response.error.message)};
    if (response.status_code >= 400)
        throw std::runtime_error{fmt::format("could not download '{}': status code {}", url, response.status_code)};
    return response;
}

};  // namespace leitse
