#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <tinyfiledialogs/tinyfiledialogs.h>
#include <filesystem>

namespace leitse {
namespace {

void main_impl(int argc, char** argv)
{}

int main(int argc, char** argv)
{
    try {
        std::string log_file_path = (std::filesystem::temp_directory_path() / "fractal.log").string();
        auto console_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file_path, true);
        auto logger = std::make_shared<spdlog::logger>("", spdlog::sinks_init_list{console_sink, file_sink});
        spdlog::set_default_logger(logger);
#ifdef DEBUG
        spdlog::set_level(spdlog::level::debug);
#endif
        spdlog::set_pattern("%^[%H:%M:%S.%f][%t][%l]%$ %v");
        spdlog::debug("starting...");
        main_impl(argc, argv);
        spdlog::debug("terminating...");
        spdlog::drop_all();
        return 0;
    }
    catch (std::exception const& ex) {
        spdlog::critical("critical exception: {}", ex.what());
        return 1;
    }
}

}  // namespace
}  // namespace fractal


int main(int argc, char** argv)
{
    leitse::main(argc, argv);
}
