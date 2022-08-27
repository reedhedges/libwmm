cmake_minimum_required(VERSION 3.16)
project(libwmm VERSION 0.1 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(wmm STATIC 
  src/EGM9615.c
  src/EGM9615.h
  src/GeomagnetismHeader.h
  src/GeomagnetismLibrary.c
  src/GeomagnetismLibrary.o
  src/wmm.cc
)

set_target_properties(wmm PROPERTIES CXX_STANDARD 20)
target_include_directories(wmm PUBLIC include PRIVATE src)
target_compile_options(wmm PRIVATE -g -O3 -Wall -Wextra)
install(TARGETS wmm DESTINATION lib)
install(FILES include/libwmm/wmm.hh DESTINATION include/libwmm)


message(STATUS "libwmm: CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
message(STATUS "libwmm: CMAKE_C_COMPILER=${CMAKE_C_COMPILER}")
message(STATUS "libwmm: CMAKE_CXX_COMPILER_TARGET=${CMAKE_CXX_COMPILER_TARGET}")
message(STATUS "libwmm: CMAKE_C_COMPILER_TARGET=${CMAKE_C_COMPILER_TARGET}")
message(STATUS "libwmm: CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
