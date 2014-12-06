@echo off
mkdir build
pushd build
cl -Zi /IC:\Users\Russ\src\boilerplate\SDL2-2.0.3\include ../game.cpp C:\Users\Russ\src\boilerplate\SDL2-2.0.3\lib\x64\SDL2.lib user32.lib gdi32.lib
popd
