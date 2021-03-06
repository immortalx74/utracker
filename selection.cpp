// begin selection
if (ImGui::IsMouseDragging(0) && ImGui::IsWindowHovered() && application_state == EDITOR)
{
	// determine direction: right-down, right-up, left-down, left-up
	if (mousex > active_cell.X && mousey > active_cell.Y)
	{
		selection.START_COL = active_cell.COL;
		selection.START_ROW = active_cell.ROW;
        selection.END_COL = (floor((mousex - winx) / UI.CELL_WIDTH));
		selection.END_ROW = (floor((mousey - winy) / UI.CELL_HEIGHT));
	}
	else if (mousex > active_cell.X && mousey < active_cell.Y)
	{
		selection.START_COL = active_cell.COL;
		selection.START_ROW = active_cell.ROW;
		selection.END_COL = (floor((mousex - winx) / UI.CELL_WIDTH));
		selection.END_ROW = (floor((mousey - winy) / UI.CELL_HEIGHT));
	}
	else if (mousex < active_cell.X && mousey > active_cell.Y)
	{
        selection.START_COL = (floor((mousex - winx) / UI.CELL_WIDTH));
        selection.START_ROW = active_cell.ROW;
        selection.END_COL = active_cell.COL;
        selection.END_ROW = (floor((mousey - winy) / UI.CELL_HEIGHT));
	}
	else if (mousex < active_cell.X && mousey < active_cell.Y)
	{
		selection.START_COL = (floor((mousex - winx) / UI.CELL_WIDTH));
		selection.START_ROW = active_cell.ROW;
		selection.END_COL = active_cell.COL;
		selection.END_ROW = (floor((mousey - winy) / UI.CELL_HEIGHT));
	}
    
    // safeguards
    if (selection.END_ROW < 0)
    {
        selection.END_ROW = 0;
    }
    
    if (selection.END_ROW > patterns_list[active_pattern].ROWS - 1)
    {
        selection.END_ROW = patterns_list[active_pattern].ROWS - 1;
    }
    
    if (selection.END_COL > (tracks_list.size() * 4) - 1)
    {
        selection.END_COL = (tracks_list.size() * 4) - 1;
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
	
	if (selection.START_COL != selection.END_COL || selection.START_ROW != selection.END_ROW)
	{
        selection_exists = true;
	}
	
	if (ImGui::IsMouseReleased(0) && selection_begin)
	{
		selection_begin = !selection_begin;
	}
}
