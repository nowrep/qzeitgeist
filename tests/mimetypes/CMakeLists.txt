include(${QT_USE_FILE})
find_package(Automoc4 REQUIRED)

set(INC_DIR ${CMAKE_SOURCE_DIR}/include)
include_directories(${INC_DIR})

set(test-mimetypes_SRCS
    test-mimetypes.cpp
)

set(test-mimetypes_HDRS
    test-mimetypes.h
)

automoc4_add_executable(test-mimetypes ${test-mimetypes_SRCS})
target_link_libraries(test-mimetypes qzeitgeist ${QT_LIBRARIES})

