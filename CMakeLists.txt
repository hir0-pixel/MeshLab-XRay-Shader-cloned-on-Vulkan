cmake_minimum_required(VERSION 3.20)

project(ShaderOptimization)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------
# Dependencies via vcpkg toolchain
# ---------------------------------------
find_package(Vulkan REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# ---------------------------------------
# Sources
# ---------------------------------------
file(GLOB SRC_FILES
        CONFIGURE_DEPENDS
        src/*.cpp
        src/*.h
)

add_executable(ShaderOptimization ${SRC_FILES})

# Include dirs
target_include_directories(ShaderOptimization PRIVATE
        ${Vulkan_INCLUDE_DIRS}
)

# Link libs
target_link_libraries(ShaderOptimization PRIVATE
        ${Vulkan_LIBRARIES}
        assimp::assimp
        glm::glm
        glfw           # from glfw3 CONFIG
)

# ---------------------------------------
# Shader compilation with glslc
# ---------------------------------------

# find glslc
if (WIN32)
    if (NOT DEFINED ENV{VULKAN_SDK})
        message(FATAL_ERROR "VULKAN_SDK environment variable not set (needed to find glslc).")
    endif()
    set(GLSLC_EXECUTABLE "$ENV{VULKAN_SDK}/Bin/glslc.exe")
else()
    find_program(GLSLC_EXECUTABLE NAMES glslc REQUIRED)
endif()

set(SHADER_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(SHADER_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")

file(GLOB SHADER_SOURCES
        "${SHADER_SRC_DIR}/*.vert"
        "${SHADER_SRC_DIR}/*.frag"
)

set(SPV_SHADERS "")

foreach(SHADER ${SHADER_SOURCES})
    get_filename_component(FILE_NAME ${SHADER} NAME)   # e g basic.vert
    set(SPV_FILE "${SHADER_BIN_DIR}/${FILE_NAME}.spv") # -> basic.vert.spv

    add_custom_command(
            OUTPUT ${SPV_FILE}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_BIN_DIR}
            COMMAND ${GLSLC_EXECUTABLE} ${SHADER} -o ${SPV_FILE}
            DEPENDS ${SHADER}
            COMMENT "Compiling shader ${FILE_NAME} -> ${SPV_FILE}"
    )

    list(APPEND SPV_SHADERS ${SPV_FILE})
endforeach()

# Custom target that builds all shaders
add_custom_target(Shaders ALL DEPENDS ${SPV_SHADERS})

# Make the exe depend on compiled shaders
add_dependencies(ShaderOptimization Shaders)
