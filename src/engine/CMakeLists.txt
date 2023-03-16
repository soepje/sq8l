set(HEADER_LIST Amp.h Data.h Doc.h FilterBase.h FilterSq.h MidiParser.h ModFoll.h SoundLib.h SqEnv.h SqLfo.h Synth.h)


add_library(engine Amp.cpp Data.cpp Doc.cpp FilterBase.cpp FilterSq.cpp MidiParser.cpp ModFoll.cpp SoundLib.cpp SqEnv.cpp SqLfo.cpp Synth.cpp ${HEADER_LIST})

set_target_properties(engine PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(engine PUBLIC .)

target_compile_features(engine PUBLIC cxx_std_11)
