#include "download.h"
#include "simple_curl.h"

namespace leitse {

std::string simple_download(std::string_view url)
{
    thread_local SimpleCurl curl;  // thread_local for persistent connections
    return curl.get(url);
}

};  // namespace leitse
