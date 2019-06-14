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
		
		// Left pane
		int app_window_height = io.DisplaySize.y;
		ImGui::SetNextWindowSize(ImVec2(UI.LEFT_PANE_WIDTH, app_window_height));
		ImGui::SetNextWindowPos(ImVec2(UI.LEFT_PANE_X + UI.MARGIN, UI.LEFT_PANE_Y));
		ImGui::Begin("LeftPane", false,ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

		// patterns list
		ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
		ImGui::Text("Patterns");
		ImGui::PopStyleColor();
		
		UI.PATTERNS_LIST_X = ImGui::GetCursorPosX();
		UI.PATTERNS_LIST_Y = ImGui::GetCursorPosY();

		if (ImGui::ListBoxHeader("##patlist", ImVec2(UI.PATTERNS_LIST_WIDTH, UI.PATTERNS_LIST_HEIGHT)))
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
		
		// Instruments list 
		ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
		ImGui::Text("Instruments");
		ImGui::PopStyleColor();
		
		UI.INSTRUMENTS_LIST_X = ImGui::GetCursorPosX();
		UI.INSTRUMENTS_LIST_Y = ImGui::GetCursorPosY();

		if (ImGui::ListBoxHeader("##inslist", ImVec2(UI.INSTRUMENTS_LIST_WIDTH, UI.INSTRUMENTS_LIST_HEIGHT)))
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
		
		// Draw sliders
		ImGui::PushItemWidth(UI.LEFT_SLIDERS_WIDTH);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + UI.MARGIN);
		ImGui::SliderInt("Octave", &octave, 1, 8);
		ImGui::SliderInt("BPM", &bpm, 32, 512);
		ImGui::SliderInt("Ticks/Row", &ticks_per_row, 1, 32);
		ImGui::SliderInt("Step", &step, 0, 32);
		
		ImGui::PopItemWidth();
		
		// show mouse coords
		ImGui::Spacing();
		ImGui::Text("mouse_x:");ImGui::SameLine();ImGui::Text(std::to_string(ImGui::GetMousePos().x).c_str());
		ImGui::Text("mouse_y:");ImGui::SameLine();ImGui::Text(std::to_string(ImGui::GetMousePos().y).c_str());

		ImGui::Text("cell_x:");ImGui::SameLine();ImGui::Text(std::to_string(active_cell.X).c_str());
		ImGui::Text("cell_y:");ImGui::SameLine();ImGui::Text(std::to_string(active_cell.Y).c_str());
		ImGui::Text("cell_row:");ImGui::SameLine();ImGui::Text(std::to_string(active_cell.ROW).c_str());
		ImGui::Text("cell_col:");ImGui::SameLine();ImGui::Text(std::to_string(active_cell.COL).c_str());
		
		// show app state
		ImGui::Spacing();
		std::string as;
		if (application_state == 0) as ="PLAY_MODULE";
		if (application_state == 1) as ="PLAY_PATTERN";
		if (application_state == 2) as ="PLAYING";
		if (application_state == 3) as ="EDITOR";

		ImGui::Text("state:");ImGui::SameLine();ImGui::Text(as.c_str());		
		// Draw patterns and instruments list's buttons
		DrawListButtons(patterns_list, instruments_list, module);
		
		// Draw toolbar
		UI.TOOLBAR_X = UI.LEFT_PANE_X + UI.LEFT_PANE_WIDTH + (2 * UI.MARGIN);
		UI.TOOLBAR_WIDTH = io.DisplaySize.x - UI.LEFT_PANE_X - UI.LEFT_PANE_WIDTH - (3 * UI.MARGIN);
		DrawToolbar(toolbar_buttons, toolbar_tooltips);

		// main
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
		
		//=========================================
		
		// draw track headers
		ImGui::SetNextWindowContentSize(ImVec2((tracks * UI.TRACK_WIDTH) + UI.MAIN_PADDING, UI.CELL_HEIGHT));
		ImGui::SetCursorPosX(UI.TRACK_HEADERS_START);
		ImGui::BeginChild("##trackheaders", ImVec2(0, UI.TRACK_HEADERS_HEIGHT), false);
		ImGui::SetScrollX(grid_scroll_x);
		ImGui::Columns(tracks);
		
		for (int t = 0; t < tracks; ++t)
		{
			std::string track_text = "    Track:";
			track_text += std::to_string(t + 1);
			ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
			ImGui::Text(track_text.c_str());
			ImGui::PopStyleColor();
			ImGui::PushItemWidth(UI.TRACK_SLIDERS_WIDTH);
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
			ImGui::SetColumnWidth(-1, UI.TRACK_WIDTH);
			ImGui::NextColumn();
		}
		
		ImGui::EndChild();
		ImGui::Columns(1);
		
		// draw row headers
		ImGui::SetNextWindowContentSize(ImVec2(UI.CELL_WIDTH, (pattern_rows * UI.CELL_HEIGHT) + UI.CELL_WIDTH));
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
		
		ImGui::SetCursorPosY(UI.MAIN_X + UI.TRACK_HEADERS_HEIGHT + UI.MARGIN);
		ImGui::SetCursorPosX(UI.CELL_WIDTH + 8);
		ImGui::SetNextWindowContentSize(ImVec2(tracks * UI.TRACK_WIDTH, pattern_rows * UI.CELL_HEIGHT));
		ImGui::BeginChild("##scrollinggrid", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
		
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
		{
			mousex = ImGui::GetMousePos().x + ImGui::GetScrollX();
			mousey = ImGui::GetMousePos().y + ImGui::GetScrollY();
			winx = ImGui::GetWindowPos().x;
			winy = ImGui::GetWindowPos().y+8;
			
			if (ImGui::IsMouseClicked(0))
			{
				int cell_col = floor((mousex - winx)) / UI.CELL_WIDTH;
				int cell_row = floor((mousey - winy)) / UI.CELL_HEIGHT;
				if (cell_col <= (tracks * 4) - 1) active_cell.COL = cell_col;
				if (cell_row < pattern_rows) active_cell.ROW = cell_row;
				active_cell.X = active_cell.COL * UI.CELL_WIDTH + winx;
				active_cell.Y = active_cell.ROW * UI.CELL_HEIGHT + winy;

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
				selection.END_X = (floor((mousex - winx) / UI.CELL_WIDTH) + 1) * UI.CELL_WIDTH + winx;
				selection.END_Y = (floor((mousey - winy) / UI.CELL_HEIGHT) + 1) * UI.CELL_HEIGHT + winy;
			}
			else if (mousex > active_cell.X && mousey < active_cell.Y)
			{
				selection.START_X = active_cell.X;
				selection.START_Y = active_cell.Y + UI.CELL_HEIGHT;
				selection.END_X = (floor((mousex - winx) / UI.CELL_WIDTH) + 1) * UI.CELL_WIDTH + winx;
				selection.END_Y = (floor((mousey - winy) / UI.CELL_HEIGHT)) * UI.CELL_HEIGHT + winy;
			}
			else if (mousex < active_cell.X && mousey > active_cell.Y)
			{
				selection.START_X = (floor((mousex - winx) / UI.CELL_WIDTH)) * UI.CELL_WIDTH + winx;
				selection.START_Y = active_cell.Y;
				selection.END_X = active_cell.X + UI.CELL_WIDTH;
				selection.END_Y = (floor((mousey - winy) / UI.CELL_HEIGHT) + 1) * UI.CELL_HEIGHT + winy;
			}
			else if (mousex < active_cell.X && mousey < active_cell.Y)
			{
				selection.START_X = (floor((mousex - winx) / UI.CELL_WIDTH)) * UI.CELL_WIDTH + winx;
				selection.START_Y = (floor((mousey - winy) / UI.CELL_HEIGHT)) * UI.CELL_HEIGHT + winy;
				selection.END_X = active_cell.X + UI.CELL_WIDTH;
				selection.END_Y = active_cell.Y + UI.CELL_HEIGHT;
			}

			// auto scroll if selection out of bounds
			float sy = ImGui::GetScrollY();
			float sx = ImGui::GetScrollX();

			if (ImGui::GetMousePos().y > ImGui::GetWindowHeight() + ImGui::GetWindowPos().y - 40)
			{
				sy += UI.CELL_HEIGHT;
				ImGui::SetScrollY(sy);
			}

			if (ImGui::GetMousePos().x > ImGui::GetWindowWidth() + ImGui::GetWindowPos().x - 40)
			{
				sx += UI.CELL_HEIGHT;
				ImGui::SetScrollX(sx);
			}

			if (ImGui::GetMousePos().y < ImGui::GetWindowPos().y + 40)
			{
				sy -= UI.CELL_HEIGHT;
				ImGui::SetScrollY(sy);
			}

			if (ImGui::GetMousePos().x < ImGui::GetWindowPos().x + 40)
			{
				sx -= UI.CELL_HEIGHT;
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
		ImVec2 highlight_tl;
		ImVec2 highlight_br;

		for (int r = 0; r < pattern_rows; r += nth_row_highlight)
		{
			highlight_tl = ImVec2(c.x, c.y);
			highlight_br = ImVec2(c.x + (tracks * UI.TRACK_WIDTH) - 8, c.y + UI.CELL_HEIGHT);
			draw_list->AddRectFilled(highlight_tl, highlight_br, col_nth_row_highlight);
			c.y += nth_row_highlight * UI.CELL_HEIGHT;
		}
		
		// draw active row
		ImVec2 row_tl = ImVec2(c.x, active_cell.Y - ImGui::GetScrollY());
		ImVec2 row_br = ImVec2(c.x + (tracks * UI.TRACK_WIDTH)-8, active_cell.Y + UI.CELL_HEIGHT - ImGui::GetScrollY());
		draw_list->AddRectFilled(row_tl, row_br, col_active_row);
		
		// draw active cell
		ImVec2 cell_tl = ImVec2(active_cell.X - ImGui::GetScrollX(), active_cell.Y - ImGui::GetScrollY());
		ImVec2 cell_br = ImVec2(active_cell.X + UI.CELL_WIDTH - ImGui::GetScrollX(), active_cell.Y + UI.CELL_HEIGHT - ImGui::GetScrollY());
		draw_list->AddRectFilled(cell_tl, cell_br, col_active_cell);
		
		//TEMP!!!!! scroll cursor out of bounds test
		ImVec2 rect_start, rect_end;
		rect_start.x = active_cell.X - ImGui::GetScrollX(); rect_start.y = active_cell.Y - ImGui::GetScrollY();
		rect_end.x = active_cell.X + UI.CELL_WIDTH - ImGui::GetScrollX(); rect_end.y = active_cell.Y + UI.CELL_HEIGHT - ImGui::GetScrollY();
		if (!ImGui::IsRectVisible(rect_start,rect_end))
		{
			float sx = ImGui::GetScrollX();
			float sy = ImGui::GetScrollY();
			if (active_cell.LAST_CURSOR_ACTION == DOWN)
			{
				sy += UI.CELL_HEIGHT;
				ImGui::SetScrollY(sy);
			}
			else if (active_cell.LAST_CURSOR_ACTION == UP)
			{
				sy -= UI.CELL_HEIGHT;
				ImGui::SetScrollY(sy);
			}
			else if (active_cell.LAST_CURSOR_ACTION == RIGHT)
			{
				sx += UI.CELL_WIDTH;
				ImGui::SetScrollX(sx);
			}
			else if (active_cell.LAST_CURSOR_ACTION == LEFT)
			{
				sx -= UI.CELL_WIDTH;
				ImGui::SetScrollX(sx);
			}
			
		}
		
		// draw selection
		if (selection_exists)
		{
			ImVec2 selection_tl = ImVec2(selection.START_X - ImGui::GetScrollX(), selection.START_Y - ImGui::GetScrollY());
			ImVec2 selection_br = ImVec2(selection.END_X - ImGui::GetScrollX(), selection.END_Y - ImGui::GetScrollY());
			draw_list->AddRectFilled(selection_tl, selection_br, col_selection);
		}
		
		ImGui::Columns(tracks);
		
		// grid drawing loop
		for (int i = pattern_start; i < pattern_end; ++i)
		{
			for (int j = 0; j < tracks; ++j)
			{
				if (module[i][j].NAME != "---")
				{
					ImGui::PushStyleColor(ImGuiCol_Text, col_note);
					ImGui::Text(module[i][j].NAME.c_str());
					ImGui::PopStyleColor();		
				}
				else
				{
					ImGui::Text(module[i][j].NAME.c_str());	
				}

				ImGui::SameLine();
				
				if (module[i][j].INSTRUMENT != 0)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, col_instrument);
					int instr = module[i][j].INSTRUMENT;
					std::string instr_str = std::to_string(instr);
					if (instr < 10) 
					{
						instr_str = " 0" + instr_str;
					}
					else
					{
						instr_str = " " + instr_str;
					}
					ImGui::Text(instr_str.c_str());
					ImGui::PopStyleColor();
				}
				else
				{
					ImGui::Text(" --");
				}
				ImGui::SameLine();
				
				if (module[i][j].VOLUME != 0.0f)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, col_volume);
					int vol = module[i][j].VOLUME;
					std::string vol_str = std::to_string(vol);
					if (vol < 10)
					{
						vol_str = " 0" + vol_str;
					}
					else
					{
						vol_str = " " + vol_str;
					}
					ImGui::Text(vol_str.c_str());
					ImGui::PopStyleColor();
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
				ImGui::NextColumn();
			}
			
			ImGui::SetColumnWidth(-1, UI.TRACK_WIDTH);
		}
		
		ImGui::Columns(1);
		
		// get keyboard
		if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
		{
			if (ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_DownArrow]) && active_cell.ROW < pattern_rows - 1)
			{
				active_cell.LAST_CURSOR_ACTION = DOWN;
				active_cell.Y += UI.CELL_HEIGHT;
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
				active_cell.Y -= UI.CELL_HEIGHT;
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
				active_cell.X += UI.CELL_WIDTH;
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
				active_cell.X -= UI.CELL_WIDTH;
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

			// note entry
			for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
			{
				if (ImGui::IsKeyReleased(i))
				{
					channelgroup->stop();
					key_pressed = false;
				}
             
                if (ImGui::IsKeyPressed(i) && !key_pressed)
				{
					key_pressed = true;

					if (active_cell.COL % 4 == 0) // note cell
					{
						std::string keychar = KeyToNote(i, octave);

						if (keychar != "invalid")
						{
							NOTE_DATA nd;
							nd.NAME = keychar;

							float freq = NoteToFrequency(keychar);
							nd.FREQUENCY = freq;

							PlayNote(system, channel, sound, channelgroup, freq);
\
							int cur_instr = module[active_cell.ROW + pattern_start][active_cell.COL/4].INSTRUMENT;
							int cur_vol = module[active_cell.ROW + pattern_start][active_cell.COL/4].VOLUME;

							if (cur_instr == 0 && active_instrument > 0)
							{
								nd.INSTRUMENT = active_instrument;
							}
							else
							{
								nd.INSTRUMENT = cur_instr;
							}
							if (cur_vol == 0)
							{
								nd.VOLUME = 64;
							}
							else
							{
								nd.VOLUME = cur_vol;
							}
							CellSet(active_cell.ROW + pattern_start, active_cell.COL, nd, module);

							// apply step to cursor
							if (active_cell.ROW + step < pattern_end)
							{
								active_cell.LAST_CURSOR_ACTION = DOWN;
								active_cell.ROW += step;
								active_cell.Y += step * UI.CELL_HEIGHT;
							}
						}
					}
					else if (active_cell.COL % 4 == 1) // instrument cell
					{
						int keychar = KeyToInstrument(i);

						if (keychar != -1)
						{
							int cur_value = module[active_cell.ROW + pattern_start][active_cell.COL/4].INSTRUMENT;
							int second_digit = (cur_value % 10);
							int new_value;

							if (cur_value > 0)
							{
								new_value = (second_digit * 10) + keychar;
							}
							else if (cur_value == 0)
							{
								new_value = keychar;
							}
							
							NOTE_DATA nd;
							nd.INSTRUMENT = new_value;
							CellSet(active_cell.ROW + pattern_start, active_cell.COL, nd, module);
						}
					}
					else if (active_cell.COL % 4 == 2) // volume cell
					{
						int keychar = KeyToVolume(i);

						if (keychar != -1)
						{
							int cur_value = module[active_cell.ROW + pattern_start][active_cell.COL/4].VOLUME;
							int second_digit = (cur_value % 10);
							int new_value;

							if (cur_value > 0)
							{
								new_value = (second_digit * 10) + keychar;
							}
							else if (cur_value == 0)
							{
								new_value = keychar;
							}
							
							if (new_value > 64) new_value = 64;

							NOTE_DATA nd;
							nd.VOLUME = new_value;
							CellSet(active_cell.ROW + pattern_start, active_cell.COL, nd, module);
						}
					}
					else if (active_cell.COL % 4 == 1) // fx + param cell
					{
						//
					}
				}
			}
		}

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