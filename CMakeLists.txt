cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MOD_VERSION "1.0.0")
set(MOD_ID "hax25dev.peak_macro")

project(PeakMacro VERSION ${MOD_VERSION})

# --------------- Geode Setup ---------------
if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "GEODE_SDK environment variable not set. Please set it to the path of your Geode SDK.")
endif()

set(GEODE_LINK_NIGHTLY ON)
add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# --------------- Source Files ---------------
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.hpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

# --------------- Target Properties ---------------
target_include_directories(${PROJECT_NAME} PRIVATE
    include/
    src/
)

# --------------- Link Geode ---------------
setup_geode_mod(${PROJECT_NAME})
