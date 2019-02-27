#include "config/config.h"
#include <doctest/doctest.h>
#include <iostream>

namespace config::test {

TEST_SUITE("config")
{
    TEST_CASE("conversions")
    {
        Config cfg;

        cfg["char"] = "-42";
        CHECK(cfg.get<char>("char") == -42);
        cfg["short"] = "-42";
        CHECK(cfg.get<short>("short") == -42);
        cfg["int"] = "-42";
        CHECK(cfg.get<int>("int") == -42);
        cfg["long long"] = "-42";
        CHECK(cfg.get<long long>("long long") == -42);
        cfg["unsigned char"] = "42";
        CHECK(cfg.get<unsigned char>("unsigned char") == 42);
        cfg["unsigned short"] = "42";
        CHECK(cfg.get<unsigned short>("unsigned short") == 42);
        cfg["unsigned int"] = "42";
        CHECK(cfg.get<unsigned int>("unsigned int") == 42u);
        cfg["unsigned long long"] = "42";
        CHECK(cfg.get<unsigned long long>("unsigned long long") == 42ull);
        cfg["float"] = "-.42";
        CHECK(cfg.get<float>("float") == -.42f);
        cfg["double"] = "42.";
        CHECK(cfg.get<double>("double") == 42.);

        cfg["bad"] = "42a";
        CHECK_THROWS(cfg.get<int>("bad"));
        CHECK_THROWS(cfg.get<float>("bad"));
        cfg["bad"] = "a42";
        CHECK_THROWS(cfg.get<int>("bad"));
        CHECK_THROWS(cfg.get<float>("bad"));
        cfg["bad"] = "42-";
        CHECK_THROWS(cfg.get<int>("bad"));
        CHECK_THROWS(cfg.get<float>("bad"));
        cfg["bad"] = "42+";
        CHECK_THROWS(cfg.get<int>("bad"));
        CHECK_THROWS(cfg.get<float>("bad"));
        cfg["bad"] = "abc";
        CHECK_THROWS(cfg.get<int>("bad"));
        CHECK_THROWS(cfg.get<float>("bad"));
        cfg["bad"] = "";
        CHECK_THROWS(cfg.get<int>("bad"));
        CHECK_THROWS(cfg.get<float>("bad"));

        struct S {
            std::string s;
            S(std::string ss) :
                s{ss}
            {}
        };
        cfg["str"] = "hello";
        CHECK(cfg.get<S>("str").s == cfg.at("str"));
    }
}

}  // namespace config::test
