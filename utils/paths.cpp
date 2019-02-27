#include "utils/paths.h"
#include <utils/scope_exit.h>
#include <cstdlib>

#ifdef _WIN32
#    include <shlobj.h>
#endif

namespace utils {

std::filesystem::path get_kiwixz_home(std::string_view const& app_name)
{
#ifdef _WIN32
    wchar_t* base;
    if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, nullptr, &base) != S_OK)
        throw std::runtime_error{"could not get appdata"};
    ScopeExit free_base{[&] {
        CoTaskMemFree(base);
    }};
    std::filesystem::path path = std::filesystem::path{base} / "kiwixz" / app_name;
    std::filesystem::create_directories(path);
    return path;
#else
    return {};
#endif
}

}  // namespace utils
