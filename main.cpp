//=================================================
// -replace active_cell.ROW++. Set current play-row from loop iterator in playpattern/playmodule
// apply step doesn't repeat
//=================================================
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"

#include <chrono>
#include <cmath>
#include "imgui.h"
#include "imgui-SFML.h"

#include "imgui_internal.h"
#include "portable-file-dialogs.h"

#include <string>
#include <iostream>
#include <array>
#include <Windows.h>
#include <future>
#include "print.cpp"
#include "misc.cpp"
#include "helpers.cpp"
#include "playback.cpp"
#include "toolbar.cpp"

#pragma comment (lib, "Comdlg32")
#pragma comment (lib, "OLE32")
#pragma comment (lib, "shell32")

int main()
{
	LoadTextures();
	sf::RenderWindow window(sf::VideoMode(1024, 880), "Tracker alpha", sf::Style::Default);
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    sf::Clock deltaClock;
    
    //Init=================================================
#include "init.cpp"
	
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) window.close();
            // if (event.type == sf::Event::KeyPressed) key_pressed = true;
            // if (event.type == sf::Event::KeyReleased) key_pressed = false;
		}
		ImGui::SFML::Update(window, deltaClock.restart());
		fsystem->update();
		io.KeyRepeatRate = 0.035f;
        
		// TEMP!!! separator colors
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Separator]              = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_SeparatorHovered]       = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_SeparatorActive]        = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        
		ImGui::PushStyleColor(ImGuiCol_Button, col_button);
        
		int pattern_start = patterns_list[active_pattern].OFFSET;
		int pattern_rows = patterns_list[active_pattern].ROWS;
		int pattern_end = pattern_start + pattern_rows;
		// Left pane======================================
#include "leftpane.cpp"
        
		// Draw toolbar==================================
		UI.TOOLBAR_X = UI.LEFT_PANE_X + UI.LEFT_PANE_WIDTH + (2 * UI.MARGIN);
		UI.TOOLBAR_WIDTH = io.DisplaySize.x - UI.LEFT_PANE_X - UI.LEFT_PANE_WIDTH - (3 * UI.MARGIN);
		DrawToolbar(toolbar_buttons, toolbar_tooltips);
        
		// Main==========================================
		// int pattern_start = patterns_list[active_pattern].OFFSET;
		// int pattern_rows = patterns_list[active_pattern].ROWS;
		// int pattern_end = pattern_start + pattern_rows;
        
		UI.MAIN_X = ImGui::GetCursorPosX();
		UI.MAIN_Y = ImGui::GetCursorPosY();
        
		if (UI.LEFT_PANE_WIDTH + (tracks * UI.TRACK_WIDTH) + UI.MAIN_PADDING > io.DisplaySize.x)
		{
			UI.MAIN_WIDTH = io.DisplaySize.x - UI.LEFT_PANE_WIDTH - UI.MARGIN;
		}
		else
		{
			UI.MAIN_WIDTH = (tracks * UI.TRACK_WIDTH) + UI.MAIN_PADDING;
		}
        
		if (UI.TOOLBAR_HEIGHT + (pattern_rows * UI.CELL_HEIGHT) + UI.MAIN_PADDING + UI.CELL_HEIGHT + UI.MARGIN > io.DisplaySize.y)
		{
			UI.MAIN_HEIGHT = io.DisplaySize.y - UI.TOOLBAR_HEIGHT - UI.MARGIN;
            
		}
		else
		{
			UI.MAIN_HEIGHT = (pattern_rows * UI.CELL_HEIGHT) + UI.MAIN_PADDING + UI.CELL_HEIGHT + UI.MARGIN;
		}
        
		ImGui::SetNextWindowSize(ImVec2(UI.MAIN_WIDTH, UI.MAIN_HEIGHT));
        
		ImGui::Begin("main", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        
		//draw headers=======================================
#include "drawheaders.cpp"
        
		//get mouse=========================================
#include "mouse.cpp"
		
		//selection=========================================
#include "selection.cpp"	
		
		//all custom drawing================================
#include "draw.cpp"
		
		// get keyboard=====================================
#include "keyboard.cpp"
        
		// application state================================
#include "appstate.cpp"
        
		ImGui::EndChild();
		ImGui::End();
        
		// demo window
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_PageDown)))
		{
			show_demo = !show_demo;
		}
		
		if (show_demo)
		{
			ImGui::ShowDemoWindow();
		}
		
		//======================================================
		window.clear();
		ImGui::SFML::Render(window);
		window.display();
	}
    
    // shut down
    result = sound->release();
    ERRCHECK(result);
    result = fsystem->close();
    ERRCHECK(result);
    result = fsystem->release();
    ERRCHECK(result);
    
	ImGui::SFML::Shutdown();
}