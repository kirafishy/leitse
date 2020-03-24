#include "simple_curl.h"
#include <fmt/format.h>
#include <stdexcept>

namespace leitse {

SimpleCurl::SimpleCurl()
{
    ptr_.reset(curl_easy_init());
    if (!ptr_)
        throw std::runtime_error{"could not initialize curl_easy"};

    check_curl(curl_easy_setopt(ptr_.get(), CURLOPT_ACCEPT_ENCODING, ""));
    check_curl(curl_easy_setopt(ptr_.get(), CURLOPT_FAILONERROR, 1L));
    check_curl(curl_easy_setopt(ptr_.get(), CURLOPT_WRITEFUNCTION, &write_callback));
}

std::string SimpleCurl::get(std::string_view url)
{
    std::string buffer;
    check_curl(curl_easy_setopt(ptr_.get(), CURLOPT_WRITEDATA, &buffer));
    check_curl(curl_easy_setopt(ptr_.get(), CURLOPT_URL, url.data()));
    check_curl(curl_easy_perform(ptr_.get()));
    return buffer;
}

void SimpleCurl::check_curl(CURLcode err)
{
    if (err != CURLE_OK)
        throw std::runtime_error{fmt::format("curl error: {}", curl_easy_strerror(err))};
}

size_t SimpleCurl::write_callback(char* ptr, size_t /*size*/, size_t nmemb, void* userdata)
{
    std::string& buffer = *reinterpret_cast<std::string*>(userdata);
    buffer.append(ptr, nmemb);
    return nmemb;
}


void SimpleCurl::CurlDeleter::operator()(CURL* ptr)
{
    curl_easy_cleanup(ptr);
}

}  // namespace leitse
