include(ExternalProject)

ExternalProject_Add(
    googletest
    URL https://googletest.googlecode.com/files/gtest-1.7.0.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    INSTALL_COMMAND ""
	CMAKE_ARGS "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}"
)

ExternalProject_Get_Property(googletest binary_dir)
add_library(gtest STATIC IMPORTED)
add_library(gtest_main STATIC IMPORTED)
set_target_properties(gtest PROPERTIES IMPORTED_LOCATION ${binary_dir}/libgtest.a)
set_target_properties(gtest_main PROPERTIES IMPORTED_LOCATION ${binary_dir}/libgtest_main.a)

add_dependencies(gtest googletest)
add_dependencies(gtest_main googletest)

ExternalProject_Get_Property(googletest source_dir)
set(GTEST_INCLUDE_DIRS ${source_dir}/include CACHE STRING "")
set(GTEST_LIBRARIES gtest pthread CACHE STRING "")
set(GTEST_MAIN_LIBRARIES gtest_main pthread CACHE STRING "")
set(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} CACHE STRING "")
