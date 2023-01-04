set(CMAKE_CXX_FLAGS "-O3")

set(TEST_MAIN ${MONOGON_PROJECT_NAME}_benchmark_tests)
set(TEST_SOURCES
        main.cpp)

find_package(OpenMP REQUIRED)

FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.7.1)
FetchContent_MakeAvailable(googlebenchmark)


add_executable(${TEST_MAIN} ${TEST_SOURCES})

target_link_libraries(${TEST_MAIN} PUBLIC ${MONOGON_PROJECT_NAME} ${OpenMP_CXX_FLAGS})
target_link_libraries(${TEST_MAIN} PRIVATE
        benchmark::benchmark
        )
