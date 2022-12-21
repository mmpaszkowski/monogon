# SOURCES AND HEADERS
set(LIBRARY_SOURCES
    Shape.cpp
    Index.cpp)

set(LIBRARY_HEADERS
    ../include/monogon/tool/Shape.h
    ../include/monogon/tool/Index.h
    ../include/monogon/tool/OneHot.h
    ../include/monogon/tool/Slice.h
    ../include/monogon/metric/Metric.h
    ../include/monogon/metric/CategoricalAccuracy.h
    ../include/monogon/Vector.h
    ../include/monogon/Array.h
    ../include/monogon/Tensor.h
    ../include/monogon/Operation.h
    ../include/monogon/Variable.h
    ../include/monogon/tool/OneHot.h
    ../include/monogon/tool/Slice.h
    ../include/monogon/view/ModelRenderer.h ../include/monogon/metric/Metric.h ../include/monogon/metric/CategoricalAccuracy.h ../include/monogon/utils/low_level.h low_level.cpp)

set(LIBRARY_INCLUDES
    ../include
    "${CMAKE_BINARY_DIR}/configured_files/include")


add_library(${MONOGON_PROJECT_NAME} STATIC
    ${LIBRARY_SOURCES}
    ${LIBRARY_HEADERS})

target_link_libraries(${MONOGON_PROJECT_NAME} PUBLIC
        fmt::fmt
        indicators::indicators
        openblas
    )

target_include_directories(${MONOGON_PROJECT_NAME} PUBLIC ${LIBRARY_INCLUDES})

add_library(${MONOGON_PROJECT_NAME}::${MONOGON_PROJECT_NAME} ALIAS ${MONOGON_PROJECT_NAME})

if (${MONOGON_ENABLE_WARNINGS})
    target_set_warnings(TARGET ${MONOGON_PROJECT_NAME} ENABLE ON AS_ERROR OFF)
endif ()

add_target_static_analyers(${MONOGON_PROJECT_NAME})
