#include <iostream>
#include <fstream>

#include "catch.h"
#include "program.h"

std::vector<uint8_t> readSysexFile(std::string filePath) {
    std::ifstream instream(filePath, std::ios::in | std::ios::binary);
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(instream)), std::istreambuf_iterator<char>());
    return std::vector<uint8_t>(data.begin() + 1, data.end() - 1);
}

TEST_CASE( "Preset loading", "[preset_anabas]" ) {
    Synth_settings settings = loadSysexProgram(readSysexFile("../testdata/ANABAS.syx"));

    REQUIRE(settings.presetNameLength == 6);
    REQUIRE(settings.presetName[0] == 65);
    REQUIRE(settings.presetName[1] == 78);
    REQUIRE(settings.presetName[2] == 65);
    REQUIRE(settings.presetName[3] == 66);
    REQUIRE(settings.presetName[4] == 65);
    REQUIRE(settings.presetName[5] == 83);
    REQUIRE(settings.presetName[6] == 0);
    REQUIRE(settings.presetName[7] == 0);
    REQUIRE(settings.presetName[8] == 0);
    REQUIRE(settings.presetName[9] == 0);
    REQUIRE(settings.presetName[10] == 0);
    REQUIRE(settings.presetName[11] == 0);
    REQUIRE(settings.presetName[12] == 0);
    REQUIRE(settings.presetName[13] == 0);
    REQUIRE(settings.presetName[14] == 0);
    REQUIRE(settings.presetName[15] == 0);
    REQUIRE(settings.presetName[16] == 0);
    REQUIRE(settings.presetName[17] == 0);
    REQUIRE(settings.presetName[18] == 0);
    REQUIRE(settings.presetName[19] == 0);
    REQUIRE(settings.presetName[20] == 0);
    REQUIRE(settings.presetName[21] == 0);
    REQUIRE(settings.presetName[22] == 0);
    REQUIRE(settings.presetName[23] == 0);
    REQUIRE(settings.presetName[24] == 0);
    REQUIRE(settings.presetName[25] == 0);
    REQUIRE(settings.presetName[26] == 0);
    REQUIRE(settings.presetName[27] == 0);
    REQUIRE(settings.presetName[28] == 0);
    REQUIRE(settings.presetName[29] == 0);
    REQUIRE(settings.presetName[30] == 0);

    REQUIRE(settings.envSettings[0].levels[0] == 0);
    REQUIRE(settings.envSettings[0].levels[1] == 25);
    REQUIRE(settings.envSettings[0].levels[2] == 8);
    REQUIRE(settings.envSettings[0].levels[3] == 20);
    REQUIRE(settings.envSettings[0].levels[4] == 0);
    REQUIRE(settings.envSettings[0].levelModulation == 0);
    REQUIRE(settings.envSettings[0].attackTimeModulation == 0);
    REQUIRE(settings.envSettings[0].times[0] == 0);
    REQUIRE(settings.envSettings[0].times[1] == 22);
    REQUIRE(settings.envSettings[0].times[2] == 63);
    REQUIRE(settings.envSettings[0].times[3] == 22);
    REQUIRE(settings.envSettings[0].timeScaling == 9);
    REQUIRE(settings.envSettings[0].options == 0);
    REQUIRE(settings.envSettings[0].outputSmoothness == 0);

    REQUIRE(settings.envSettings[1].levels[0] == 0);
    REQUIRE(settings.envSettings[1].levels[1] == 63);
    REQUIRE(settings.envSettings[1].levels[2] == 33);
    REQUIRE(settings.envSettings[1].levels[3] == 28);
    REQUIRE(settings.envSettings[1].levels[4] == 0);
    REQUIRE(settings.envSettings[1].levelModulation == 38);
    REQUIRE(settings.envSettings[1].attackTimeModulation == 4);
    REQUIRE(settings.envSettings[1].times[0] == 2);
    REQUIRE(settings.envSettings[1].times[1] == 18);
    REQUIRE(settings.envSettings[1].times[2] == 22);
    REQUIRE(settings.envSettings[1].times[3] == 12);
    REQUIRE(settings.envSettings[1].timeScaling == 11);
    REQUIRE(settings.envSettings[1].options == 0);
    REQUIRE(settings.envSettings[1].outputSmoothness == 0);

    REQUIRE(settings.envSettings[2].levels[0] == 0);
    REQUIRE(settings.envSettings[2].levels[1] == 0);
    REQUIRE(settings.envSettings[2].levels[2] == 3);
    REQUIRE(settings.envSettings[2].levels[3] == 0);
    REQUIRE(settings.envSettings[2].levels[4] == 0);
    REQUIRE(settings.envSettings[2].levelModulation == 0);
    REQUIRE(settings.envSettings[2].attackTimeModulation == 1);
    REQUIRE(settings.envSettings[2].times[0] == 0);
    REQUIRE(settings.envSettings[2].times[1] == 14);
    REQUIRE(settings.envSettings[2].times[2] == 0);
    REQUIRE(settings.envSettings[2].times[3] == 25);
    REQUIRE(settings.envSettings[2].timeScaling == 0);
    REQUIRE(settings.envSettings[2].options == 0);
    REQUIRE(settings.envSettings[2].outputSmoothness == 0);

    REQUIRE(settings.envSettings[3].levels[0] == 0);
    REQUIRE(settings.envSettings[3].levels[1] == 63);
    REQUIRE(settings.envSettings[3].levels[2] == 58);
    REQUIRE(settings.envSettings[3].levels[3] == 58);
    REQUIRE(settings.envSettings[3].levels[4] == 0);
    REQUIRE(settings.envSettings[3].levelModulation == 0);
    REQUIRE(settings.envSettings[3].attackTimeModulation == 63);
    REQUIRE(settings.envSettings[3].times[0] == 0);
    REQUIRE(settings.envSettings[3].times[1] == 33);
    REQUIRE(settings.envSettings[3].times[2] == 33);
    REQUIRE(settings.envSettings[3].times[3] == 4);
    REQUIRE(settings.envSettings[3].timeScaling == 0);
    REQUIRE(settings.envSettings[3].options == 0);
    REQUIRE(settings.envSettings[3].outputSmoothness == 0);

    REQUIRE(settings.oscSettings[0].octave == -1);
    REQUIRE(settings.oscSettings[0].semi == 0);
    REQUIRE(settings.oscSettings[0].fine == 4);
    REQUIRE(settings.oscSettings[0].wave == 0);
    REQUIRE(settings.oscSettings[0].modulationSource1 == 0);
    REQUIRE(settings.oscSettings[0].modulationAmount1 == 5);
    REQUIRE(settings.oscSettings[0].modulationSource2 == 6);
    REQUIRE(settings.oscSettings[0].modulationAmount2 == 18);
    REQUIRE(settings.oscSettings[0].level == 63);
    REQUIRE(settings.oscSettings[0].enabled == 1);
    REQUIRE(settings.oscSettings[0].modulationSource3 == -1);
    REQUIRE(settings.oscSettings[0].modulationAmount3 == 63);
    REQUIRE(settings.oscSettings[0].modulationSource4 == 2);
    REQUIRE(settings.oscSettings[0].modulationAmount4 == 0);

    REQUIRE(settings.oscSettings[1].octave == -1);
    REQUIRE(settings.oscSettings[1].semi == 0);
    REQUIRE(settings.oscSettings[1].fine == 5);
    REQUIRE(settings.oscSettings[1].wave == 0);
    REQUIRE(settings.oscSettings[1].modulationSource1 == 0);
    REQUIRE(settings.oscSettings[1].modulationAmount1 == 5);
    REQUIRE(settings.oscSettings[1].modulationSource2 == 6);
    REQUIRE(settings.oscSettings[1].modulationAmount2 == 0);
    REQUIRE(settings.oscSettings[1].level == 63);
    REQUIRE(settings.oscSettings[1].enabled == 1);
    REQUIRE(settings.oscSettings[1].modulationSource3 == -1);
    REQUIRE(settings.oscSettings[1].modulationAmount3 == 63);
    REQUIRE(settings.oscSettings[1].modulationSource4 == 0);
    REQUIRE(settings.oscSettings[1].modulationAmount4 == 0);

    REQUIRE(settings.oscSettings[2].octave == -1);
    REQUIRE(settings.oscSettings[2].semi == 0);
    REQUIRE(settings.oscSettings[2].fine == 3);
    REQUIRE(settings.oscSettings[2].wave == 0);
    REQUIRE(settings.oscSettings[2].modulationSource1 == 0);
    REQUIRE(settings.oscSettings[2].modulationAmount1 == 5);
    REQUIRE(settings.oscSettings[2].modulationSource2 == 6);
    REQUIRE(settings.oscSettings[2].modulationAmount2 == 0);
    REQUIRE(settings.oscSettings[2].level == 63);
    REQUIRE(settings.oscSettings[2].enabled == 1);
    REQUIRE(settings.oscSettings[2].modulationSource3 == -1);
    REQUIRE(settings.oscSettings[2].modulationAmount3 == 63);
    REQUIRE(settings.oscSettings[2].modulationSource4 == 0);
    REQUIRE(settings.oscSettings[2].modulationAmount4 == 0);

    REQUIRE(settings.modulationMatrixSettings[0].modulationSource1 == -1);
    REQUIRE(settings.modulationMatrixSettings[0].modulationAmount1 == 0);
    REQUIRE(settings.modulationMatrixSettings[0].modulationSource2 == -1);
    REQUIRE(settings.modulationMatrixSettings[0].modulationAmount2 == 0);
    REQUIRE(settings.modulationMatrixSettings[0].modulationSource3 == -1);
    REQUIRE(settings.modulationMatrixSettings[0].modulationAmount3 == 0);
    REQUIRE(settings.modulationMatrixSettings[0].amplitudeModulationSource == -1);
    REQUIRE(settings.modulationMatrixSettings[0].amplitudeModulationAmount == 0);

    REQUIRE(settings.modulationMatrixSettings[1].modulationSource1 == -1);
    REQUIRE(settings.modulationMatrixSettings[1].modulationAmount1 == 0);
    REQUIRE(settings.modulationMatrixSettings[1].modulationSource2 == -1);
    REQUIRE(settings.modulationMatrixSettings[1].modulationAmount2 == 0);
    REQUIRE(settings.modulationMatrixSettings[1].modulationSource3 == -1);
    REQUIRE(settings.modulationMatrixSettings[1].modulationAmount3 == 0);
    REQUIRE(settings.modulationMatrixSettings[1].amplitudeModulationSource == -1);
    REQUIRE(settings.modulationMatrixSettings[1].amplitudeModulationAmount == 0);

    REQUIRE(settings.modulationMatrixSettings[2].modulationSource1 == -1);
    REQUIRE(settings.modulationMatrixSettings[2].modulationAmount1 == 0);
    REQUIRE(settings.modulationMatrixSettings[2].modulationSource2 == -1);
    REQUIRE(settings.modulationMatrixSettings[2].modulationAmount2 == 0);
    REQUIRE(settings.modulationMatrixSettings[2].modulationSource3 == -1);
    REQUIRE(settings.modulationMatrixSettings[2].modulationAmount3 == 0);
    REQUIRE(settings.modulationMatrixSettings[2].amplitudeModulationSource == -1);
    REQUIRE(settings.modulationMatrixSettings[2].amplitudeModulationAmount == 0);

    REQUIRE(settings.filterSettings.cutoff == 2);
    REQUIRE(settings.filterSettings.resonance == 29);
    REQUIRE(settings.filterSettings.noteModulationAmount == 17);
    REQUIRE(settings.filterSettings.modulationSource1 == 5);
    REQUIRE(settings.filterSettings.modulationAmount1 == 56);
    REQUIRE(settings.filterSettings.modulationSource2 == 17);
    REQUIRE(settings.filterSettings.modulationAmount2 == 24);

    REQUIRE(settings.ampSettings.envelopeAmplitudeModulationAmount == 47);
    REQUIRE(settings.ampSettings.stereoPanning == 0);
    REQUIRE(settings.ampSettings.amplitudeModulationSource == -1);
    REQUIRE(settings.ampSettings.amplitudeModulationAmount == 0);
    REQUIRE(settings.ampSettings.panningModulationSource == 0);
    REQUIRE(settings.ampSettings.panningModulationAmount == 0);
    REQUIRE(settings.ampSettings.saturation == 0);

    REQUIRE(settings.muffle == 0);
    REQUIRE(settings.field_13c == 0);
    REQUIRE(settings.sync == 0);
    REQUIRE(settings.am == 0);
    REQUIRE(settings.mono == 1);
    REQUIRE(settings.glide == 12);
    REQUIRE(settings.restartVoice == 0);
    REQUIRE(settings.restartEnvelopes == 1);
    REQUIRE(settings.restartOscillators == 0);
    REQUIRE(settings.cycle == 0);
    REQUIRE(settings.pitchBendRange == 2);
    REQUIRE(settings.pitchBendMode == 0);
    REQUIRE(settings.emuOptions == 1);
    REQUIRE(settings.voiceStealingMode == 0);
    REQUIRE(settings.field_196 == 0);
}
