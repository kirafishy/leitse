#include "config/config.h"
#include <doctest/doctest.h>
#include <array>
#include <forward_list>
#include <iostream>

namespace config::test {

TEST_SUITE("config")
{
    TEST_CASE("conversions")
    {
        Config conf;

        conf["char"] = "-42";
        CHECK(conf.get<char>("char") == -42);
        conf["short"] = "-42";
        CHECK(conf.get<short>("short") == -42);
        conf["int"] = "-42";
        CHECK(conf.get<int>("int") == -42);
        conf["long long"] = "-42";
        CHECK(conf.get<long long>("long long") == -42);
        conf["unsigned char"] = "42";
        CHECK(conf.get<unsigned char>("unsigned char") == 42);
        conf["unsigned short"] = "42";
        CHECK(conf.get<unsigned short>("unsigned short") == 42);
        conf["unsigned int"] = "42";
        CHECK(conf.get<unsigned int>("unsigned int") == 42u);
        conf["unsigned long long"] = "42";
        CHECK(conf.get<unsigned long long>("unsigned long long") == 42ull);
        conf["float"] = "-.42";
        CHECK(conf.get<float>("float") == -.42f);
        conf["double"] = "42.";
        CHECK(conf.get<double>("double") == 42.);

        conf["bad"] = "42a";
        CHECK_THROWS(conf.get<int>("bad"));
        CHECK_THROWS(conf.get<float>("bad"));
        conf["bad"] = "a42";
        CHECK_THROWS(conf.get<int>("bad"));
        CHECK_THROWS(conf.get<float>("bad"));
        conf["bad"] = "42-";
        CHECK_THROWS(conf.get<int>("bad"));
        CHECK_THROWS(conf.get<float>("bad"));
        conf["bad"] = "42+";
        CHECK_THROWS(conf.get<int>("bad"));
        CHECK_THROWS(conf.get<float>("bad"));
        conf["bad"] = "abc";
        CHECK_THROWS(conf.get<int>("bad"));
        CHECK_THROWS(conf.get<float>("bad"));
        conf["bad"] = "";
        CHECK_THROWS(conf.get<int>("bad"));
        CHECK_THROWS(conf.get<float>("bad"));

        struct S {
            std::string s;
            explicit S(std::string ss) :
                s{std::move(ss)}
            {}
        };
        conf["str"] = "hello";
        CHECK(conf.get<S>("str").s == conf.at("str"));
    }

    TEST_CASE("parse_args")
    {
        Config conf;

        auto parse = [&](auto&&... args) {
            std::array<std::string, sizeof...(args) + 1> args_str{"self", std::forward<decltype(args)>(args)...};
            std::array<char*, sizeof...(args) + 2> argv;
            for (unsigned i = 0; i < args_str.size(); ++i)
                argv[i] = args_str[i].data();
            argv[argv.size() - 1] = nullptr;
            int argc = static_cast<int>(argv.size() - 1);
            if (conf.parse_args(argc, argv.data()))
                return -1;
            return argc;
        };

        conf["a"] = "a";
        conf["b"] = "b";
        conf["c"] = "c";
        CHECK(parse("-a=1", "-b=2", "-c=3") == 1);
    }

    TEST_CASE("parse_args clean argv")
    {
        std::forward_list<std::string> str_registry;  // keep the strings outside of lambda, so argv pointers stay valid
        Config conf;

        auto parse = [&](auto&&... args) {
            std::array<std::string, sizeof...(args) + 1> args_str = {"self", std::forward<decltype(args)>(args)...};
            std::array<char*, sizeof...(args) + 2> argv;
            for (unsigned i = 0; i < args_str.size(); ++i)
                argv[i] = str_registry.emplace_front(std::move(args_str[i])).data();
            argv[argv.size() - 1] = nullptr;
            int argc = static_cast<int>(argv.size() - 1);
            conf.parse_args(argc, argv.data());
            return argv;
        };

        conf["a"] = "a";
        conf["b"] = "b";
        auto argv = parse("-a=1", "first", "-b=2", "second", "--", "-third");
        CHECK(argv[0] == std::string{"self"});
        CHECK(argv[1] == std::string{"first"});
        CHECK(argv[2] == std::string{"second"});
        CHECK(argv[3] == std::string{"-third"});
        CHECK(argv[4] == nullptr);
    }
}

}  // namespace config::test
