void DrawToolbar(std::vector<sf::Texture> &toolbar_buttons, std::array<std::string,13> &toolbar_tooltips)
{
	ImGui::SetNextWindowSize(ImVec2(UI.TOOLBAR_WIDTH, UI.TOOLBAR_HEIGHT));
	ImGui::SetNextWindowPos(ImVec2(UI.TOOLBAR_X, UI.TOOLBAR_Y));
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
		
		// check button presses
		if (ImGui::ImageButton(toolbar_buttons[i], ImVec2(24, 24)))
		{
			if (i == 8) // repeat
			{
				btn_repeat = !btn_repeat;
			}

			if (i == 4) // play
			{
				application_state = PLAY_MODULE;
				std::cout << application_state << std::endl;
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