cmake_minimum_required(VERSION 3.10)
project(dhcp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Explicitly list your source files
# This is preferred over GLOB because CMake cannot always detect 
# when you add a new file to the folder without a re-configure.
set(SOURCES
    src/main.cpp
    src/dhcp_packet.cpp
    # Add new .cpp files here as you create them
)

# 2. Define the executable using the variable
add_executable(dhcp ${SOURCES})

# 3. Handle include directories properly
# This tells the compiler where to look for your .h files.
target_include_directories(dhcp PRIVATE 
    include    # For your custom headers
    src        # In case you have headers sitting next to .cpp files
)

# 4. Success message
message(STATUS "DHCP Project configured for ${CMAKE_SYSTEM_NAME}")
