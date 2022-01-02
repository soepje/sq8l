set(HEADER_LIST Amp.h Data.h Doc.h FilterBase.h FilterSq.h MidiParser.h ModFoll.h PackDlt.h PackRle.h SimPack.h SoundLib.h SqEnv.h SqLfo.h Synth.h)


add_library(engine Amp.cpp Data.cpp Doc.cpp FilterBase.cpp FilterSq.cpp MidiParser.cpp ModFoll.cpp PackDlt.cpp PackRle.cpp SimPack.cpp SoundLib.cpp SqEnv.cpp SqLfo.cpp Synth.cpp ${HEADER_LIST})

target_include_directories(engine PUBLIC .)

target_compile_features(engine PUBLIC cxx_std_11)
