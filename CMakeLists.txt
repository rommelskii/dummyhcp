# 1. Set the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# 2. Define the project name and version
project(dhcp VERSION 1.0)

# 3. Specify the C++ standard (Modern C++ is recommended)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. Include directories (in case you add .h files in src later)
include_directories(src)

# 5. Locate the source files
# Instead of listing files manually, we tell CMake to look in /src
file(GLOB SOURCES "src/*.cpp")

# 6. Define the executable and its source files
add_executable(dhcp ${SOURCES}
        src/dhcp_packet.cpp
        include/dhcp_packet.h
        include/server.h
        src/main.cpp
        src/server.cpp)

target_include_directories(dhcp PRIVATE include)

# 7. Provide a message when configuration is successful
message(STATUS "Project configured for ${CMAKE_SYSTEM_NAME}")