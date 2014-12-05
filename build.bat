@echo off
set INCLUDE=%cd%\SDL2-2.0.3\include
%INCLUDE%
mkdir build
pushd build
cl -Zi ../game.cpp sdl.lib user32.lib gdi32.lib
popd
