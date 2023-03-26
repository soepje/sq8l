#pragma once

class SoundLib {
public:
    static void getWaveParameters(unsigned int wave, int note, unsigned char *page, unsigned char *wsr);
    static void getWaveParameters(unsigned int wave, int note, unsigned char* page, unsigned int* fine, int* semi, unsigned char* wsr);
};
