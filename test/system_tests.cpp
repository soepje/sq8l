#define CATCH_CONFIG_MAIN

#include <iostream>
#include <PackDlt.h>
#include <Data.h>
#include "catch.h"
#include "Doc.h"

uint32_t adler32(unsigned char *data, size_t len) {
    uint32_t a = 1, b = 0;
    for (int index = 0; index < len; ++index) {
        a = (a + data[index]) % 65521;
        b = (b + a) % 65521;
    }
    return (b << 16) | a;
}

TEST_CASE( "Waveform unpacking is checked", "[waveform]" ) {
    unsigned char* waves = nullptr;
    int outputBufferLength = 0;

    PackDlt36 pack = PackDlt36();
    pack.unpack(docPackedWaves, 0x2b429, &outputBufferLength, &waves);

    REQUIRE(outputBufferLength == 0x40000);
    REQUIRE(adler32(waves, outputBufferLength) == 0xa36dd0b4);
}

TEST_CASE("WSR adjust", "[doc]") {
    int semi = -24;
    unsigned int wsr = 9;

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
    unsigned int wsr = 0;
    unsigned int page = 0;

    fun_45c49c(0x39, 0x39, &a, &wsr, &page, 3, 0, 0);

    REQUIRE(a == 17664);
    REQUIRE(wsr == 9);
    REQUIRE(page == 236);
}

TEST_CASE("load something", "[asdf]") {
    REQUIRE(getVolume2(0x51f0) == 0xc69);
    REQUIRE(getVolume2(0x45f0) == 0x635);
    REQUIRE(getVolume2(0x4604) == 0x63a);
    REQUIRE(getVolume2(0x64f2) == 0x2531);
}
