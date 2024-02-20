cmake_minimum_required(VERSION 3.21)

project(lab_07)
set(SOURCE_FILES
        tbb.cpp
)
find_package(TBB REQUIRED)
add_executable(tbb.cpp)
target_link_libraries(graph TBB::tbb)

set_target_properties(graph PROPERTIES CXX_STANDARD 17)