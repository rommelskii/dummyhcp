cmake_minimum_required(VERSION 3.10)
project(dhcp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#DummyHCP proper
set(SOURCES
    src/main.cpp
    src/packet.cpp
)
add_executable(dhcp ${SOURCES})
target_include_directories(dhcp PRIVATE 
    include    
    src       
)
set_target_properties(dhcp PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

#GTest 
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(
  test_main
  tests/test_main.cpp
  src/packet.cpp
)
target_include_directories(test_main PRIVATE 
    tests/fixtures
    include    
    src       
)
target_link_libraries(
  test_main
  GTest::gtest_main
)
set_target_properties(test_main PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
)
include(GoogleTest)
gtest_discover_tests(test_main)

