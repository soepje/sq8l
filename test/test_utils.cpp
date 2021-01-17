#include <cstdio>
#include <fstream>

#include "portaudio.h"
#include "test_utils.h"

#define SAMPLE_RATE (44100)

Message theLick[] = {
    Message { 0, NoteOn, 52, 0 },
    Message { 13000, NoteOff, 52, 0 },
    Message { 0, NoteOn, 54, 0 },
    Message { 7000, NoteOff, 54, 0 },
    Message { 0, NoteOn, 55, 0 },
    Message { 13000, NoteOff, 55, 0 },
    Message { 0, NoteOn, 57, 0 },
    Message { 7000, NoteOff, 57, 0 },
    Message { 0, NoteOn, 54, 0 },
    Message { 20000, NoteOff, 54, 0 },
    Message { 0, NoteOn, 50, 0 },
    Message { 13000, NoteOff, 50, 0 },
    Message { 0, NoteOn, 52, 0 },
    Message { 27000, NoteOff, 52, 0 },
};


inline void DEBUG_PLOT_CLEAR() {
    std::ofstream ofs;
    ofs.open("plot.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

inline void DEBUG_PLOT(int x) {
    std::ofstream outfile;
    outfile.open("plot.txt", std::ios_base::app); // append instead of overwrite
    outfile << x << "\n";
}

inline void DEBUG_PLOT(float x) {
    std::ofstream outfile;
    outfile.open("plot.txt", std::ios_base::app); // append instead of overwrite
    outfile << x << "\n";
}



struct paData {
    float *buf;
    int numSamples;
    int numChannels;
    int offset;

    float phase;
};

static int playSamplesCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
    paData *data = (paData*)userData;
    float *out = (float*)outputBuffer;

    for (int i = 0; i < framesPerBuffer; i++) {
        if (data->offset + i < data->numSamples) {
            if (data->numChannels == 1) {
                out[i*2] = data->buf[data->offset + i];
                out[i*2+1] = data->buf[data->offset + i];
            } else if (data->numChannels == 2) {
                out[i*2] = data->buf[(data->offset + i)*2];
                out[i*2+1] = data->buf[(data->offset + i)*2 + 1];
            }
        } else {
            out[i*2] = 0.0;
            out[i*2+1] = 0.0;
        }
    }

    data->offset += framesPerBuffer;

    return 0;
}

PaError playSamples(float* samples, int numSamples, int numChannels) {
    paData data = {
            samples,
            numSamples,
            numChannels,
            0,
            0.0
    };

    PaStream *stream;
    PaError err;
//    freopen("/dev/null", "w", stderr);
    err = Pa_Initialize();
//    freopen("/dev/stderr", "w", stderr);

    if( err != paNoError ) goto error;


    for (int i = 0, end = Pa_GetDeviceCount(); i != end; ++i) {
        PaDeviceInfo const* info = Pa_GetDeviceInfo(i);
        if (!info) continue;
        printf("%d: %s\n", i, info->name);
    }

    err = Pa_OpenDefaultStream( &stream,0,2, paFloat32, SAMPLE_RATE, 256, playSamplesCallback, &data);

    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

    Pa_Sleep((numSamples / SAMPLE_RATE) * 1000);

    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    return paNoError;

error:
    return err;
}
