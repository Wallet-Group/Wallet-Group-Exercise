cmake_minimum_required(VERSION 3.16)
project(ServiceApp VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Collect all source files from services directory
file(GLOB_RECURSE SERVICE_SOURCES "services/*.cpp")

# Create executable
add_executable(${PROJECT_NAME} 
    main.cpp
    ${SERVICE_SOURCES}
)

# Link libraries if needed (example: pthread for threading)
# target_link_libraries(${PROJECT_NAME} pthread)

# Install target
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
