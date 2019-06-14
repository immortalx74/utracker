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