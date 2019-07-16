ImGui::SetNextWindowSize(ImVec2(UI.PATTERN_OPTIONS_MODAL_WIDTH, UI.PATTERN_OPTIONS_MODAL_HEIGHT));

if (ImGui::BeginPopupModal("Pattern Options", &p_opened, ImGuiWindowFlags_NoResize))
{
    static char current_pattern_name[24] = "";
    static int r;
    static bool enable_write = false;
    
    if (!enable_write)
    {
        r = patterns_list[active_pattern].ROWS;
        strcpy(current_pattern_name, patterns_list[active_pattern].NAME.c_str());
        enable_write = true;
    }
    
    ImGui::PushItemWidth(200);
    ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
    ImGui::Text("Number of rows");
    ImGui::SliderInt("##numofrows", &r, 1, MAX_ROWS_PER_PATTERN);
    ImGui::PopStyleColor();
    
    ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
    ImGui::Text("Pattern name");
    ImGui::PopStyleColor();
    
    if (ImGui::InputText("##patternname", current_pattern_name, 24, ImGuiInputTextFlags_CharsNoBlank))
    {
        
    }
    
    ImGui::PopItemWidth();
    
    float modal_width = ImGui::GetWindowSize().x;
    float modal_height = ImGui::GetWindowSize().y;
    ImGui::SetCursorPos(ImVec2(modal_width- 180, modal_height - 30));
    
    if (ImGui::Button("OK", ImVec2(80,0)))
    {
        enable_write = false;
        
        if (strlen(current_pattern_name) != 0)
        {
            patterns_list[active_pattern].NAME = current_pattern_name;
        }
        
        ResizePattern(r);
        
        ImGui::CloseCurrentPopup();
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(80,0)))
    {
        enable_write = false;
        ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
}