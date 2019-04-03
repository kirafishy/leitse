#include "curl_handle.h"
#include "ugg.h"
#include <config/config.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <utils/thread_pool.h>
#include <filesystem>
#include <thread>
#include <vector>

namespace leitse {
namespace {

void main_impl(int argc, char** argv)
{
    CurlHandle curl_handle;

    std::vector<std::unique_ptr<Aggregator>> aggregators;
    aggregators.push_back(std::make_unique<aggregators::Ugg>());

    config::Config conf;
    conf.set("league_dir", "C:/Riot Games/League of Legends");
    conf.set("threads", 2 * std::thread::hardware_concurrency());
    for (std::unique_ptr<Aggregator> const& aggregator : aggregators)
        for (std::pair<std::string, std::string> const& option : aggregator->options())
            conf.set(option.first, option.second);

    conf.parse_global_config("leitse");
    if (conf.parse_args(argc, argv)) {
        conf.show_help(argv[0]);
        return;
    }
    if (argc > 1)
        throw std::runtime_error{"too much arguments"};

    DataDragon data_dragon;
    data_dragon.populate();

    auto league_dir = conf.get<std::filesystem::path>("league_dir");
    if (!std::filesystem::exists(league_dir))
        throw std::runtime_error{fmt::format("league_dir '{}' does not exist", league_dir.generic_string())};

    std::filesystem::path champions_dir = league_dir / "Config" / "Champions";
    utils::ThreadPool thread_pool{conf.get<size_t>("threads")};
    for (Champion const& champion : data_dragon.champions()) {
        std::filesystem::path item_set_dir = champions_dir / champion.id / "Recommended";
        std::filesystem::create_directories(item_set_dir);
        for (std::unique_ptr<Aggregator> const& aggregator : aggregators)
            thread_pool.submit([&aggregator, &champion, item_set_dir] {
                try {
                    for (ItemSet const& item_set : aggregator->itemsets(champion))
                        item_set.write(item_set_dir);
                    spdlog::info("wrote item sets from '{}' for '{}'", aggregator->name(), champion.name);
                }
                catch (std::exception const& ex) {
                    spdlog::error("could not get item sets from '{}' for '{}': {}",
                                  aggregator->name(), champion.name, ex.what());
                }
            });
    }
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
        main_impl(argc, argv);
        spdlog::drop_all();
    }
    catch (std::exception const& ex) {
        spdlog::critical(ex.what());
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
