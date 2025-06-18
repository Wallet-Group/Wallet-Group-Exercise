cmake_minimum_required(VERSION 3.16)
project(ServiceApp VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Compiler flags - MSVC specific
if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/W4 /Zi /Od /RTC1")
    set(CMAKE_CXX_FLAGS_RELEASE "/W4 /O2 /DNDEBUG")
else()
    # GCC/Clang flags
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif()

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/interface
    ${CMAKE_SOURCE_DIR}/modules/json
    ${CMAKE_SOURCE_DIR}/modules/File
    ${CMAKE_SOURCE_DIR}/helper
    ${CMAKE_SOURCE_DIR}/screen
)

# Collect source files
file(GLOB_RECURSE SERVICE_SOURCES "services/*.cpp")
file(GLOB_RECURSE SERVICE_MANAGER_SOURCES "helper/File/*.cpp")
file(GLOB_RECURSE JSON_MODULE_SOURCES "modules/json/json.hpp")
file(GLOB_RECURSE FILE_MODULE_SOURCES "helper/File/JsonFileManager.cpp")
file(GLOB_RECURSE HELPER_SOURCES "helper/*.cpp")
file(GLOB_RECURSE MODEL_SOURCES "models/*.cpp")
file(GLOB_RECURSE DEFINE_SOURCES "defined/*.cpp")
file(GLOB_RECURSE SCREEN_SOURCES "screen/*.cpp")

# Create executable
add_executable(${PROJECT_NAME}
    main.cpp
    ${SERVICE_SOURCES}
    ${SERVICE_MANAGER_SOURCES}
    ${JSON_MODULE_SOURCES}
    ${FILE_MODULE_SOURCES}
    ${HELPER_SOURCES}
    ${MODEL_SOURCES}
    ${DEFINE_SOURCES}
    ${SCREEN_SOURCES}
)

# Link libraries - MSVC specific
if(MSVC)
    target_link_libraries(${PROJECT_NAME} PRIVATE)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
endif()

# Install target
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
