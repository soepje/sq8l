set(HEADER_LIST Data.h)

add_library(data Data.cpp ${HEADER_LIST})

set_target_properties(data PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(data PUBLIC .)

target_compile_features(data PUBLIC cxx_std_11)
