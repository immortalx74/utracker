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
#include <fstream>
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
#include "fileio.cpp"
#include "playback.cpp"
#include "toolbar.cpp"
#include "ini_settings.cpp"

#pragma comment (lib, "Comdlg32")
#pragma comment (lib, "OLE32")
#pragma comment (lib, "shell32")

int main()
{
    unsigned int m_width = sf::VideoMode::getDesktopMode().width;
    unsigned int m_height = sf::VideoMode::getDesktopMode().height;
    
    bool close_window = false;
    bool open_close_window_modal = false;
    
    sf::RenderWindow window(sf::VideoMode(window_metrics.WIDTH, window_metrics.HEIGHT), "uTracker v0.1", sf::Style::Default);
    window.setFramerateLimit(30);
	ImGui::SFML::Init(window);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
    sf::Clock deltaClock;
    
#include "init.cpp"
	window.setPosition(ImVec2(window_metrics.X, window_metrics.Y));
    window.setSize(ImVec2(window_metrics.WIDTH, window_metrics.HEIGHT));
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
            {
                open_close_window_modal = true;
            }
		}
        
        if (close_window)
        {
            window.close();
        }
        
        ImGui::SFML::Update(window, deltaClock.restart());
		fsystem->update();
		io.KeyRepeatRate = 0.035f;
        
        if (open_close_window_modal)
        {
            ImGui::OpenPopup("Exit uTracker");
        }
        
        ImGui::PushStyleColor(ImGuiCol_PopupBg, color_schemes[active_color_scheme].DATA[WindowBackground].COLOR_VALUE);
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, color_schemes[active_color_scheme].DATA[PanelBackground].COLOR_VALUE);
        ImGui::PushStyleColor(ImGuiCol_Button, color_schemes[active_color_scheme].DATA[Buttons].COLOR_VALUE);
        ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);
        if (ImGui::BeginPopupModal("Exit uTracker", NULL, ImGuiWindowFlags_NoResize))
        {
            ImGui::Text("All unsaved data will be lost\n\nAre you sure?");
            
            if (ImGui::Button("OK", ImVec2(120, 0)))
            {
                close_window = true;
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::SameLine();
            
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                open_close_window_modal = false;
                close_window = false;
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndPopup();
        }
        ImGui::PopStyleColor(4);
        
		ImGui::PushStyleColor(ImGuiCol_Button, color_schemes[active_color_scheme].DATA[Buttons].COLOR_VALUE);
        
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
        
		if (UI.LEFT_PANE_WIDTH + (tracks_list.size() * UI.TRACK_WIDTH) + UI.MAIN_PADDING > io.DisplaySize.x)
		{
			UI.MAIN_WIDTH = io.DisplaySize.x - UI.LEFT_PANE_WIDTH - UI.MARGIN;
		}
		else
		{
			UI.MAIN_WIDTH = (tracks_list.size() * UI.TRACK_WIDTH) + UI.MAIN_PADDING;
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
        ImGui::SetNextWindowPos(ImVec2(UI.LEFT_PANE_WIDTH + (2 * UI.MARGIN),
                                       UI.TOOLBAR_HEIGHT + UI.MARGIN));
		
        ImGui::PushStyleColor(ImGuiCol_WindowBg, color_schemes[active_color_scheme].DATA[PanelBackground].COLOR_VALUE);
        ImGui::Begin("main", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        
#include "drawheaders.cpp"
#include "mouse.cpp"
#include "selection.cpp"	
#include "draw.cpp"
#include "keyboard.cpp"
#include "appstate.cpp"
        
		ImGui::EndChild();
        
        // Right click context menu
#include "right_click_context_menu.cpp"
        
		ImGui::PopStyleColor();
        ImGui::End();
        ImGui::PopStyleColor();
        
        // demo window
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_PageDown)))
		{
			show_demo = !show_demo;
		}
		
		if (show_demo)
		{
			ImGui::ShowDemoWindow();
            //ImGui::ShowStyleEditor();
		}
        
		//======================================================
		window.clear(color_schemes[active_color_scheme].DATA[WindowBackground].COLOR_VALUE);
		ImGui::SFML::Render(window);
		window.display();
	}
    
    // shut down
    result = fsystem->release();
    ERRCHECK(result);
    
    while (1)
    {
        if (IniSaveWindow(window))
        {
            break;
        }
    }
    
	ImGui::SFML::Shutdown();
    
    //std::cin.get();
}