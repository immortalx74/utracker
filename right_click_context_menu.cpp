if (ImGui::BeginPopupContextItem("pattern context menu"))
{
    if (ImGui::Selectable("Cut"))
    {
        CopyToClipboard(selection.START_ROW, selection.START_COL,
                        selection.END_ROW, selection.END_COL, true);
    }
    if (ImGui::Selectable("Copy"))
    {
        CopyToClipboard(selection.START_ROW, selection.START_COL,
                        selection.END_ROW, selection.END_COL, false);
    }
    if (ImGui::Selectable("Paste"))
    {
        PasteFromClipboard();
    }
    ImGui::Separator();
    if (ImGui::Selectable("Select Track"))
    {
        selection_exists = true;
        
        selection.START_ROW = 0;
        selection.START_COL = (active_cell.COL / 4) * 4;
        selection.END_ROW = patterns_list[active_pattern].ROWS - 1;
        selection.END_COL = selection.START_COL + 3;
    }
    if (ImGui::Selectable("Select All"))
    {
        selection_exists = true;
        
        selection.START_ROW = 0;
        selection.START_COL = 0;
        selection.END_ROW = patterns_list[active_pattern].ROWS - 1;
        selection.END_COL = (tracks_list.size() * 4) - 1;
    }
    
    ImGui::EndPopup();
}
