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
