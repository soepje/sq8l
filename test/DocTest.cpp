#define CATCH_CONFIG_MAIN

#include <iostream>
#include <PackDlt.h>
#include <Data.h>
#include "catch.h"
#include "Doc.h"

TEST_CASE("WSR adjust", "[doc]") {
    int semi = -24;
    unsigned char wsr = 9;

    fun_45c470(&semi, &wsr);

    REQUIRE(semi == 12);
    REQUIRE(wsr == 11);

    semi = -12;
    wsr = 25;

    fun_45c470(&semi, &wsr);

    REQUIRE(semi == 12);
    REQUIRE(wsr == 26);

    semi = -12;
    wsr = 9;

    fun_45c470(&semi, &wsr);

    REQUIRE(semi == 12);
    REQUIRE(wsr == 10);
}

TEST_CASE("load wave params", "[doc2]") {
    int a = 0;
    unsigned char wsr = 0;
    unsigned char page = 0;

    fun_45c49c(0x39, 0x39, &a, &wsr, &page, 3, 0, 0);

    REQUIRE(a == 17664);
    REQUIRE(wsr == 9);
    REQUIRE(page == 236);
}

TEST_CASE("load something", "[asdf]") {
    REQUIRE(pitchToFrequency(0x51f0) == 0xc69);
    REQUIRE(pitchToFrequency(0x45f0) == 0x635);
    REQUIRE(pitchToFrequency(0x4604) == 0x63a);
    REQUIRE(pitchToFrequency(0x64f2) == 0x2531);
}
