cmake_minimum_required(VERSION 3.17)

project(SQ8L)

include_directories("/usr/include/")
include_directories("/usr/include/portaudiocpp")

add_library(engine
        src/engine/Amp.cpp
        src/engine/Doc.cpp
        src/engine/FilterSq.cpp
        src/engine/ModFoll.cpp
        src/engine/SimPack.cpp
        src/engine/PackDlt.cpp
        src/engine/MidiParser.cpp
        src/engine/PackRle.cpp
        src/engine/SqEnv.cpp
        src/engine/SqLfo.cpp
        src/engine/Data.cpp
        src/engine/FilterBase.cpp
        src/engine/Synth.cpp
        src/engine/SoundLib.cpp)

add_library(catch
        lib/catch/catch.cpp)

# System tests
add_executable(tests
        test/system_tests.cpp)

target_include_directories(tests PUBLIC src/engine lib/catch)
target_link_libraries(tests PRIVATE engine catch)

# Functional tests
add_executable(functional_tests
        test/functional_tests.cpp
        test/test_utils.cpp)

target_include_directories(functional_tests PUBLIC src/engine lib/catch)
target_link_libraries(functional_tests PRIVATE m dl)
target_link_libraries(functional_tests PRIVATE catch engine portaudio)

# Plugin
add_library(plugin
        src/plugin/program.cpp)

target_include_directories(plugin PUBLIC src/engine)
target_link_libraries(plugin PRIVATE engine)

add_executable(program_test
        src/plugin/program_test.cpp)

target_include_directories(program_test PUBLIC src/engine lib/catch)
target_link_libraries(program_test PRIVATE catch engine plugin)
