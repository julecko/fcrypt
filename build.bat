@echo off
setlocal EnableExtensions EnableDelayedExpansion

REM Defaults
set "BUILD_TYPE=Release"
set "RUN_EXEC=0"
set "RUN_ARGS="

:parse_args
if "%~1"=="" goto build

if /I "%~1"=="-d" (
    set "BUILD_TYPE=Debug"
    shift
    goto parse_args
)

if /I "%~1"=="-r" (
    set "RUN_EXEC=1"
    shift
    goto collect_run_args
)

if not "%~1"=="" (
    echo Unknown argument: %~1
    exit /b 1
)


:collect_run_args

if "%~1"=="" goto build

if defined RUN_ARGS (
    set "RUN_ARGS=!RUN_ARGS! %~1"
) else (
    set "RUN_ARGS=%~1"
)
shift
goto collect_run_args


:build

if not defined VCPKG_ROOT (
    echo ERROR: VCPKG_ROOT is not set
    exit /b 1
)

set "BUILD_DIR=build"

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
echo.
echo Configuring (%BUILD_TYPE%)...
cmake -S . -B "%BUILD_DIR%" ^
  -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
  -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake"

echo.
echo Building...
cmake --build "%BUILD_DIR%" --config %BUILD_TYPE%

if "%RUN_EXEC%"=="1" (
    echo.
    echo Running %BUILD_DIR%\%BUILD_TYPE%\fcrypt.exe %RUN_ARGS%
    "%BUILD_DIR%\%BUILD_TYPE%\fcrypt.exe" %RUN_ARGS%
)

endlocal
