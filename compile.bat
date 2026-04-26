cl /EHsc /Zi /std:c++17 main.cpp ^
   /I"external\glfw-3.4.bin.WIN64\include" ^
   /I"%VULKAN_SDK%\Include" ^
   /link ^
   /LIBPATH:"external\glfw-3.4.bin.WIN64\lib-vc2022" ^
   /LIBPATH:"%VULKAN_SDK%\Lib" ^
   glfw3.lib vulkan-1.lib user32.lib gdi32.lib shell32.lib msvcrt.lib