#define CATCH_CONFIG_MAIN

#include <iostream>

#include <Synth.h>

#include "catch.h"
#include "test_utils.h"

TEST_CASE( "A sawtooth wave is played", "[sawtooth]" ) {
    float samples[44100 * 2] = { 0.0 };

    float amplitude = -1.0f;
    for (int i = 0; i < 44100 * 2; i++) {
        samples[i] = amplitude;
        amplitude += 0.01;
        if (amplitude >= 1.0) amplitude -= 2.0;
    }

    playSamples(samples, 44100 * 2, 1);
}

TEST_CASE( "A sawtooth wave is played 2", "[sawtooth2]" ) {

    Doc doc{16};
    doc.initVoices2();
    doc.setSampleRate(44100);

    VoiceSettings* voiceSettings = doc.getVoiceSettings(0);
    voiceSettings->oscSettings[0].wave = 0;
    voiceSettings->oscSettings[0].level = 40;
    voiceSettings->oscSettings[0].enabled = true;

    doc.triggerVoice(0, 49, -1, false, false);
    doc.updateVoiceSettings(0);

    float samples[44100 * 2] = { 0.0 };

    for (int i = 0; i < 44100 * 2; i++) {
        float sample = doc.getSample(0) * 10000;
        samples[i] = sample;
    }

    playSamples(samples, 44100 * 2, 1);

}

TEST_CASE( "The lick 1", "[lick1]" ) {

    Doc doc{16};
    doc.initVoices2();
    doc.setSampleRate(44100);

    VoiceSettings* voiceSettings = doc.getVoiceSettings(0);
    voiceSettings->oscSettings[0].wave = 15;
    voiceSettings->oscSettings[0].level = 40;
    voiceSettings->oscSettings[0].enabled = true;

    float samples[44100 * 4] = { 0.0 };
    int offset = 0;

    for (Message message : theLick) {
        for (int i = 0; i < message.delta; i++) {
            samples[offset + i] =  doc.getSample(0) * 10000;
        }
        offset += message.delta;

        if (message.status == NoteOn) {
            doc.triggerVoice(0, message.data1, -1, false, false);
            doc.updateVoiceSettings(0);
        } else if (message.status == NoteOff) {
            doc.stopVoice(0);
        }
    }

    playSamples(samples, 44100 * 4, 1);

}

TEST_CASE("Envelope saw", "[envsaw1]") {
    Synth synth{44100};
    Synth_settings settings = { 0 };

    settings.mono = 1;
    settings.voiceStealingMode = 0;

    settings.oscSettings[0].wave = 0;
    settings.oscSettings[0].enabled = 1;
    settings.oscSettings[0].level = 63;
    settings.oscSettings[0].octave = 0;

    settings.ampSettings.stereoPanning = 60;
    settings.ampSettings.saturation = 14;
    settings.ampSettings.envelopeAmplitudeModulationAmount = 50;

    settings.envSettings[3].levels[0] = 0;
    settings.envSettings[3].levels[1] = 60;
    settings.envSettings[3].levels[2] = 40;
    settings.envSettings[3].levels[3] = 40;
    settings.envSettings[3].levels[4] = 0;

    settings.envSettings[3].times[0] = 20;
    settings.envSettings[3].times[1] = 18;
    settings.envSettings[3].times[2] = 21;
    settings.envSettings[3].times[3] = 30;

    synth.mSettings = &settings;

    float samples[44100 * 4] = { 0.0 };

    synth.triggerNote(0, 40, 100);

    for (int i = 0; i < 44100 * 2; i++) {
        synth.getSamples(false, &samples[i*2], 1, &samples[i*2+1]);

        if (i == 20000) {
            synth.triggerNote(0, 40, 0);
        }
    }

    playSamples(samples, 44100 * 2, 2);
}

TEST_CASE("Synth bass lick", "[lick2]") {
    Synth synth{44100};
    Synth_settings settings = { 0 };

    settings.mono = 1;

    settings.ampSettings.amplitudeModulationAmount = 10;

    settings.oscSettings[0].wave = 0;
    settings.oscSettings[0].enabled = 1;
    settings.oscSettings[0].level = 63;
    settings.oscSettings[0].modulationAmount1 = 0; // 2
    settings.oscSettings[0].modulationSource1 = 0; // should be LFO
    settings.oscSettings[0].octave = -2;

    settings.filterSettings.cutoff = 0;
    settings.filterSettings.resonance = 31;
    settings.filterSettings.noteModulationAmount = 16; // called keyboard follow
    settings.filterSettings.modulationSource1 = 0; // env 3
    settings.filterSettings.modulationAmount1 = 50;
    settings.filterSettings.modulationSource1 = 0; // wheel
    settings.filterSettings.modulationAmount2 = 70;

    settings.envSettings[3].levels[0] = 10;
    settings.envSettings[3].levels[1] = 63;
    settings.envSettings[3].levels[2] = 63;
    settings.envSettings[3].levels[3] = 63;
    settings.envSettings[3].levels[4] = 0;

    settings.envSettings[3].times[0] = 10;
    settings.envSettings[3].times[1] = 18;
    settings.envSettings[3].times[2] = 21;
    settings.envSettings[3].times[3] = 0;

    synth.mSettings = &settings;

    float samples[44100 * 4] = { 0.0 };
    int offset = 0;

    for (Message message : theLick) {
        for (int i = 0; i < message.delta; i++) {
            float output[2] = { 0.0, 0.0 };

            synth.getSamples(false, &output[0], 1, &output[1]);
            samples[offset + i] = output[0];
        }
        offset += message.delta;

        if (message.status == NoteOn) {
            synth.triggerNote(0, message.data1, 100);
        } else if (message.status == NoteOff) {
            synth.triggerNote(0, message.data1, 0);
        }
    }

    playSamples(samples, 44100 * 4, 1);
}