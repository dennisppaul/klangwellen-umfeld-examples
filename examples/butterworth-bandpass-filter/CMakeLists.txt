cmake_minimum_required(VERSION 3.12)

project(butterworth-bandpass-filter)

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

include(${UMGEBUNG_PATH}/cmake/default-pre.cmake)           # prepare umgebung ( should happen before `add_executable` )

# add source + header files from this directory

file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
include_directories(".")
target_compile_definitions(${PROJECT_NAME} PRIVATE UMGEBUNG_WINDOW_TITLE="${PROJECT_NAME}") # set window title

# add umgebung

add_subdirectory(${UMGEBUNG_PATH} ${CMAKE_BINARY_DIR}/umgebung-lib-${PROJECT_NAME})
add_umgebung_libs()

# add run target

add_custom_target(run
        COMMAND ${PROJECT_NAME}
        DEPENDS ${PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
