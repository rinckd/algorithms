include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(GoogleTests
        Utilities.cpp
        GoogleTests.cpp)

target_link_libraries(GoogleTests gtest gtest_main)
target_link_libraries(GoogleTests Library)
