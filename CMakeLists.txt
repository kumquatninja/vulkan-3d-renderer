cmake_minimum_required(VERSION 3.10)
project(KQuatApp LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_program(GLSLANG_VALIDATOR glslangValidator REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)

set(SOURCES 
    src/main.cpp 
    src/Engine.cpp
    src/Renderer.cpp 
    src/WindowManager.cpp
    src/Input.cpp
    src/Camera.cpp
    src/Config.cpp
    external/inih/ini.c
    external/inih/cpp/INIReader.cpp
)

add_executable(KQuatApp ${SOURCES})

target_include_directories(KQuatApp PRIVATE 
    include
    ${Vulkan_INCLUDE_DIRS}
    external/glfw/include
    external/glm
    external/stb
    external/tinyobjloader
    external/inih
)

if(WIN32)
    target_link_libraries(KQuatApp PRIVATE
        Vulkan::Vulkan
        glfw
    )
elseif(UNIX)
    find_package(glfw3 REQUIRED)

    target_link_libraries(KQuatApp PRIVATE
        Vulkan::Vulkan
        glfw
    )
endif()

set_target_properties(KQuatApp PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

set(ASSET_SRC "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(SHADER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(SCENES_SRC "${CMAKE_CURRENT_SOURCE_DIR}/scenes")

set(SHADER_OUTPUTS
    "${SHADER_SRC}/vert.spv"
    "${SHADER_SRC}/frag.spv"
    "${SHADER_SRC}/grid_vert.spv"
    "${SHADER_SRC}/grid_frag.spv"
)

add_custom_command(
    OUTPUT ${SHADER_OUTPUTS}
    COMMAND ${GLSLANG_VALIDATOR} -V "${SHADER_SRC}/shader.vert" -o "${SHADER_SRC}/vert.spv"
    COMMAND ${GLSLANG_VALIDATOR} -V "${SHADER_SRC}/shader.frag" -o "${SHADER_SRC}/frag.spv"
    COMMAND ${GLSLANG_VALIDATOR} -V "${SHADER_SRC}/grid.vert" -o "${SHADER_SRC}/grid_vert.spv"
    COMMAND ${GLSLANG_VALIDATOR} -V "${SHADER_SRC}/grid.frag" -o "${SHADER_SRC}/grid_frag.spv"
    DEPENDS
        "${SHADER_SRC}/shader.vert"
        "${SHADER_SRC}/shader.frag"
        "${SHADER_SRC}/grid.vert"
        "${SHADER_SRC}/grid.frag"
    COMMENT "Compiling Vulkan shaders to SPIR-V..."
)

add_custom_target(CompileShaders ALL DEPENDS ${SHADER_OUTPUTS})
add_dependencies(KQuatApp CompileShaders)

add_custom_command(TARGET KQuatApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ASSET_SRC}/textures"
        "$<TARGET_FILE_DIR:KQuatApp>/assets/textures"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ASSET_SRC}/models"
        "$<TARGET_FILE_DIR:KQuatApp>/assets/models"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SHADER_SRC}"
        "$<TARGET_FILE_DIR:KQuatApp>/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/config.ini"
        "$<TARGET_FILE_DIR:KQuatApp>/config.ini"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SCENES_SRC}"
        "$<TARGET_FILE_DIR:KQuatApp>/scenes"
    
    COMMENT "Copying assets, shaders, and config to build directory..."
)