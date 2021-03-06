include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-fvisibility=hidden __HAVE_GCC_VISIBILITY)
if (__HAVE_GCC_VISIBILITY AND NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
endif (__HAVE_GCC_VISIBILITY AND NOT WIN32)

find_package(Automoc4)
include(${QT_USE_FILE})

add_definitions(-DBUILD_QZEITGEIST)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})

set(qzeitgeist_HEADERS
    mimetype.h
    symbol.h
    tools.h
    timerange.h
    subject.h
    event.h
    resultset.h
    monitor.h
    log.h
    extensions/index.h
    extensions/datasource.h
    extensions/datasourceregistry.h
    qzeitgeist.h
    qzeitgeist_defines.h
    ${CMAKE_CURRENT_BINARY_DIR}/qzeitgeist_version.h
)

set(qzeitgeist_SRCS
    mimetype.cpp
    symbol.cpp
    tools.cpp
    timerange.cpp
    subject.cpp
    event.cpp
    resultset.cpp
    monitor.cpp
    log.cpp
    extensions/index.cpp
    extensions/datasource.cpp
    extensions/datasourceregistry.cpp
)

configure_file(qzeitgeist_version.h.in ${CMAKE_CURRENT_BINARY_DIR}/qzeitgeist_version.h @ONLY)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-long-long -pedantic")
# Generated zeitgeist_event_constructv_full is reported as unused
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")

automoc4_add_library(qzeitgeist SHARED ${qzeitgeist_SRCS})

include_directories(${ZG2_INCLUDE_DIRS})
link_directories(${ZG2_LIBRARY_DIRS})

target_link_libraries(qzeitgeist ${ZG2_LIBRARIES} ${QT_QTCORE_LIBRARIES})
set_target_properties(qzeitgeist PROPERTIES VERSION "${QZEITGEIST_VERSION}"
                      VERSION ${QZEITGEIST_VERSION}
                      SOVERSION ${QZEITGEIST_LIB_SOVERSION}
                      )

install(TARGETS qzeitgeist EXPORT QZeitgeistExport DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})

install(FILES ${qzeitgeist_HEADERS} DESTINATION include/QZeitgeist)
