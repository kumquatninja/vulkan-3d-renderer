cl /EHsc /Zi /O2 /std:c++17 src/main.cpp src/WindowManager.cpp src/Renderer.cpp ^
   /I"include\" ^
   /I"third_party\glfw-3.4.bin.WIN64\include" ^
   /I"third_party\glm" ^
   /I"third_party\stb" ^
   /I"third_party\tinyobjloader" ^
   /I"%VULKAN_SDK%\Include" ^
   /link ^
   /LIBPATH:"third_party\glfw-3.4.bin.WIN64\lib-vc2022" ^
   /LIBPATH:"%VULKAN_SDK%\Lib" ^
   glfw3.lib vulkan-1.lib user32.lib gdi32.lib shell32.lib msvcrt.lib