::vcpkg hash https://github.com/<user>/<repo>/archive/refs/tags/<tag>.tar.gz SHA512
::vcpkg hash https://github.com/<user>/<repo>/archive/<tag>.tar.gz SHA512

@echo off
setlocal enabledelayedexpansion

set TAG=%~1
set URL=https://github.com/chenjunfu2/nbt-cpp/archive/%TAG%.tar.gz
set FILE=%TEMP%\nbt-cpp-%TAG%.tar.gz

echo Downloading: %URL%
echo To: %FILE%
echo.
curl -L -o "%FILE%" "%URL%" --proxy http://127.0.0.1:7897
if errorlevel 1 (
    echo Download Error
    echo.
    pause
    exit /b 1
)

echo.
echo hash:
vcpkg hash "%FILE%" SHA512

del "%FILE%"
if errorlevel 1 (
    echo "%FILE%"
    echo.
    pause
    exit /b 1
)

echo.
pause

endlocal