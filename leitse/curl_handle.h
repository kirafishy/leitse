#pragma once

#include <curl/curl.h>
#include <utils/shared_handle.h>

namespace leitse {
namespace details_curl_handle {

void init();

}


using CurlHandle = utils::SharedHandle<&details_curl_handle::init, &curl_global_cleanup>;

}  // namespace leitse
