cmake_minimum_required(VERSION 3.10)
project(computer_club)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)

set(CORE_SOURCES
    src/Time.cpp
    src/Club.cpp
)

add_library(core_lib STATIC ${CORE_SOURCES})
target_include_directories(core_lib PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable(task src/main.cpp)
target_link_libraries(task PRIVATE core_lib)

if(BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)

    # Unit
    file(GLOB TEST_SOURCES tests/*.cpp)
    if(TEST_SOURCES)
        add_executable(all_tests ${TEST_SOURCES})
        target_link_libraries(all_tests
            PRIVATE
                core_lib
                GTest::gtest_main
        )
        include(GoogleTest)
        gtest_discover_tests(all_tests
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            PROPERTIES FAIL_REGULAR_EXPRESSION "FAILED"
        )
    else()
        message(WARNING "No test sources found in tests/")
    endif()
endif()