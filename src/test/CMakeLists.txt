
enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/src/test/*.cpp)

# TODO find a cleaner way to link sources file if possible

file(GLOB RT_SOURCE
  ${PROJECT_SOURCE_DIR}/src/util.cpp ${PROJECT_SOURCE_DIR}/src/geometry/geometry_util.cpp ${PROJECT_SOURCE_DIR}/lodepng/lodepng.cpp
  )

add_executable(runUnitTests ${TEST_SRC_FILES} ${RT_SOURCE})


target_link_libraries(runUnitTests PRIVATE gtest gtest_main)

add_test(UnitTests runUnitTests)
