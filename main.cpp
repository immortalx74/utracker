//=============================================================================================
// -replace active_cell.ROW++. Set current play-row from loop iterator in playpattern/playmodule
//
//=============================================================================================
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

#include <SFML/Window/Cursor.hpp>

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
#include "listbuttons.cpp"

int main()
{
	LoadTextures();
	sf::RenderWindow window(sf::VideoMode(1024, 768), "Tracker alpha", sf::Style::Default);
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);
	ImGuiIO& io = ImGui::GetIO(); (void)io;

    sf::Clock deltaClock;

    //======================================================================
    FMOD::System     *system;
    FMOD::Sound      *sound;
    FMOD::Channel    *channel = 0;
    FMOD::ChannelGroup *channelgroup;
    FMOD_RESULT       result;

    result = FMOD::System_Create(&system);
    ERRCHECK(result);

    // system->setOutput(FMOD_OUTPUTTYPE_ASIO);
    system->setOutput(FMOD_OUTPUTTYPE_WASAPI);

    result = system->init(500, FMOD_INIT_NORMAL, 0);
    ERRCHECK(result);

    result = system->createSound("res/pianoc5.wav", FMOD_SOFTWARE, 0, &sound);
    ERRCHECK(result);

    result = sound->setMode(FMOD_LOOP_OFF);
    ERRCHECK(result);

    result = system->createChannelGroup("mychannels", &channelgroup);
    ERRCHECK(result);

    //======================================================================
    
   // create default tracks
    for (int i = 0; i < tracks; ++i)
    {
        CreateTrack(tracks_list);
    }
	
    CreatePattern(patterns_list, 64, module); // create default PATTERN_
    CreateInstrument(instruments_list); // create default instrument (serves as "no instrument" equivalent of MPT)

    active_cell.X = 306;
    active_cell.Y = 132;
    active_cell.ROW = 0;
    active_cell.COL = 0;
    
	SELECTION selection;
	selection.START_X = 0;
	selection.START_Y = 0;
	selection.END_X = 0;
	selection.END_Y = 0;
	
	float grid_scroll_x = 0.0f;
    float grid_scroll_y = 0.0f;
	
	float mousex = 0;
	float mousey = 0;
	float winx = 0;
	float winy = 0;
	bool key_pressed = false;
	
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) window.close();
		}
		ImGui::SFML::Update(window, deltaClock.restart());
		system->update();
		io.KeyRepeatRate = 0.035f;
		
		ImGui::PushStyleColor(ImGuiCol_Button, col_button);
		
		// Left pane======================================
		#include "leftpane.cpp"
		
		// Draw toolbar==================================
		UI.TOOLBAR_X = UI.LEFT_PANE_X + UI.LEFT_PANE_WIDTH + (2 * UI.MARGIN);
		UI.TOOLBAR_WIDTH = io.DisplaySize.x - UI.LEFT_PANE_X - UI.LEFT_PANE_WIDTH - (3 * UI.MARGIN);
		DrawToolbar(toolbar_buttons, toolbar_tooltips);

		// Main==========================================
		int pattern_start = patterns_list[active_pattern].OFFSET;
		int pattern_rows = patterns_list[active_pattern].ROWS;
		int pattern_end = pattern_start + pattern_rows;

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
		
		//selection==========================================
		#include "selection.cpp"	
		
		//all custom drawing================================
		#include "draw.cpp"
		
		// get keyboard===================================
		#include "keyboard.cpp"

		if (application_state == PLAY_PATTERN)
		{
			active_cell.ROW = 0;
			ImGui::SetScrollY(0);
			application_state = PLAYING;
			future_play = std::async(std::launch::async, PlayPattern, module, system, channel,sound, channelgroup, pattern_start, pattern_end, tracks);
        }

		if (application_state == PLAY_MODULE)
		{
			active_cell.ROW = 0;
			ImGui::SetScrollY(0);
			application_state = PLAYING;
			int end = patterns_list.size();
			future_play = std::async(std::launch::async, PlayModule, module, system, channel,sound, channelgroup, 0, end, tracks);
        }
		
		if (application_state == EDITOR)
		{
			FMOD_RESULT result;
			result = channel->stop();
		}

		if (application_state == PLAYING)
		{
			active_cell.LAST_CURSOR_ACTION = DOWN;
			active_cell.Y = 132 + (active_cell.ROW * UI.CELL_HEIGHT);
			if (active_cell.ROW >= patterns_list[active_pattern].ROWS - 1)
			{
				ImGui::SetScrollY(0);
				active_cell.ROW = 0;
				active_cell.Y = 132;
			}
		}
		
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
		
		//============================================================================================
		window.clear();
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
}