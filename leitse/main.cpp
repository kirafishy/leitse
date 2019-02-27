#include <config/config.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <filesystem>

namespace leitse {
namespace {

void main_impl(int argc, char** argv)
{
    config::Config cfg;
    cfg.parse(argc, argv);
}

int main(int argc, char** argv)
{
    try {
        std::string log_file_path = (std::filesystem::temp_directory_path() / "leitse.log").string();
        auto console_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file_path, true);
        auto logger = std::make_shared<spdlog::logger>("", spdlog::sinks_init_list{console_sink, file_sink});
        spdlog::set_default_logger(logger);
#ifdef DEBUG
        spdlog::set_level(spdlog::level::debug);
#endif
        spdlog::set_pattern("%^[%H:%M:%S.%f][%t][%l]%$ %v");
        spdlog::debug("started");
        main_impl(argc, argv);
        spdlog::debug("terminated");
        spdlog::drop_all();
    }
    catch (std::exception const& ex) {
        spdlog::critical("critical exception: {}", ex.what());
        return 1;
    }
    return 0;
}

}  // namespace
}  // namespace leitse


int main(int argc, char** argv)
{
    leitse::main(argc, argv);
}
