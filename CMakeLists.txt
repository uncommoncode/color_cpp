cmake_minimum_required(VERSION 2.8)

include(cmake/google_test.cmake)

project(color)

set(sources color/transformation.cpp
            color/palette.cpp
            color/interpolation.cpp
            color/internal/math.cpp)
set(headers color/interpolation.hpp
            color/palette.hpp
            color/space.hpp
            color/transformation.hpp
            color/internal/math.hpp)

add_library(color ${sources} ${headers})

enable_testing()
add_executable(test_color test/test_transformation.cpp
                          test/test_palette.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${GTEST_INCLUDE_DIRS})
target_link_libraries(test_color color ${GTEST_BOTH_LIBRARIES})
add_dependencies(test_color googletest)
add_test(TestColor test_color)

install(TARGETS color
        ARCHIVE DESTINATION lib)
install(DIRECTORY color/
        DESTINATION include/color
        FILES_MATCHING PATTERN "*.hpp"
        PATTERN "internal" EXCLUDE)
