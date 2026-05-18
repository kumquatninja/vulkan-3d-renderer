cmake_minimum_required(VERSION 3.10)
project(KQuatApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

set(SOURCES 
    src/main.cpp 
    src/Renderer.cpp 
    src/WindowManager.cpp
    src/Input.cpp
    src/Camera.cpp
)

add_executable(KQuatApp ${SOURCES})

target_include_directories(KQuatApp PRIVATE 
    include 
    external/glfw-3.4.bin.WIN64/include
    external/glm
    external/stb
    external/tinyobjloader
    ${Vulkan_INCLUDE_DIRS}
)

if(WIN32)
    target_link_libraries(KQuatApp PRIVATE
        Vulkan::Vulkan
        "${CMAKE_CURRENT_SOURCE_DIR}/external/glfw-3.4.bin.WIN64/lib-vc2022/glfw3dll.lib"
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

if(WIN32)
    set(GLFW_DLL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/glfw-3.4.bin.WIN64/lib-vc2022/glfw3.dll")

    add_custom_command(TARGET KQuatApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${GLFW_DLL_PATH}"
        "$<TARGET_FILE_DIR:KQuatApp>"
    )
endif()

set(ASSET_SRC "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(SHADER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/shaders")

add_custom_command(TARGET KQuatApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:KQuatApp>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ASSET_SRC}/textures"
        "$<TARGET_FILE_DIR:KQuatApp>/assets/textures"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ASSET_SRC}/models"
        "$<TARGET_FILE_DIR:KQuatApp>/assets/models"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SHADER_SRC}"
        "$<TARGET_FILE_DIR:KQuatApp>/shaders"
    
    COMMENT "Copying assets and shaders to build directory..."
)