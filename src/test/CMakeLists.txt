
enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/src/test/*.cpp)

add_executable(runUnitTests ${TEST_SRC_FILES})


target_link_libraries(runUnitTests PRIVATE gtest gtest_main CONAN_PKG::cpp-taskflow)

add_test(UnitTests runUnitTests)
