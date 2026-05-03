cmake_minimum_required(VERSION 3.10)
project(KQuatApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

set(SOURCES 
    src/main.cpp 
    src/Renderer.cpp 
    src/WindowManager.cpp
)

add_executable(KQuatApp ${SOURCES})

target_include_directories(KQuatApp PRIVATE 
    include 
    third_party/glfw-3.4.bin.WIN64/include
    third_party/glm
    third_party/stb
    third_party/tinyobjloader
    ${Vulkan_INCLUDE_DIRS}
)

if(WIN32)
    target_link_libraries(KQuatApp PRIVATE 
        Vulkan::Vulkan 
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw-3.4.bin.WIN64/lib-vc2022/glfw3dll.lib"
    )
elseif(UNIX)
    find_package(glfw3 REQUIRED)

    target_link_libraries(KQuatApp PRIVATE_HEADER
        Vulkan::Vulkan
        glfw
    )
endif()

set_target_properties(KQuatApp PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

if(WIN32)
    set(GLFW_DLL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw-3.4.bin.WIN64/lib-vc2022/glfw3.dll")

    add_custom_command(TARGET KQuatApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${GLFW_DLL_PATH}"
        "$<TARGET_FILE_DIR:KQuatApp>"
    )
endif()

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets/textures" DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug/assets")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets/models" DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug/assets")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/shaders" DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug/")