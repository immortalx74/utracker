@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64
set path=C:\dev\tracker;%path%

set SOURCES=imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp imgui/imgui-SFML.cpp imgui/imgui_demo.cpp main.cpp

start /wait /B cl /Fdbuild\ /Febuild/tracker /Fobuild\  /EHsc /Zi /FC /nologo /W1 /Od /Iinclude /Iimgui %SOURCES% /link lib\sfml-system.lib lib\sfml-graphics.lib lib\sfml-main.lib lib\sfml-window.lib opengl32.lib winmm.lib gdi32.lib lib\freetype.lib lib\fmodexL64_vc.lib

start /wait build/tracker.exe