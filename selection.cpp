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