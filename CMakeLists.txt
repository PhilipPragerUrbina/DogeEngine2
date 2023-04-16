cmake_minimum_required(VERSION 3.22)
project(DogeEngine2)

set(CMAKE_CXX_STANDARD 17)

# shader source locations
set(SHADER_DIR ${PROJECT_SOURCE_DIR}/DogeEngine/Rendering/Shaders)
# Shader compiler location(GLSC)
set(Vulkan_GLSLC_EXECUTABLE C:/VulkanSDK/1.3.239.0/Bin/glslangValidator.exe)
#Set shader output location
set(SHADER_COMPILE_DIR ${PROJECT_SOURCE_DIR}/Resources/CompiledShaders)

# find shaders
file(GLOB SHADERS ${SHADER_DIR}/*.vert ${SHADER_DIR}/*.frag)

#compile shaders  to spir-v
# Credit: https://stackoverflow.com/questions/67040258/cmake-compile-glsl-shaders
foreach(SHADER IN LISTS SHADERS)
    get_filename_component(FILENAME ${SHADER} NAME)
    add_custom_command(OUTPUT ${SHADER_COMPILE_DIR}/${FILENAME}.spv
            COMMAND ${Vulkan_GLSLC_EXECUTABLE} -G ${SHADER} -o ${SHADER_COMPILE_DIR}/${FILENAME}.spv
            DEPENDS ${SHADER}
            COMMENT "Compiling ${FILENAME}")
    list(APPEND SPV_SHADERS ${SHADER_COMPILE_DIR}/${FILENAME}.spv)
endForeach()
add_custom_target(SHADER_TARGET ALL DEPENDS ${SPV_SHADERS})


# Use GLFW, GLM, GLAD and other headers
include_directories(External/GLFW/include External/glm/glm External/stb External/glad/include External/loaders)

#Find sources
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "DogeEngine/*.hpp" "External/glad/*.c")

add_executable(DogeEngine2 main.cpp ${sources})

#add bundled windows 64 bit binaries as well as assimp
target_link_libraries(DogeEngine2 opengl32.lib ${PROJECT_SOURCE_DIR}/External/GLFW/lib/glfw3.lib)

#add shaders as dependency
add_dependencies(DogeEngine2 SHADER_TARGET)