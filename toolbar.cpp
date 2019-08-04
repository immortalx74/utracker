void DrawToolbar(std::vector<sf::Texture> &toolbar_buttons,
                 std::array<std::string,13> &toolbar_tooltips)
{
	ImGui::SetNextWindowSize(ImVec2(UI.TOOLBAR_WIDTH, UI.TOOLBAR_HEIGHT));
	ImGui::SetNextWindowPos(ImVec2(UI.TOOLBAR_X, UI.TOOLBAR_Y));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, color_schemes[active_color_scheme].DATA[PanelBackground].COLOR_VALUE);
    
    ImGuiStyle& style = ImGui::GetStyle();
    style.FramePadding = ImVec2(1.0f, 3.0f);
    
    ImGui::Begin("toolbar", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
    
	for (int i = 0; i < 13; ++i)
	{
		if (toggle_buttons.BTN_PAUSE && i == 6)
		{
            ImGui::PushStyleColor(ImGuiCol_Button, color_schemes[active_color_scheme].DATA[ToggleButton].COLOR_VALUE);
            toggle_buttons.BTN_PAUSE_CHANGED = !toggle_buttons.BTN_PAUSE_CHANGED;
		}
        
        if (toggle_buttons.BTN_REPEAT && i == 8)
		{
            ImGui::PushStyleColor(ImGuiCol_Button, color_schemes[active_color_scheme].DATA[ToggleButton].COLOR_VALUE);
            toggle_buttons.BTN_REPEAT_CHANGED = !toggle_buttons.BTN_REPEAT_CHANGED;
		}
		
		if (i == 4 || i == 9 || i == 12)
		{
			ImGui::Image(toolbar_buttons[13], ImVec2(24, 24)); // separator
			ImGui::SameLine();
		}
		
		// check button presses
		if (ImGui::ImageButton(toolbar_buttons[i], ImVec2(24, 24), 0, sf::Color::Transparent,
                               color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE))
		{
			if (i == 1) // load
            {
                ReadModule("module.utr");
            }
            
            if (i == 2) // save
            {
                WriteModule("module.utr");
            }
            
            if (i == 4) // play module
			{
				if (selection_exists)
                {
                    selection_exists = false;
                }
                
                if (application_state != PLAYING)
				{
					application_state = PLAY_MODULE;
				}
			}
            
			if (i == 5) // play pattern
			{
				if (selection_exists)
                {
                    selection_exists = false;
                }
                
                if (application_state != PLAYING)
				{
					application_state = PLAY_PATTERN;
				}
			}
            
            if (i == 6) // pause
            {
                toggle_buttons.BTN_PAUSE = !toggle_buttons.BTN_PAUSE;
            }
            
			if (i == 7) // stop
			{
				application_state = EDITOR;
			}
            
            if (i == 8) // repeat
			{
                toggle_buttons.BTN_REPEAT = !toggle_buttons.BTN_REPEAT;
			}
            
            if (i == 9) // cut
            {
                CopyToClipboard(selection.START_ROW, selection.START_COL,
                                selection.END_ROW, selection.END_COL, true);
            }
            
            if (i == 10) // copy
            {
                CopyToClipboard(selection.START_ROW, selection.START_COL,
                                selection.END_ROW, selection.END_COL, false);
            }
            
            if (i == 11) // paste
            {
                PasteFromClipboard();
            }
            
            if (i == 12 && application_state == EDITOR) // settings
            {
                is_settings_open= true;
            }
            
		}
        
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(toolbar_tooltips[i].c_str());
		}
		
		if (toggle_buttons.BTN_PAUSE_CHANGED)
		{
            toggle_buttons.BTN_PAUSE_CHANGED = !toggle_buttons.BTN_PAUSE_CHANGED;
			ImGui::PopStyleColor();
		}
        
        if (toggle_buttons.BTN_REPEAT_CHANGED)
		{
            toggle_buttons.BTN_REPEAT_CHANGED = !toggle_buttons.BTN_REPEAT_CHANGED;
			ImGui::PopStyleColor();
		}
		ImGui::SameLine();
	}
    
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
    style.FramePadding = ImVec2(4.0f, 3.0f);
}