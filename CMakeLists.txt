cmake_minimum_required(VERSION 3.16)
project(libwmm VERSION 0.1 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(LIBWMM_ENABLE_IPO "Enable IPO/LTO if build type is Release, RelWithDebug, or otherwise matches Rel*")

set(parallel $ENV{CMAKE_BUILD_PARALLEL_LEVEL})
if(parallel)
  message(STATUS "libwmm: CMAKE_BUILD_PARALLEL_LEVEL environment variable is set to ${parallel}")
else()
  message(WARNING "libwmm: CMAKE_BUILD_PARALLEL_LEVEL environment variable is not set")
endif()

add_library(wmm STATIC 
  src/EGM9615.c
  src/EGM9615.h
  src/GeomagnetismHeader.h
  src/GeomagnetismLibrary.c
  src/wmm.cc
)

set_target_properties(wmm PROPERTIES CXX_STANDARD 20)

if(LIBWMM_ENABLE_IPO AND CMAKE_BUILD_TYPE MATCHES "Rel.*")
  # TODO also check if IPO is available?
  message(STATUS "IPO Enabled via LIBWMM_ENABLE_IPO and build type is Rel*, enabling IPO for libwmm build.")
  set_property(TARGET wmm PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  set_property(TARGET wmm PROPERTY INTERPROCEDURAL_OPTIMIZATION FALSE)
endif()

target_include_directories(wmm PUBLIC include PRIVATE src)
target_compile_options(wmm PRIVATE -g -O3 -Wall -Wextra)
install(TARGETS wmm DESTINATION lib)
install(FILES include/libwmm/wmm.hh DESTINATION include/libwmm)


message(STATUS "libwmm: CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
message(STATUS "libwmm: CMAKE_C_COMPILER=${CMAKE_C_COMPILER}")
message(STATUS "libwmm: CMAKE_CXX_COMPILER_TARGET=${CMAKE_CXX_COMPILER_TARGET}")
message(STATUS "libwmm: CMAKE_C_COMPILER_TARGET=${CMAKE_C_COMPILER_TARGET}")
message(STATUS "libwmm: CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "libwmm: LIBWMM_ENABLE_IPO=${LIBWMM_ENABLE_IPO}")
message(STATUS "libwmm: CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
