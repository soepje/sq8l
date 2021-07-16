#include <Synth.h>

Synth_settings loadDefaultProgram();

Synth_settings loadSysexProgram(std::vector<uint8_t> sysexData);

std::vector<Synth_settings> loadSysexProgramBank(std::vector<uint8_t> sysexData);
