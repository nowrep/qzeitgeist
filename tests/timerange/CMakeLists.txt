include(${QT_USE_FILE})
find_package(Automoc4 REQUIRED)

set(INC_DIR ${CMAKE_SOURCE_DIR}/include)
include_directories(${INC_DIR})

set(test-timerange_SRCS
    test-timerange.cpp
)

set(test-timerange_HDRS
    test-timerange.h
)

automoc4_add_executable(test-timerange ${test-timerange_SRCS})
target_link_libraries(test-timerange qzeitgeist ${QT_LIBRARIES})

