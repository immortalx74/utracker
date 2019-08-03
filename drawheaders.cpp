// draw track headers
ImVec2 cur_pos = ImGui::GetCursorPos();

ImGui::NewLine();

// track add/delete buttons
ImGui::PushStyleColor(ImGuiCol_Button, color_schemes[active_color_scheme].DATA[Buttons].COLOR_VALUE);
ImGui::PushID("track_add");
if (ImGui::ImageButton(buttons[1], ImVec2(16, 16), 0, sf::Color::Transparent,
                       color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE))
{
    ImGui::OpenPopup("Add Track");
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Add Track");
}

if (ImGui::BeginPopup("Add Track"))
{
    static int position = 1;
    
    if (ImGui::RadioButton("Before", position == 0))
    {
        position = 0;
    }
    if (ImGui::RadioButton("After", position == 1))
    {
        position = 1;
    }
    
    ImGui::Separator();
    
    int selected = 0;
    
    for (int i = 0; i < tracks_list.size(); ++i)
    {
        std::string track_text = "Track: ";
        
        if (i < 9)
        {
            track_text += "0";
        }
        track_text += std::to_string(i + 1);
        
        if (ImGui::Selectable(track_text.c_str(), selected == i))
        {
            selected = i;
            CreateTrack(tracks_list, selected + position, false);
        }
    }
    
    ImGui::EndPopup();
}


ImGui::PopID();

ImGui::PushID("track_del");
if (ImGui::ImageButton(buttons[3], ImVec2(16, 16), 0, sf::Color::Transparent,
                       color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE))
{
    ImGui::OpenPopup("Delete Track");
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Delete Track");
}

if (ImGui::BeginPopup("Delete Track"))
{
    ImGui::Text("Delete track");
    
    ImGui::Separator();
    
    int selected = 0;
    for (int i = 0; i < tracks_list.size(); ++i)
    {
        std::string track_text = "Track: ";
        
        if (i < 9)
        {
            track_text += "0";
        }
        track_text += std::to_string(i + 1);
        
        if (ImGui::Selectable(track_text.c_str(), selected == i))
        {
            selected = i;
            DeleteTrack(tracks_list, selected);
        }
    }
    
    ImGui::EndPopup();
}

ImGui::PopID();
ImGui::PopStyleColor();


// start headers
ImGui::SetCursorPos(ImVec2(cur_pos.x, cur_pos.y));

ImGui::SetNextWindowContentSize(ImVec2((tracks_list.size() * UI.TRACK_WIDTH) + UI.MAIN_PADDING, UI.CELL_HEIGHT));
ImGui::SetCursorPosX(UI.TRACK_HEADERS_START);

ImGui::BeginChild("##trackheaders", ImVec2(0, UI.TRACK_HEADERS_HEIGHT), false);

ImGui::SetScrollX(grid_scroll_x);

ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);
ImGui::Columns(tracks_list.size());

for (int t = 0; t < tracks_list.size(); ++t)
{
    std::string track_text = "Track: ";
    
    if (t < 9)
    {
        track_text += "0";
    }
	track_text += std::to_string(t + 1);
	ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[HeadingText].COLOR_VALUE);
	ImGui::Text(track_text.c_str());
	ImGui::PopStyleColor();
    
    ImGui::SameLine();
    ImGui::PushID(std::to_string(t).c_str());
    if (!tracks_list[t].SOLO)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, color_schemes[active_color_scheme].DATA[Buttons].COLOR_VALUE);
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, color_schemes[active_color_scheme].DATA[ToggleButton].COLOR_VALUE);
    }
    if(ImGui::SmallButton("S"))
    {
        bool cur_state = tracks_list[t].SOLO;
        
        for(int s = 0; s < tracks_list.size(); ++s)
        {
            tracks_list[s].SOLO = false;
            tracks_list[s].MUTE = !cur_state;
        }
        
        tracks_list[t].SOLO = !cur_state;
        tracks_list[t].MUTE = false;
    }
    ImGui::PopStyleColor();
    
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Toggle track solo");
    }
    
    
    ImGui::SameLine();
    if (!tracks_list[t].MUTE)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, color_schemes[active_color_scheme].DATA[Buttons].COLOR_VALUE);
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, color_schemes[active_color_scheme].DATA[MuteButton].COLOR_VALUE);
    }
    
    if(ImGui::SmallButton("M"))
    {
        tracks_list[t].MUTE = !tracks_list[t].MUTE;
        
        for(int m = 0; m < tracks_list.size(); ++m)
        {
            if (tracks_list[m].SOLO)
            {
                tracks_list[m].SOLO = !tracks_list[m].SOLO;
            }
        }
    }
    ImGui::PopStyleColor();
    
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Toggle track mute");
    }
    
    ImGui::PopID();
    
	ImGui::PushItemWidth(UI.TRACK_SLIDERS_WIDTH);
	ImGui::PushID(std::to_string(t).c_str());
    
    ImGui::PushStyleColor(ImGuiCol_FrameBg, color_schemes[active_color_scheme].DATA[FrameBackground].COLOR_VALUE);
    ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);
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
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
	ImGui::PopItemWidth();
	if (ImGui::GetColumnsCount() > 1)
    {
        ImGui::SetColumnWidth(-1, UI.TRACK_WIDTH);
        ImGui::NextColumn();
    }
    
}

ImGui::PopStyleColor();
ImGui::EndChild();

ImGui::Columns(1);

// draw row headers
ImGui::SetNextWindowContentSize(ImVec2(UI.CELL_WIDTH, (patterns_list[active_pattern].ROWS * UI.CELL_HEIGHT) + UI.CELL_WIDTH));

ImGui::BeginChild("##rowheaders", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);
ImGui::SetScrollY(grid_scroll_y);
ImGui::SetCursorPosY(8);

ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);

for (int p = 0; p < patterns_list[active_pattern].ROWS; ++p)
{
	std::string row_text = std::to_string(p);
	std::string row_text_zeros = std::string(3 - row_text.length(), '0') + row_text;
	ImGui::Text(row_text_zeros.c_str());
}

ImGui::PopStyleColor();

ImGui::EndChild();


ImGui::SetCursorPosY(UI.MAIN_X + UI.TRACK_HEADERS_HEIGHT + UI.MARGIN);
ImGui::SetCursorPosX(UI.CELL_WIDTH + 8);
ImGui::SetNextWindowContentSize(ImVec2(tracks_list.size() * UI.TRACK_WIDTH, patterns_list[active_pattern].ROWS * UI.CELL_HEIGHT));

ImGui::PushStyleColor(ImGuiCol_ChildBg, color_schemes[active_color_scheme].DATA[PatternBackground].COLOR_VALUE);
{
    int key = key_binding[PatternSetFocus].KEY;
    bool mdfr = GetModifiers(key_binding[PatternSetFocus]);
    if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr)
    {
        ImGui::SetNextWindowFocus();
    }
}

ImGui::BeginChild("##scrollinggrid", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

grid_scroll_x = ImGui::GetScrollX(); // update scroll_x to sync with track headers scrolling
grid_scroll_y = ImGui::GetScrollY(); // update scroll_y to sync with row headers scrolling