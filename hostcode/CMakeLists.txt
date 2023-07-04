# Basic project settings
cmake_minimum_required(VERSION 3.8)
project(razor-arm-host VERSION 1.0.0)

# Set some build settings
set(CMAKE_BUILD_TYPE Release)
set(BUILD_SHARED_LIBS OFF)

# Create executable for GLSLSlime
add_executable(razor-arm-host main.cpp)

# Ensure that optimisation is enabled
target_compile_features(razor-arm-host PRIVATE cxx_std_17)
if(UNIX)
    target_compile_options(razor-arm-host PRIVATE -O3) # Assume GCC or Clang
endif()

# Fetch imgui/glfw/glad/glm
include(FetchContent)
FetchContent_Declare(
    imgui-glfw-glad-glm
    GIT_REPOSITORY https://github.com/LJ3D/imgui-glfw-glad-glm
    GIT_SHALLOW ON
    UPDATE_DISCONNECTED ON
)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(imgui-glfw-glad-glm)

# Link everything together
target_include_directories(razor-arm-host PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(
    razor-arm-host
    imgui
    glm
)

# Copy GLSL files to build directory
add_custom_target(
    copy_glsl_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/GLSL
        ${CMAKE_BINARY_DIR}/GLSL
    COMMENT "Copying GLSL files to build directory"
)
add_dependencies(razor-arm-host copy_glsl_files)