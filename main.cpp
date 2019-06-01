#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"

#include "imgui.h"
#include "imgui-SFML.h"
#include <string>
#include <iostream>
#include <array>
#include "misc.cpp"
bool play = false;

void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

int main()
{
	load_textures();
	sf::RenderWindow window(sf::VideoMode(1024, 768), "Tracker alpha", sf::Style::Default);
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);
	ImGuiIO& io = ImGui::GetIO(); (void)io;

    sf::Clock deltaClock;

    //======================================================================
    FMOD::System     *system;
    FMOD::Sound      *sound1, *sound2, *sound3;
    FMOD::Channel    *channel = 0;
    FMOD_RESULT       result;
    int               key;
    unsigned int      version;

    result = FMOD::System_Create(&system);
    ERRCHECK(result);

    result = system->init(32, FMOD_INIT_NORMAL, 0);
    ERRCHECK(result);

    result = system->createSound("drumloop.wav", FMOD_HARDWARE, 0, &sound1);
    ERRCHECK(result);


    //======================================================================
	
    // create containers
    std::vector<PATTERN> patterns_list;
    std::vector<INSTRUMENT> instruments_list;
    std::vector<TRACK> tracks_list;
    
    for (int i = 0; i < tracks; ++i)
    {
        create_track(tracks_list);
    }
	
    std::vector<std::vector<NOTE_DATA>> module;
	
    create_pattern(patterns_list, 64, module); // create default pattern
	
    ACTIVE_CELL active_cell;
    active_cell.X = cell_width;
    active_cell.Y = 131;
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
	
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) window.close();
		}

		ImGui::SFML::Update(window, deltaClock.restart());
		
		//============================================================================================
		io.KeyRepeatRate = 0.035f;
		
		ImGui::PushStyleColor(ImGuiCol_Button, col_button);
		
		// Left pane
		int app_window_height = io.DisplaySize.y;
		ImGui::SetNextWindowSize(ImVec2(260, app_window_height));
		ImGui::SetNextWindowPos(ImVec2(4, 0));
		ImGui::Begin("LeftPane", false,ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		
		// Patterns list
		ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
		ImGui::Text("Patterns");
		ImGui::PopStyleColor();
		ImGui::PushItemWidth(200);
		
		if (ImGui::ListBoxHeader("##patlist", 20, 10)) // label, items_count, height_in_items
		{   
			for (int i = 0; i < patterns_list.size(); ++i)
			{
				if (ImGui::Selectable(patterns_list[i].NAME.c_str(), active_pattern == i))
				{
					active_pattern = i;
				}
			}
		}
		ImGui::ListBoxFooter();
		ImGui::PopItemWidth();
		
		// Instruments list 
		ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
		ImGui::Text("Instruments");
		ImGui::PopStyleColor();    
		ImGui::PushItemWidth(200);
		
		if (ImGui::ListBoxHeader("##inslist", 20, 10)) // label, items_count, height_in_items
		{   
			for (int i = 0; i < instruments_list.size(); ++i)
			{
				if (ImGui::Selectable(instruments_list[i].NAME.c_str(), active_instrument == i))
				{
					active_instrument = i;
				}
			}
		}
		ImGui::ListBoxFooter();
		ImGui::PopItemWidth();
		
		// Draw sliders
		ImGui::PushItemWidth(100);
		ImGui::SetCursorPosY(428);
		
		ImGui::SliderInt("Octave", &octave, 0, 9);
		ImGui::SliderInt("Bpm", &bpm, 32, 512);
		ImGui::SliderInt("Ticks/Row", &ticks_per_row, 1, 32);
		ImGui::SliderInt("Step", &step, 0, 32);
		
		ImGui::PopItemWidth();
		
		// // show mouse coords
		ImGui::Text("mouse_x:");ImGui::SameLine();ImGui::Text(std::to_string(ImGui::GetMousePos().x).c_str());
		ImGui::Text("mouse_y:");ImGui::SameLine();ImGui::Text(std::to_string(ImGui::GetMousePos().y).c_str());
		
		// Draw Pattern buttons
		ImGui::SetCursorPos(ImVec2(220, 27));
		if (ImGui::Button("+##pattern_add"))
		{
			create_pattern(patterns_list, 64, module);
		}
		
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Add Pattern");
		}
		
		
		ImGui::SetCursorPos(ImVec2(220, 47));
		if (ImGui::Button("-##pattern_del"))
		{
			//
		}
		
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Delete Pattern");
		}
		
		ImGui::SetCursorPos(ImVec2(220, 67));
		if (ImGui::Button("^##pattern_up"))
		{
			//
		}
		
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Move pattern up");
		}
		
		ImGui::SetCursorPos(ImVec2(220, 87));
		if (ImGui::Button("v##pattern_down"))
		{
			//
		}
		
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Move pattern down");
		}
		
		ImGui::SetCursorPos(ImVec2(220, 107));
		if (ImGui::Button("o##pattern_edit"))
		{
			ImGui::OpenPopup("Pattern options");
		}
		
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Open pattern options");
		}
		
		ImGui::SetNextWindowSize(ImVec2(400, 200));
		
		bool p_opened = true;
		
		if (ImGui::BeginPopupModal("Pattern options", &p_opened, ImGuiWindowFlags_NoResize))
		{
			ImGui::EndPopup();
		}
		
		
		// Draw Instrument buttons
		ImGui::SetCursorPos(ImVec2(220, 246));
		if (ImGui::Button("+##instrument_add"))
		{
			create_instrument(instruments_list);
		}
		
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Add Instrument");
		}
		
		ImGui::SetCursorPos(ImVec2(220, 266));
		if (ImGui::Button("-##instrument_del"))
		{
			//
		}
		
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Delete Instrument");
		}
		
		ImGui::SetCursorPos(ImVec2(220, 286));
		if (ImGui::Button("^##instrument_up"))
		{
			//
		}
		
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Move instrument up");
		}
		
		ImGui::SetCursorPos(ImVec2(220, cell_width));
		if (ImGui::Button("v##instrument_down"))
		{
			//
		}
		
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Move instrument down");
		}
		
		ImGui::SetCursorPos(ImVec2(220, 326));
		if (ImGui::Button("o##instrument_edit"))
		{
			ImGui::OpenPopup("Instrument options");
		}
		
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Open instrument options");
		}
		
		ImGui::SetNextWindowSize(ImVec2(400, 200));
		
		if (ImGui::BeginPopupModal("Instrument options", &p_opened, ImGuiWindowFlags_NoResize))
		{
			ImGui::EndPopup();
		}
		
		ImGui::End();
		
		// Draw toolbar
		float cur_pos_x = ImGui::GetCursorPosX();
		int app_window_width = io.DisplaySize.x;
		
		ImGui::SetNextWindowSize(ImVec2(app_window_width - cur_pos_x - 264, 48));
		ImGui::SetNextWindowPos(ImVec2(268, 0));
		ImGui::Begin("toolbar", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		
		for (int i = 0; i < 13; ++i)
		{
			if (btn_repeat && i == 8)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, col_btn_repeat);
				btn_repeat_changed = !btn_repeat_changed;
			}
			
			if (i == 4 || i == 9 || i == 12)
			{
				ImGui::Image(toolbar_buttons[13], ImVec2(24, 24)); // separator
				ImGui::SameLine();
			}
			
			if (ImGui::ImageButton(toolbar_buttons[i], ImVec2(24, 24)))
			{
				if (i == 8) // repeat button toggle
				{
					btn_repeat = !btn_repeat;
				}
			}
			
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip(toolbar_tooltips[i].c_str());
			}
			
			if (btn_repeat_changed)
			{
				btn_repeat_changed = !btn_repeat_changed;
				ImGui::PopStyleColor();
			}
			ImGui::SameLine();
		}
		
		ImGui::End();
		
		ImGui::PopStyleColor();
		
		// main
		float cursor_pos_x = ImGui::GetCursorPosX();
		float cursor_pos_y = ImGui::GetCursorPosY();

		ImVec2 cursor_screen_pos = ImGui::GetCursorScreenPos();
		
		int pattern_start = patterns_list[active_pattern].OFFSET;
		int pattern_rows = patterns_list[active_pattern].ROWS;
		int pattern_end = pattern_start + pattern_rows;
		
		if (cursor_screen_pos.x + (tracks * 120) < app_window_width - cursor_pos_x - 264)
		{
			ImGui::SetNextWindowSize(ImVec2(cursor_screen_pos.x + (tracks * 120) + 108, app_window_height - cursor_pos_y - 26));
		}
		else
		{
			ImGui::SetNextWindowSize(ImVec2(app_window_width - cursor_pos_x - 264, app_window_height - cursor_pos_y - 26));
		}

		ImGui::Begin("main", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		
		//=========================================
		
		// draw track headers
		ImGui::SetNextWindowContentSize(ImVec2((tracks * 120) + 108, cell_height));
		ImGui::SetCursorPosX(46);
		ImGui::BeginChild("##trackheaders", ImVec2(0, 60), false);
		ImGui::SetScrollX(grid_scroll_x);
		ImGui::Columns(tracks);
		
		for (int t = 0; t < tracks; ++t)
		{
			std::string track_text = "    Track:";
			track_text += std::to_string(t + 1);
			ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
			ImGui::Text(track_text.c_str());
			ImGui::PopStyleColor();
			ImGui::PushItemWidth(104);
			ImGui::PushID(std::to_string(t).c_str());
			ImGui::SliderInt("##trackvolume", &tracks_list[t].VOLUME, 0, 64, "v:%02d");
			if (ImGui::IsItemClicked(1))
			{
				tracks_list[t].VOLUME = 64;
			}
			ImGui::PopID();
			ImGui::PushID(std::to_string(t).c_str());
			ImGui::SliderFloat("##trackpan", &tracks_list[t].PAN, -1, 1, "p:%+.2f");
			if (ImGui::IsItemClicked(1))
			{
				tracks_list[t].PAN = 0;
			}
			ImGui::PopID();
			ImGui::PopItemWidth();
			ImGui::SetColumnWidth(-1, 120);
			ImGui::NextColumn();
		}
		
		ImGui::EndChild();
		ImGui::Columns(1);
		
		// draw row headers
		ImGui::SetNextWindowContentSize(ImVec2(cell_width, (pattern_rows * cell_height) + cell_width));
		ImGui::BeginChild("##rowheaders", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);
		ImGui::SetScrollY(grid_scroll_y);
		ImGui::SetCursorPosY(8);
		for (int p = 0; p < pattern_rows; ++p)
		{
			std::string row_text = std::to_string(p);
			std::string row_text_zeros = std::string(3 - row_text.length(), '0') + row_text;
			ImGui::Text(row_text_zeros.c_str());
		}
		ImGui::EndChild();
		
		ImGui::SetCursorPosY(72);
		ImGui::SetCursorPosX(38);
		ImGui::SetNextWindowContentSize(ImVec2(tracks * 120, pattern_rows * cell_height));
		ImGui::BeginChild("##scrollinggrid", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
		
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
		{
			mousex = ImGui::GetMousePos().x + ImGui::GetScrollX();
			mousey = ImGui::GetMousePos().y + ImGui::GetScrollY();
			winx = ImGui::GetWindowPos().x;
			winy = ImGui::GetWindowPos().y+8;
			
			if (ImGui::IsMouseClicked(0))
			{
				active_cell.COL = floor((mousex - winx)) / cell_width;
				active_cell.ROW = floor((mousey - winy)) / cell_height;
				active_cell.X = active_cell.COL * cell_width + winx;
				active_cell.Y = active_cell.ROW * cell_height + winy;
				if (selection_exists)
				{
					selection_exists = false;
				}
			}
		}
		
		// begin selection
		if (ImGui::IsMouseDragging(0) && ImGui::IsWindowHovered())
		{
			// determine direction: right-down, right-up, left-down, left-up
			if (mousex > active_cell.X && mousey > active_cell.Y)
			{
				selection.START_X = active_cell.X;
				selection.START_Y = active_cell.Y;
				selection.END_X = (floor((mousex - winx) / cell_width) + 1) * cell_width + winx;
				selection.END_Y = (floor((mousey - winy) / cell_height) + 1) * cell_height + winy;
			}
			else if (mousex > active_cell.X && mousey < active_cell.Y)
			{
				selection.START_X = active_cell.X;
				selection.START_Y = active_cell.Y + cell_height;
				selection.END_X = (floor((mousex - winx) / cell_width) + 1) * cell_width + winx;
				selection.END_Y = (floor((mousey - winy) / cell_height)) * cell_height + winy;
			}
			else if (mousex < active_cell.X && mousey > active_cell.Y)
			{
				selection.START_X = (floor((mousex - winx) / cell_width)) * cell_width + winx;
				selection.START_Y = active_cell.Y;
				selection.END_X = active_cell.X + cell_width;
				selection.END_Y = (floor((mousey - winy) / cell_height) + 1) * cell_height + winy;
			}
			else if (mousex < active_cell.X && mousey < active_cell.Y)
			{
				selection.START_X = (floor((mousex - winx) / cell_width)) * cell_width + winx;
				selection.START_Y = (floor((mousey - winy) / cell_height)) * cell_height + winy;
				selection.END_X = active_cell.X + cell_width;
				selection.END_Y = active_cell.Y + cell_height;
			}

			// auto scroll if selection out of bounds
			float sy = ImGui::GetScrollY();
			float sx = ImGui::GetScrollX();

			if (ImGui::GetMousePos().y > ImGui::GetWindowHeight() + ImGui::GetWindowPos().y - 40)
			{
				sy += cell_height;
				ImGui::SetScrollY(sy);
			}

			if (ImGui::GetMousePos().x > ImGui::GetWindowWidth() + ImGui::GetWindowPos().x - 40)
			{
				sx += cell_height;
				ImGui::SetScrollX(sx);
			}

			if (ImGui::GetMousePos().y < ImGui::GetWindowPos().y + 40)
			{
				sy -= cell_height;
				ImGui::SetScrollY(sy);
			}

			if (ImGui::GetMousePos().x < ImGui::GetWindowPos().x + 40)
			{
				sx -= cell_height;
				ImGui::SetScrollX(sx);
			}
			
			selection_begin = ! selection_begin;
			
			if (selection.START_X != selection.END_X || selection.START_Y != selection.END_Y)
			{
				selection_exists = true;
			}
			
			if (ImGui::IsMouseReleased(0) && selection_begin)
			{
				selection_begin = !selection_begin;
			}
		}
		
		
		grid_scroll_x = ImGui::GetScrollX(); // update scroll_x to sync with track headers scrolling
		grid_scroll_y = ImGui::GetScrollY(); // update scroll_y to sync with row headers scrolling
		
		// draw nth row highlights
		ImVec2 c = ImGui::GetCursorScreenPos();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		
		for (int r = 0; r < pattern_rows; r += nth_row_highlight)
		{
			draw_list->AddRectFilled(ImVec2(c.x, c.y), ImVec2(c.x + (tracks * 120) - 8, c.y + cell_height), col_nth_row_highlight);
			c.y += nth_row_highlight * cell_height;
		}
		
		// draw active row
		draw_list->AddRectFilled(ImVec2(c.x, active_cell.Y - ImGui::GetScrollY()),
								 ImVec2(c.x + (tracks * 120) - 8, active_cell.Y + cell_height - ImGui::GetScrollY()), col_active_row);
		
		// draw active cell
		draw_list->AddRectFilled(ImVec2(active_cell.X - ImGui::GetScrollX(), active_cell.Y - ImGui::GetScrollY()),
								 ImVec2(active_cell.X + cell_width - ImGui::GetScrollX(), active_cell.Y + cell_height - ImGui::GetScrollY()), col_active_cell);
		
		//TEMP!!!!! scroll cursor out of bounds test
		ImVec2 rect_start, rect_end;
		rect_start.x = active_cell.X - ImGui::GetScrollX(); rect_start.y = active_cell.Y - ImGui::GetScrollY();
		rect_end.x = active_cell.X + cell_width - ImGui::GetScrollX(); rect_end.y = active_cell.Y + cell_height - ImGui::GetScrollY();
		if (!ImGui::IsRectVisible(rect_start,rect_end))
		{
			float sx = ImGui::GetScrollX();
			float sy = ImGui::GetScrollY();
			if (active_cell.LAST_CURSOR_ACTION == DOWN)
			{
				sy += cell_height;
				ImGui::SetScrollY(sy);
			}
			else if (active_cell.LAST_CURSOR_ACTION == UP)
			{
				sy -= cell_height;
				ImGui::SetScrollY(sy);
			}
			else if (active_cell.LAST_CURSOR_ACTION == RIGHT)
			{
				sx += cell_width;
				ImGui::SetScrollX(sx);
			}
			else if (active_cell.LAST_CURSOR_ACTION == LEFT)
			{
				sx -= cell_width;
				ImGui::SetScrollX(sx);
			}
			
		}
		
		// draw selection
		if (selection_exists)
		{
			draw_list->AddRectFilled(ImVec2(selection.START_X - ImGui::GetScrollX(), selection.START_Y - ImGui::GetScrollY()),
									 ImVec2(selection.END_X - ImGui::GetScrollX(), selection.END_Y - ImGui::GetScrollY()), col_selection);
		}
		
		ImGui::Columns(tracks);
		
		// grid drawing loop
		for (int i = pattern_start; i < pattern_end; ++i)
		{
			for (int j = 0; j < tracks; ++j)
			{
				ImGui::Text(module[i][j].NAME.c_str());
				ImGui::SameLine();
				
				if (module[i][j].INSTRUMENT != -1)
				{
					ImGui::Text(std::to_string(module[i][j].INSTRUMENT).c_str());
				}
				else
				{
					ImGui::Text(" --");
				}
				ImGui::SameLine();
				
				if (module[i][j].VOLUME != 0.0f)
				{
					ImGui::Text(std::to_string(module[i][j].VOLUME).c_str());
				}
				else
				{
					ImGui::Text(" --");
				}
				ImGui::SameLine();
				
				if (module[i][j].FX != -1)
				{
					ImGui::Text(std::to_string(module[i][j].FX).c_str());
				}
				else
				{
					ImGui::Text("---");
				}
			}
			
			ImGui::SetColumnWidth(-1, 120);
			ImGui::NextColumn();
		}
		
		ImGui::Columns(1);
		
		// get keyboard
		if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
		{
			if (ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_DownArrow]) && active_cell.ROW < pattern_rows - 1)
			{
				active_cell.LAST_CURSOR_ACTION = DOWN;
				active_cell.Y += cell_height;
				active_cell.ROW++;

				if (active_cell.ROW == patterns_list[active_pattern].ROWS - 1)
				{
					ImGui::SetScrollY(ImGui::GetScrollMaxY());
				}

				if (selection_exists)
				{
					selection_exists = false;
				}
			}
			else if (ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_UpArrow]) && active_cell.ROW > 0)
			{
				active_cell.LAST_CURSOR_ACTION = UP;
				active_cell.Y -= cell_height;
				active_cell.ROW--;

				if (active_cell.ROW == 0)
				{
					ImGui::SetScrollY(0);
				}

				if (selection_exists)
				{
					selection_exists = false;
				}
			}
			else if (ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_RightArrow]) && active_cell.COL < (tracks * 4) - 1)
			{
				active_cell.LAST_CURSOR_ACTION = RIGHT;
				active_cell.X += cell_width;
				active_cell.COL++;

				if (active_cell.COL == (tracks * 4) - 1)
				{
					ImGui::SetScrollX(ImGui::GetScrollMaxX());
				}

				if (selection_exists)
				{
					selection_exists = false;
				}
			}
			else if (ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_LeftArrow]) && active_cell.COL > 0)
			{
				active_cell.LAST_CURSOR_ACTION = LEFT;
				active_cell.X -= cell_width;
				active_cell.COL--;

				if (active_cell.COL == 0)
				{
					ImGui::SetScrollX(0);
				}

				if (selection_exists)
				{
					selection_exists = false;
				}
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
		ImGui::SFML::Render(window); // @note you need to have this
		window.display(); // if you want scary black atmosphere
	}
	
	// @note pointless function; if your program closes why do ya need to spend
	// extra time to close it?
	// ImGui::SFML::Shutdown();
}
