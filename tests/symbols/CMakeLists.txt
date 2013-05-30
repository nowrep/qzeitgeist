include(${QT_USE_FILE})
find_package(Automoc4 REQUIRED)

set(INC_DIR ${CMAKE_SOURCE_DIR}/include)
include_directories(${INC_DIR})

set(test-symbols_SRCS
    test-symbols.cpp
)

set(test-symbols_HDRS
    test-symbols.h
)

automoc4_add_executable(test-symbols ${test-symbols_SRCS})
target_link_libraries(test-symbols qzeitgeist ${QT_LIBRARIES})

