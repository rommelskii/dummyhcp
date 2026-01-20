cmake_minimum_required(VERSION 3.10)
project(dhcp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(dhcp_core OBJECT src/packet.cpp)
target_include_directories(dhcp_core PUBLIC include src)

add_executable(dhcp src/main.cpp)
target_link_libraries(dhcp PRIVATE dhcp_core)
set_target_properties(dhcp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

set(DHCP_TESTS 
    initialization_tests 
    header_tests 
    serialization_tests 
    option_tests
)

foreach(test_name ${DHCP_TESTS})
    add_executable(${test_name} tests/${test_name}.cpp)
    target_link_libraries(${test_name} PRIVATE dhcp_core GTest::gtest_main)
    target_include_directories(${test_name} PRIVATE tests/fixtures include src)
    set_target_properties(${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")
    gtest_discover_tests(${test_name})
endforeach()
