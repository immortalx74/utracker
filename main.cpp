//============================================================================
// - Check if a sample was removed from disk (popup warning? something else?).
// - Calculate correct speed (BPM relationship with rows_per_beat).
// - Possible bug when changing BPM/ticks while on playback. (slows down?)
// - Implement move up/move down for patterns/instruments.
//   What should happen when moving/deleting patterns/instruments?
// - Bug when playing module. Notes should stop just like when playing
//   a single pattern.
// - Add a "pan" field to notes and include it on PlayRow. It will be set by an FX
// - Prevent sliders (and possibly other ui elements) of having values <>
//   min/max.NOTE:This is an imgui issue.
// - Implement saving/loading application settings. Suggested fields:
//   Default new pattern rows/track count/BPM/Ticks/Middle octave.
//   Window state (pos/size/maximized). Color scheme. Keyb shortcuts.
// - Read/Write file format.
// - WAV export.
// - Right click context menu: Shift note(s) up/down by 1 or by octave,
//   [selection,current track, current pattern, whole module?]
//   Set volume [same as above], Set instrument[same as above]
// - BUG: On module playback mode, stop goes to next pattern. Should stay
//   on same pattern and cursor should be set to row zero.
// - Fix Solo/Mute. Should be updated even after firing notes.
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

#include "portable-file-dialogs.h"
#include "iniparser.hpp"

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

// imx family's program restructuring MODEL (I.F.P.R.M.)
//struct {
//public:
//int get_myx()
//{
//std::cout << "get_myx\n";
//return this->myx;
//}
//private:
//int myx = 1500;
//} MY_GLOBS;
//

int main()
{
    int ival;
	std::string sval;
	// Creating ini file object
	INI::File ft;
	// Loading from file
	if (!ft.Load("settings.ini"))
	{
		print("hey");
	}
    
	ft.GetSection("Main")->SetValue("value1",20);
    ft.GetSection("Main")->SetValue("value2","HeyDude");
    
    ft.GetSection("Sound")->SetValue("volume",10);
    ival = ft.GetValue("Sound:volume",3).AsInt();
    print(ival);
    
	// get "value1" from section "Main". If there is no such entry return default value 1
	//ival = ft.GetSection("Main")->GetValue("value1",1).AsInt();
	// get "value2" from section "Main", defaulting to 2
	//ival = ft.GetValue("Main:value2",2).AsInt();
	// set "value1" in subsection "subsect" of section "Main"
	//ft.SetValue("Main.subsect:value1",345.5);
	// get "value1" from subsection "subsect" of section "Main"
	//dval = ft.GetSection("Main")->GetSubSection("subsect")->GetValue("value1").AsDouble();
	// set 4th element of array "array" in section "Main" to 32.1
	//ft.GetSection("Main")->SetArrayValue("array",4,32.1);
	// get 4th element of array "array" in section "Main"
	//dval = ft.GetSection("Main")->GetValue("array").AsArray()[4].AsDouble();
    
    
    ft.Save("settings.ini");
    
    
    
	sf::RenderWindow window(sf::VideoMode(1024, 920), "Tracker alpha", sf::Style::Default);
	window.setPosition(ImVec2(300,0));
    window.setFramerateLimit(60);
	ImGui::SFML::Init(window);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
    sf::Clock deltaClock;
    
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
        
#include "leftpane.cpp"
        
		// Draw toolbar==================================
		UI.TOOLBAR_X = UI.LEFT_PANE_X + UI.LEFT_PANE_WIDTH + (2 * UI.MARGIN);
		UI.TOOLBAR_WIDTH = io.DisplaySize.x - UI.LEFT_PANE_X - UI.LEFT_PANE_WIDTH - (3 * UI.MARGIN);
		
        DrawToolbar(toolbar_buttons, toolbar_tooltips);
        
        //bool pp_opened = true;
        if (is_settings_open)
        {
            ImGui::OpenPopup("Settings");
        }
#include "settings.cpp"
        
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
        
		if (UI.TOOLBAR_HEIGHT + (patterns_list[active_pattern].ROWS * UI.CELL_HEIGHT) + UI.MAIN_PADDING + UI.CELL_HEIGHT + UI.MARGIN > io.DisplaySize.y)
		{
			UI.MAIN_HEIGHT = io.DisplaySize.y - UI.TOOLBAR_HEIGHT - UI.MARGIN;
            
		}
		else
		{
			UI.MAIN_HEIGHT = (patterns_list[active_pattern].ROWS * UI.CELL_HEIGHT) + UI.MAIN_PADDING + UI.CELL_HEIGHT + UI.MARGIN;
		}
        
		ImGui::SetNextWindowSize(ImVec2(UI.MAIN_WIDTH, UI.MAIN_HEIGHT));
        
		ImGui::Begin("main", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        
#include "drawheaders.cpp"
#include "mouse.cpp"
#include "selection.cpp"	
#include "draw.cpp"
#include "keyboard.cpp"
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