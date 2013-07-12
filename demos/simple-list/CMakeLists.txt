include(${QT_USE_FILE})
find_package(Automoc4 REQUIRED)

automoc4_add_executable(simple-list main.cpp simplelist.cpp)
target_link_libraries(simple-list qzeitgeist ${QT_LIBRARIES})
