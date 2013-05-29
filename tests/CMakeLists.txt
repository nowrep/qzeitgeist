include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}/include ${CMAKE_BINARY_DIR}/src)

add_subdirectory(mimetypes)
#add_subdirectory(log)
