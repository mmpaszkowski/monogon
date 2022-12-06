# SOURCES AND HEADERS
set(LIBRARY_SOURCES
    library.cpp)

set(LIBRARY_HEADERS
    ../include/monogon/Vector.h
    ../include/monogon/Matrix.h)

set(LIBRARY_INCLUDES
    ../include
    "${CMAKE_BINARY_DIR}/configured_files/include")


add_library(${MONOGON_PROJECT_NAME} STATIC
    ${LIBRARY_SOURCES}
    ${LIBRARY_HEADERS})

target_link_libraries(${MONOGON_PROJECT_NAME} PRIVATE fmt::fmt)

target_include_directories(${MONOGON_PROJECT_NAME} PUBLIC ${LIBRARY_INCLUDES})

add_library(${MONOGON_PROJECT_NAME}::${MONOGON_PROJECT_NAME} ALIAS ${MONOGON_PROJECT_NAME})

if (${MONOGON_ENABLE_WARNINGS})
    target_set_warnings(TARGET ${MONOGON_PROJECT_NAME} ENABLE ON AS_ERROR OFF)
endif ()

add_target_static_analyers(${MONOGON_PROJECT_NAME})
