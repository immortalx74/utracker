void DrawToolbar(std::vector<sf::Texture> &toolbar_buttons, std::array<std::string,13> &toolbar_tooltips)
{
	ImGui::SetNextWindowSize(ImVec2(UI.TOOLBAR_WIDTH, UI.TOOLBAR_HEIGHT));
	ImGui::SetNextWindowPos(ImVec2(UI.TOOLBAR_X, UI.TOOLBAR_Y));
	ImGui::Begin("toolbar", false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
    
	for (int i = 0; i < 13; ++i)
	{
		if (btn_repeat && i == 8)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, col_toggle_button);
			btn_repeat_changed = !btn_repeat_changed;
		}
		
		if (i == 4 || i == 9 || i == 12)
		{
			ImGui::Image(toolbar_buttons[13], ImVec2(24, 24)); // separator
			ImGui::SameLine();
		}
		
		// check button presses
		if (ImGui::ImageButton(toolbar_buttons[i], ImVec2(24, 24)))
		{
			if (i == 1)
			{
				auto f = pfd::open_file("Choose files to read", "/tmp/",
                                        { "Sound Files (.wav .sound)", "*.wav *.sound",
                                        "All Files", "*" },	true);
				
				std::cout << "Selected files:";
				for (auto const &name : f.result())
                    std::cout << " " + name;
				std::cout << "\n";
			}
            
			if (i == 8) // repeat
			{
				btn_repeat = !btn_repeat;
			}
            
			if (i == 4) // play module
			{
				if (application_state != PLAYING)
				{
					application_state = PLAY_MODULE;
				}
			}
            
			if (i == 5) // play pattern
			{
				if (application_state != PLAYING)
				{
					application_state = PLAY_PATTERN;
				}
			}
            
			if (i == 7) // stop
			{
				application_state = EDITOR;
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
}