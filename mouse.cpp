if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) && application_state == EDITOR)
{
	mousex = ImGui::GetMousePos().x + ImGui::GetScrollX();
	mousey = ImGui::GetMousePos().y + ImGui::GetScrollY();
	winx = ImGui::GetWindowPos().x;
	winy = ImGui::GetWindowPos().y+8;
	
	if (ImGui::IsMouseClicked(0))
	{
		int cell_col = floor((mousex - winx)) / UI.CELL_WIDTH;
		int cell_row = floor((mousey - winy)) / UI.CELL_HEIGHT;
		if (cell_col <= (tracks_list.size() * 4) - 1) active_cell.COL = cell_col;
		if (cell_row < patterns_list[active_pattern].ROWS) active_cell.ROW = cell_row;
		active_cell.X = active_cell.COL * UI.CELL_WIDTH + winx;
		active_cell.Y = active_cell.ROW * UI.CELL_HEIGHT + winy;
        
		if (selection_exists)
		{
			selection_exists = false;
		}
	}
}