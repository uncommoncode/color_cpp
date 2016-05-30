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

set(compiler_features cxx_generalized_initializers
                      cxx_auto_type
                      cxx_constexpr
                      cxx_lambdas
                      cxx_lambda_init_captures
                      cxx_nullptr
                      cxx_range_for
                      cxx_strong_enums
                      cxx_static_assert
                      cxx_right_angle_brackets
                      cxx_relaxed_constexpr)

add_library(color ${sources} ${headers})
target_compile_features(color PUBLIC ${compiler_features})

enable_testing()
add_executable(test_color test/test_transformation.cpp
                          test/test_palette.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${GTEST_INCLUDE_DIRS})
target_compile_features(test_color PUBLIC ${compiler_features})
target_link_libraries(test_color color ${GTEST_BOTH_LIBRARIES})
add_dependencies(test_color googletest)
add_test(TestColor test_color)

install(TARGETS color
        ARCHIVE DESTINATION lib)
install(DIRECTORY color/
        DESTINATION include/color
        FILES_MATCHING PATTERN "*.hpp"
        PATTERN "internal" EXCLUDE)
