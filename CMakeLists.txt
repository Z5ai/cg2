cmake_minimum_required(VERSION 3.15)
set(COURSE_NAME "CG2")
project(${COURSE_NAME} C CXX)


# Add CGV framework
set(CGV_DIR ${CMAKE_SOURCE_DIR}/framework)
find_package(CGV REQUIRED)

# Add exercises
# - configure for common root
set(CG2_ROOT_DIR ${CMAKE_SOURCE_DIR})
# - add exercises
add_subdirectory(exercise1)
add_subdirectory(exercise2)
add_subdirectory(exercise45)
