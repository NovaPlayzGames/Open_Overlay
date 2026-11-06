@echo off
REM ============================================================
REM   Open Overlay OBS Plugin - Build Script
REM ============================================================
REM   
REM   This script builds the Open Overlay OBS plugin.
REM   It will try to find OBS Studio automatically, or you
REM   can provide the path.
REM   
REM   Requirements:
REM   - Visual Studio 2019 or 2022
REM   - CMake 3.16+
REM   - OBS Studio (for development headers)
REM
REM ============================================================

echo.
echo ============================================
echo   Open Overlay OBS Plugin Build Script
echo ============================================
echo.

REM Check for required tools
echo Checking CMake...
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: CMake not found. Please install CMake 3.16+
    echo Download: https://cmake.org/download/
    pause
    exit /b 1
)
echo   [OK] CMake found

echo Checking Visual Studio...
where cl >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo NOTE: MSVC not in PATH. 
    echo Please run this from "Developer Command Prompt for VS" 
    echo or "x64 Native Tools Command Prompt"
    echo.
)

REM Find OBS Studio
echo.
echo Looking for OBS Studio...
set "OBS_STUDIO_PATH="
set "OBS_STUDIO_PATH=C:\obs-studio"
set /p OBS_STUDIO_PATH="Enter OBS Studio path [%OBS_STUDIO_PATH%]: "
if "%OBS_STUDIO_PATH%"=="" set OBS_STUDIO_PATH=C:\obs-studio

echo Using OBS Studio path: %OBS_STUDIO_PATH%

REM Check if OBS Studio exists
if not exist "%OBS_STUDIO_PATH%" (
    echo.
    echo ERROR: OBS Studio not found at: %OBS_STUDIO_PATH%
    echo.
    echo Please do ONE of the following:
    echo.
    echo   OPTION 1: Clone OBS Studio source code:
    echo     cd C:\Users\20leo\Desktop
    echo     git clone --recursive https://github.com/obsproject/obs-studio.git
    echo.
    echo   OPTION 2: Use pre-built OBS from:
    echo     https://github.com/obsproject/obs-studio/releases
    echo.
    echo   Then update the path above.
    echo.
    pause
    exit /b 1
)

echo   [OK] OBS Studio found at %OBS_STUDIO_PATH%

REM Create build directory
echo.
echo Creating build directory...
cd /d "%~dp0"
if not exist "build" mkdir build
cd build

REM Configure with CMake
echo.
echo Running CMake configuration...
echo.

cmake .. -G "Visual Studio 17 2022" -A x64 ^
    -DOBS_STUDIO_PATH="%OBS_STUDIO_PATH%" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_INSTALL_PREFIX="%OBS_STUDIO_PATH%"

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: CMake configuration failed!
    echo.
    echo Make sure you have:
    echo   1. Cloned OBS Studio with submodules: 
    echo      git clone --recursive https://github.com/obsproject/obs-studio.git
    echo   2. Built OBS Studio at least once
    echo.
    pause
    exit /b 1
)

echo.
echo Building plugin...
echo.

REM Build
cmake --build . --config Release --parallel

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo ============================================
echo   Build Complete!
echo ============================================
echo.

REM Check for output in multiple possible locations
set "FOUND_DLL="
set "OUTPUT_DIR="

REM Check the CMake configured output directory
if exist "bin\Release\open_overlay_plugin.dll" (
    set "FOUND_DLL=bin\Release\open_overlay_plugin.dll"
    set "OUTPUT_DIR=bin\Release"
)

REM Check if exists in build root
if not defined FOUND_DLL (
    if exist "open_overlay_plugin.dll" (
        set "FOUND_DLL=open_overlay_plugin.dll"
        set "OUTPUT_DIR=."
    )
)

REM Check Release folder
if not defined FOUND_DLL (
    if exist "Release\open_overlay_plugin.dll" (
        set "FOUND_DLL=Release\open_overlay_plugin.dll"
        set "OUTPUT_DIR=Release"
    )
)

REM Check Debug folder
if not defined FOUND_DLL (
    if exist "Debug\open_overlay_plugin.dll" (
        set "FOUND_DLL=Debug\open_overlay_plugin.dll"
        set "OUTPUT_DIR=Debug"
    )
)

if defined FOUND_DLL (
    echo Output: %FOUND_DLL%
    echo.
    echo To install in OBS Studio:
    echo   copy "%FOUND_DLL%" "C:\Program Files\obs-studio\obs-plugins\64bit\"
    echo.
    echo Or use: cmake --install . --config Release
) else (
    echo WARNING: Could not find output DLL
    echo.
    echo Looking in build directory...
    dir /s /b *.dll 2>nul
    echo.
    echo Check these locations:
    echo   - build\bin\Release\open_overlay_plugin.dll
    echo   - build\open_overlay_plugin.dll
    echo   - build\Release\open_overlay_plugin.dll
)

echo.
pause