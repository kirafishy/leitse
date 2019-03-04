#pragma once

#include "curl_handle.h"
#include <string>
#include <string_view>

namespace leitse {

struct SimpleCurl {
    SimpleCurl();

    std::string get(std::string_view url);

private:
    struct CurlDeleter {
        void operator()(CURL* ptr);
    };

    CurlHandle curl_handle_;
    std::unique_ptr<CURL, CurlDeleter> ptr_;

    void check_curl(CURLcode err);
    static size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata);
};

};  // namespace leitse
