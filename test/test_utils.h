#include "portaudio.h"

enum Status {
    NoteOn,
    NoteOff
};

struct Message {
    unsigned int delta;
    Status status;
    unsigned char data1;
    unsigned char data2;
};

extern Message theLick[14];

PaError playSamples(float* samples, int numSamples, int numChannels);
