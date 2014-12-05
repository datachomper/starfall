@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x64
set "PATH=%cd%\SDL2-2.0.3\lib\x64\;%cd%\SDL2-2.0.3\include\;%PATH%"
