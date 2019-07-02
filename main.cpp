//============================================================================
// - Selection goes out of bounds on up and right.
// - Prevent loading the same sample again (Or not? Does it matter?).
// - Check if a sample was removed from disk (popup warning? something else?).
// - Fix changing the name of patterns/instruments. When entering options,
//   it should get the current name of the current pattern/instrument and
//   allow changing.
// - Calculate the volume chain (global->track->note)
// - Calculate correct speed (BPM relationship with ticks_per_row).
// - Possible bug when changing BPM/ticks while on playback. (slows down?)
// - Implement delete/move up/move down for patterns/instruments.
//   What should happen when moving/deleting patterns/instruments?
//   For instruments, should the notes switch to next instrument?
//   What if the instrument with higher index gets removed? 
//   For patterns, there's a lot of data to be moved around.
// - Implement cut/copy/paste. Cursor is always top-left of selection,
//   handle both toolbar and context menu.
// - Bug when playing module. Notes should stop just like when playing
//   a single pattern.
// - Set track sliders (vol + pan)to actually do something.
// - Add a "pan" field to NOTE_DATA struct. Also add a global pan and set
//   notes to play with pan chain:global pan->note pan.
// - Prevent sliders (and possibly other ui elements) of having values <>
//   min/max.NOTE:This is an imgui issue.
// - Implement saving/loading application settings. Suggested fields:
//   Default new pattern rows/track count/BPM/Ticks/Middle octave.
//   Window state (pos/size/maximized). Color scheme
// - Read/Write file format.
// - WAV export.
// - Right click context menu: Shift note(s) up/down by 1 or by octave,
//   [selection,current track, current pattern, whole module?]
//   Set volume [same as above], Set instrument[same as above]
//============================================================================
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "fmod/fmod.hpp"
#include "fmod/fmod_common.h"
#include "fmod/fmod_errors.h"

#include <chrono>
#include <cmath>
#include "imgui.h"
#include "imgui-SFML.h"

//#include "imgui_internal.h"
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
		}
        
		ImGui::SFML::Update(window, deltaClock.restart());
		fsystem->update();
		io.KeyRepeatRate = 0.035f;
        
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
    result = fsystem->close();
    ERRCHECK(result);
    result = fsystem->release();
    ERRCHECK(result);
    
	ImGui::SFML::Shutdown();
    
    //std::cin.get();
}