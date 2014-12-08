@echo off
cl -Zi -Iinclude game.cpp SDL2.lib SDL2_ttf.lib SDL2_mixer.lib user32.lib gdi32.lib
game.exe
