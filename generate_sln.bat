@echo off
if not "%1" == "build" (
    if not "%1" == "clang" (
        cmake project -Bbuild
    ) else (
        cmake project -Bbuild-clang -T "ClangCL"
    )
)

if not errorlevel == 1 (
    if "%1" == "open" (
        for /f "delims=" %%i in ('dir "build\\*.sln" /B') do start build\\%%i
    ) else if "%2" == "open" (
        for /f "delims=" %%i in ('dir "build-clang\\*.sln" /B') do start build-clang\\%%i
    ) else if "%1" == "build" (
        cmake --build %*
    )
)

