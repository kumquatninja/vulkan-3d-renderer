@echo off
setlocal enabledelayedexpansion

set "INPUT_DIR=%~dp0assets\models"
set "OUTPUT_DIR=%~dp0assets\models"

where ktx >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [ERROR] 'totkx' was not found in your PATH.
    echo Please install KTX-Software or add the directory containing ktx.exe to User PATH.
    exit /b 1
)

echo Starting PNG to KTX2 conversion...
echo Input Directory: %INPUT_DIR%
echo ---------------------------------------

for %%F in ("%INPUT_DIR%\*.png") do (
    set "PNG_FILE=%%~fF"
    set "KTX_FILE=%%~dpnF.ktx2"

    echo Converting: %%~nxF -^> %%~nF.ktx2

    ktx create --format R8G8B8A8_SRGB --assign-tf srgb "!PNG_FILE!" "!KTX_FILE!"
)

echo ---------------------------------------
echo Texture conversion complete!
pause