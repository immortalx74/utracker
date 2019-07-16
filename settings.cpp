ImGui::SetNextWindowSize(ImVec2(588, 400));

if (ImGui::BeginPopupModal("Settings", &is_settings_open, ImGuiWindowFlags_NoResize))
{
    static int page = 0;
    
    if (ImGui::Button("Keyboard", ImVec2(80, 0)))
    {
        page = 0;
    }
    
    if (ImGui::Button("Colors", ImVec2(80, 0)))
    {
        page = 1;
    }
    
    
    ImVec2 parent_pos = ImGui::GetWindowPos();
    ImGui::SetNextWindowPos(ImVec2(parent_pos.x + 100, parent_pos.y + 24));
    ImGui::BeginChild("##pagecontent", ImVec2(480,338), true);
    bool check_ctrl = false;
    bool check_alt = false;
    bool check_shift = false;
    
    switch (page)
    {
        case 0: // general
        ImGui::Checkbox("CTRL", &check_ctrl);
        ImGui::Text("heyyyy!");
        break;
        
        case 1: // colors
        
        ImGui::ColorEdit4("Heading Text", (float*)&col_title_text, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4("Cursor", (float*)&col_active_cell, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4("Cursor Border", (float*)&col_active_cell_border, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4("Current Row", (float*)&col_active_row, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4("Nth Row Highlight", (float*)&col_nth_row_highlight, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4("Selection Rectangle", (float*)&col_selection, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4("Buttons", (float*)&col_button, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4("Note Text", (float*)&col_note, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4("Instrument Text", (float*)&col_instrument, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4("Volume Text", (float*)&col_volume, ImGuiColorEditFlags_NoInputs);
        
        break;
    }
    
    ImGui::EndChild();
    
    ImGui::SetCursorPos(ImVec2(276, 370));
    
    if (ImGui::Button("Reset to Defaults"))
    {
        col_title_text = DEFAULT_COL_TITLE_TEXT;
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("OK", ImVec2(80,0)))
    {
        is_settings_open= false;
        ImGui::CloseCurrentPopup();
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Cancel", ImVec2(80,0)))
    {
        is_settings_open= false;
        ImGui::CloseCurrentPopup();
    }
    
    ImGui::EndPopup();
}