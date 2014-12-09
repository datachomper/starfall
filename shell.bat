@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86
set "PATH=%cd%\lib\SDL2\x86\;%PATH%"
set "LIB=%cd%\lib\SDL2\x86\;%LIB%"
set "INCLUDE=%cd%\include\SDL2\;%INCLUDE%"
