#include "Synth.h"

class ProgramLoader {
private:
    Synth_settings static loadSysexProgramData(std::vector<uint8_t> sysexData);

public:
    Synth_settings static loadDefaultProgram();
    Synth_settings static loadSysexProgram(std::vector<uint8_t> sysexData);
    std::vector<Synth_settings> static loadSysexProgramBank(std::vector<uint8_t> sysexData);
};
