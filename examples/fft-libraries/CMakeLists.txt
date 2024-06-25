cmake_minimum_required(VERSION 3.12)

project(fft-libraries)

set(UMGEBUNG_PATH   "../../libraries/umgebung/")            # point to umgebung library
include_directories("../../libraries/klangwellen/include")  # add klangwellen library

# --------------------- ADAPT PATHS ABOVE ACCORDINGLY --------------------- #
# ( e.g for development )
#
# set(UMGEBUNG_PATH "$ENV{HOME}/Documents/dev/umgebung/git/umgebung")
# include_directories("../../../klangwellen/include")
#
# --------------------- ADAPT PATHS ABOVE ACCORDINGLY --------------------- #

option(DISABLE_GRAPHICS "Disable graphic output" OFF)
option(DISABLE_VIDEO "Disable video output" ON)
option(DISABLE_AUDIO "Disable audio output + input" OFF)

include(${UMGEBUNG_PATH}/cmake/default-pre.cmake)           # prepare umgebung ( before `add_executable` )

# add source + header files from this directory
set(CMSIS_DSP_PATH "${CMAKE_SOURCE_DIR}/libraries/cmsis_dsp")
set(KISS_FFT_PATH "${CMAKE_SOURCE_DIR}/libraries/kiss_fft")

file(GLOB SOURCE_FILES 
    "*.cpp" 
    "*.c" 
    "${KISS_FFT_PATH}/kiss_fft.c"
    "${KISS_FFT_PATH}/kiss_fftr.c"
    "${CMSIS_DSP_PATH}/Source/TransformFunctions/arm_cfft_f32.c"
    "${CMSIS_DSP_PATH}/Source/ComplexMathFunctions/arm_cmplx_mag_f32.c"
    "${CMSIS_DSP_PATH}/Source/StatisticsFunctions/arm_max_f32.c"
    "${CMSIS_DSP_PATH}/Source/SupportFunctions/arm_copy_f32.c"
    "${CMSIS_DSP_PATH}/Source/TransformFunctions/arm_bitreversal2.c"
    "${CMSIS_DSP_PATH}/Source/TransformFunctions/arm_cfft_init_f32.c"
    "${CMSIS_DSP_PATH}/Source/TransformFunctions/arm_cfft_f32.c"
    "${CMSIS_DSP_PATH}/Source/TransformFunctions/arm_cfft_radix8_f32.c"
    "${CMSIS_DSP_PATH}/Source/TransformFunctions/arm_rfft_fast_f32.c"
    "${CMSIS_DSP_PATH}/Source/TransformFunctions/arm_rfft_fast_init_f32.c"
    "${CMSIS_DSP_PATH}/Source/CommonTables/arm_const_structs.c"
    "${CMSIS_DSP_PATH}/Source/CommonTables/arm_common_tables.c"
    "${CMSIS_DSP_PATH}/Source/FastMathFunctions/arm_sin_f32.c"
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_definitions(${PROJECT_NAME} PUBLIC USE_CMSIS_DSP)

# find_package(FFTW REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE FFTW::FFTW)
set(FFTW_INCLUDE_DIR "/opt/homebrew/Cellar/fftw/3.3.10_1/include")
set(FFTW_LIBRARY "/opt/homebrew/Cellar/fftw/3.3.10_1/lib/libfftw3.3.dylib")
target_include_directories(${PROJECT_NAME} PRIVATE ${FFTW_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFTW_LIBRARY})

include_directories(".")
include_directories("./libraries/kiss_fft")
include_directories("./libraries/cmsis_dsp/Include")

include(${UMGEBUNG_PATH}/cmake/default-post.cmake)          # prepare umgebung ( after `add_executable` )

