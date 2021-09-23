@echo off
mkdir build
copy .\lib\*.dll .\build\
pushd build
cl /I \INCLUDE /I..\inc\ /Zi ..\src\main.c ..\src\game.c ..\src\highscore.c Shell32.lib ..\lib\SDL2.lib ..\lib\SDL2main.lib ..\lib\glew32.lib ..\lib\SDL2_ttf.lib /link /subsystem:CONSOLE
popd
