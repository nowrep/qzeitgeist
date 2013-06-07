include(${QT_USE_FILE})
find_package(Automoc4 REQUIRED)

set(INC_DIR ${CMAKE_SOURCE_DIR}/include)
include_directories(${INC_DIR})

set(test-event_SRCS
    test-event.cpp
)

set(test-event_HDRS
    test-event.h
)

automoc4_add_executable(test-event ${test-event_SRCS})
target_link_libraries(test-event qzeitgeist ${QT_LIBRARIES})

