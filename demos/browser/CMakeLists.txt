include(${QT_USE_FILE})
find_package(Automoc4 REQUIRED)

automoc4_add_executable(browser main.cpp zgbrowser.cpp)
target_link_libraries(browser qzeitgeist ${QT_LIBRARIES} ${QT_QTWEBKIT_LIBRARY})
