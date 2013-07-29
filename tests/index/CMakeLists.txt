include(${QT_USE_FILE})
find_package(Automoc4 REQUIRED)

set(INC_DIR ${CMAKE_SOURCE_DIR}/include)
include_directories(${INC_DIR})

set(test-index_SRCS
    test-index.cpp
)

set(test-index_HDRS
    test-index.h
)

automoc4_add_executable(test-index ${test-index_SRCS})
target_link_libraries(test-index qzeitgeist ${QT_LIBRARIES})

