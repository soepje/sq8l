#pragma once

extern unsigned char /* 0x494318 */ docSampleZones[75][16];
extern unsigned char /* 0x4947c8 */ docWaveParameters[151][4];
extern unsigned int /* 0x4c0278 */ docFrequencyCurve[2050];
extern float /* 0x4c127c */ docVolumeCurve[256];
extern unsigned char /* 0x494e4c */ docPackedWaves[0x2b429];
extern char /* 0x4c1688 */ envShapingCurves[8][256];
extern unsigned char /* 0x494dcc */ envExponentialVelocityCurve[128];
extern unsigned char /* 0x494d4c */ envLinearVelocityCurve[128];
extern unsigned short /* 0x4c2090 */ envEnvelopeTimes[64];

// TODO figure out offsets in binary

// 0x494318 docSampleZones
// 0x4947c8 docWaveParameters
// 0x4c0278 docVolumeCurve2
// 0x4c127c docVolumeCurve
// 0x494e4c docPackedWaves
// 0x4c1688 envShapingCurves
// 0x494dcc envExponentialVelocityCurve
// 0x494d4c envLinearVelocityCurve
// 0x4c2090 envEnvelopeTimes