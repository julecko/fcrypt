@echo off
setlocal EnableExtensions EnableDelayedExpansion

REM Defaults
set "BUILD_TYPE=Release"
set "RUN_EXEC=0"

:parse_args
if "%~1"=="" goto build

if /I "%~1"=="-d" set "BUILD_TYPE=Debug" & shift & goto parse_args
if /I "%~1"=="-r" set "RUN_EXEC=1"        & shift & goto parse_args

echo Unknown argument: %~1
exit /b 1

:build

set "BUILD_DIR=build"

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

echo.
echo Configuring (%BUILD_TYPE%)...
cmake -S . -B "%BUILD_DIR%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE%

echo.
echo Building...
cmake --build "%BUILD_DIR%" --config %BUILD_TYPE%

if "%RUN_EXEC%"=="1" (
    echo.
    echo Running %BUILD_DIR%\%BUILD_TYPE%\fcrypt.exe
    "%BUILD_DIR%\%BUILD_TYPE%\fcrypt.exe"
)

endlocal
