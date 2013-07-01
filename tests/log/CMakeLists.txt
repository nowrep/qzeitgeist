include(${QT_USE_FILE})
find_package(Automoc4 REQUIRED)

set(INC_DIR ${CMAKE_SOURCE_DIR}/include)
include_directories(${INC_DIR})

set(test-log_SRCS
    test-log.cpp
)

set(test-log_HDRS
    test-log.h
)

automoc4_add_executable(test-log ${test-log_SRCS})
target_link_libraries(test-log qzeitgeist ${QT_LIBRARIES})

