#include "SoundLib.h"
#include "Data.h"

// TODO argument types are not correct
void /* FUN_00455f0c */ getWaveParameters(unsigned int wave, int note, unsigned char* page, unsigned int* fine, int* semi, unsigned char* wsr) {
    if (wave < 0x4b) {
        if (note < 0) {
            note = 0;
        } else if (note > 0x7f) {
            note = 0x7f;
        }

        unsigned char index = docSampleZones[wave][note / 8];

        *page = docWaveParameters[index][0];
        *wsr = docWaveParameters[index][1];
        *semi = static_cast<signed char>(docWaveParameters[index][2]);
        *fine = docWaveParameters[index][3];
    } else {
        *page = 0;
        *wsr = 0;
        *semi = 0;
        *fine = 0;
    }
}

void /* FUN_00455f84 */ getWaveParameters(unsigned int wave, int note, unsigned char *page, unsigned char *wsr) {
    if (wave < 0x4b) {
        if (note < 0) {
            note = 0;
        } else if (note > 0x7f) {
            note = 0x7f;
        }

        int index = docSampleZones[wave][note / 8];

        *page = docWaveParameters[index][0];
        *wsr = docWaveParameters[index][1];
    } else {
        *page = 0;
        *wsr = 0;
    }
}
