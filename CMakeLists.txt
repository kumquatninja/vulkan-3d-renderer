cmake_minimum_required(VERSION 3.10)
project(KQuatApp LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)

set(SOURCES 
    src/main.cpp 
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
    
    COMMENT "Copying assets, shaders, and config to build directory..."
)