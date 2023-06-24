cmake_minimum_required(VERSION 3.16)

set(MONOGON_PROJECT_NAME monogon)

# PROJECT
project(${MONOGON_PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)

# GLOBAL CMAKE VARIABLES
set(CMAKE_CXX_STANDARD              20)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS   ON)
set(CMAKE_CXX_FLAGS "-O3")

# MAIN_PROJECT CHECK
set(MONOGON_MAIN_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MONOGON_MAIN_PROJECT ON)
endif()

# OPTIONS
option(MONOGON_ENABLE_WARNINGS              "Enable to add warnings to a target." ${MONOGON_MAIN_PROJECT})
option(MONOGON_ENABLE_TESTING               "Enable a Unit Testing build."        ${MONOGON_MAIN_PROJECT})
option(MONOGON_ENABLE_COVERAGE              "Enable a Code Coverage build."       ${MONOGON_MAIN_PROJECT})

option(MONOGON_ENABLE_CLANG_TIDY            "Enable to add clang tidy."           OFF) # Have to be fixed
option(MONOGON_ENABLE_CLANG_FORMAT          "Enable to add clang-format."         ${MONOGON_MAIN_PROJECT})
option(MONOGON_ENABLE_CPPCHECK              "Enable to add cppcheck."             ${MONOGON_MAIN_PROJECT})

# CMAKE MODULES
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)
include(ConfigSafeGuards)
include(Warnings)
include(AddGitSubmodule)
include(FetchContent)
include(Tools)
include(LTO)

# EXTERNAL LIBRARIES
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 9.1.0
        GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
        indicators
        GIT_REPOSITORY https://github.com/mmpaszkowski/indicators
        GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(indicators)

# SUBDIRECTORIES
add_subdirectory(configured)
add_subdirectory(src)

# TESTS
if (MONOGON_ENABLE_TESTING)
    include(CTest)
    if (MONOGON_ENABLE_COVERAGE)
        include(CodeCoverage)
        append_coverage_compiler_flags()
    endif()
    add_subdirectory(tests)
    add_subdirectory(benchmark)
endif()

# INSTALL TARGETS
install(TARGETS ${MONOGON_PROJECT_NAME}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)