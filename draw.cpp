int pattern_start = patterns_list[active_pattern].OFFSET;
int pattern_rows = patterns_list[active_pattern].ROWS;
int pattern_end = pattern_start + pattern_rows;

// draw nth row highlights
ImVec2 c = ImGui::GetCursorScreenPos();
ImDrawList* draw_list = ImGui::GetWindowDrawList();
ImVec2 highlight_tl;
ImVec2 highlight_br;

for (int r = 0; r < pattern_rows; r += nth_row_highlight)
{
	highlight_tl = ImVec2(c.x-4, c.y);
	highlight_br = ImVec2(c.x + (tracks * UI.TRACK_WIDTH) - 8, c.y + UI.CELL_HEIGHT);
	draw_list->AddRectFilled(highlight_tl, highlight_br, ImColor(col_nth_row_highlight));
	c.y += nth_row_highlight * UI.CELL_HEIGHT;
}

// draw active row
ImVec2 row_tl = ImVec2(c.x-4, active_cell.Y - ImGui::GetScrollY());
ImVec2 row_br = ImVec2(c.x + (tracks * UI.TRACK_WIDTH)-8, active_cell.Y + UI.CELL_HEIGHT - ImGui::GetScrollY());
draw_list->AddRectFilled(row_tl, row_br, ImColor(col_active_row));

// draw active cell
ImVec2 cell_tl = ImVec2(active_cell.X - ImGui::GetScrollX(), active_cell.Y - ImGui::GetScrollY());
ImVec2 cell_br = ImVec2(active_cell.X + UI.CELL_WIDTH - ImGui::GetScrollX(), active_cell.Y + UI.CELL_HEIGHT - ImGui::GetScrollY());

draw_list->AddRectFilled(cell_tl, cell_br, ImColor(col_active_cell));

if (ImGui::IsWindowFocused())
{
	
	draw_list->AddRect(cell_tl, cell_br, ImColor(col_active_cell_border));
}

// Auto scroll when using arrow keys / on playback
ImVec2 rect_start, rect_end;

float sx = ImGui::GetScrollX();
float sy = ImGui::GetScrollY();

if (active_cell.LAST_CURSOR_ACTION == DOWN)
{
    rect_start.x = active_cell.X - ImGui::GetScrollX(); rect_start.y = active_cell.Y + UI.CELL_HEIGHT - ImGui::GetScrollY();
    rect_end.x = active_cell.X + UI.CELL_WIDTH - ImGui::GetScrollX(); rect_end.y = active_cell.Y + (2 * UI.CELL_HEIGHT) - ImGui::GetScrollY();
    
    if (!ImGui::IsRectVisible(rect_start,rect_end) && (key_pressed || application_state == PLAYING))
    {
        sy += UI.CELL_HEIGHT;
		ImGui::SetScrollY(sy);
    }
}
else if (active_cell.LAST_CURSOR_ACTION == UP)
{
    rect_start.x = active_cell.X - ImGui::GetScrollX(); rect_start.y = active_cell.Y - UI.CELL_HEIGHT - ImGui::GetScrollY();
    rect_end.x = active_cell.X + UI.CELL_WIDTH - ImGui::GetScrollX(); rect_end.y = active_cell.Y - ImGui::GetScrollY();
    
    if (!ImGui::IsRectVisible(rect_start,rect_end) && (key_pressed || application_state == PLAYING))
    {
        sy -= UI.CELL_HEIGHT;
		ImGui::SetScrollY(sy);
    }
}
else if (active_cell.LAST_CURSOR_ACTION == RIGHT)
{
    rect_start.x = active_cell.X + UI.CELL_WIDTH - ImGui::GetScrollX(); rect_start.y = active_cell.Y - ImGui::GetScrollY();
    rect_end.x = active_cell.X + (2 * UI.CELL_WIDTH) - ImGui::GetScrollX(); rect_end.y = active_cell.Y + UI.CELL_HEIGHT - ImGui::GetScrollY();
    
    if (!ImGui::IsRectVisible(rect_start,rect_end) && (key_pressed || application_state == PLAYING))
    {
        sx += UI.CELL_WIDTH;
		ImGui::SetScrollX(sx);
    }
}
else if (active_cell.LAST_CURSOR_ACTION == LEFT)
{
    rect_start.x = active_cell.X - UI.CELL_WIDTH - ImGui::GetScrollX(); rect_start.y = active_cell.Y - ImGui::GetScrollY();
    rect_end.x = active_cell.X - ImGui::GetScrollX(); rect_end.y = active_cell.Y + UI.CELL_HEIGHT - ImGui::GetScrollY();
    
    if (!ImGui::IsRectVisible(rect_start,rect_end) && (key_pressed || application_state == PLAYING))
    {
        sx -= UI.CELL_WIDTH;
		ImGui::SetScrollX(sx);
    }
}

// draw selection
if (selection_exists)
{
	int srow,scol,erow,ecol;
    if(selection.END_ROW < selection.START_ROW || selection.END_COL < selection.START_COL)
    {
        erow = selection.END_ROW;
        ecol = selection.END_COL + 1;
        srow = selection.START_ROW + 1;
        scol = selection.START_COL;
    }
    else
    {
        srow = selection.START_ROW;
        scol = selection.START_COL;
        erow = selection.END_ROW + 1;
        ecol = selection.END_COL + 1;
    }
    
    ImVec2 selection_tl = ImVec2((scol * UI.CELL_WIDTH) + winx - ImGui::GetScrollX(), (srow * UI.CELL_HEIGHT) + winy - ImGui::GetScrollY());
	ImVec2 selection_br = ImVec2((ecol * UI.CELL_WIDTH) + winx - ImGui::GetScrollX(), (erow * UI.CELL_HEIGHT) + winy - ImGui::GetScrollY());
	draw_list->AddRectFilled(selection_tl, selection_br, ImColor(col_selection));
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