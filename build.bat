@echo off

set path=C:\dev\utracker;%path%

set SOURCES=imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp imgui/imgui-SFML.cpp imgui/imgui_demo.cpp main.cpp

REM DEBUG
start /wait /B cl /MP /Fdbuild\ /Febuild/utracker /Fobuild\  /EHsc /Zi /FC /nologo /W1 /Od /Iinclude /Iimgui %SOURCES% /link /SUBSYSTEM:CONSOLE lib\sfml-system.lib lib\sfml-graphics.lib lib\sfml-main.lib lib\sfml-window.lib opengl32.lib winmm.lib gdi32.lib lib\freetype.lib lib\fmodL_vc.lib

REM RELEASE
REM start /wait /B cl /MD /Fdbuild\ /Febuild/utracker /Fobuild\  /EHsc /FC /nologo /W1 /O2 /Iinclude /Iimgui %SOURCES% /link /SUBSYSTEM:WINDOWS lib\sfml-system.lib lib\sfml-graphics.lib lib\sfml-main.lib lib\sfml-window.lib opengl32.lib winmm.lib gdi32.lib lib\freetype.lib lib\fmod_vc.lib

start /wait build/utracker.exe