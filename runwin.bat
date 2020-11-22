@echo off
cd .\bin\
if "%1" == "debug" (
    .\prog-debug.exe
) else (
    .\prog.exe
)