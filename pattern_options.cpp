ImGui::SetNextWindowSize(ImVec2(UI.PATTERN_OPTIONS_MODAL_WIDTH, UI.PATTERN_OPTIONS_MODAL_HEIGHT));
ImGui::PushStyleColor(ImGuiCol_PopupBg, color_schemes[active_color_scheme].DATA[WindowBackground].COLOR_VALUE);

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
    ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[HeadingText].COLOR_VALUE);
    
    ImGui::Text("Pattern name");
    
    
    ImGui::PushStyleColor(ImGuiCol_FrameBg, color_schemes[active_color_scheme].DATA[FrameBackground].COLOR_VALUE);
    if (ImGui::InputText("##patternname", current_pattern_name, 24, ImGuiInputTextFlags_CharsNoBlank))
    {
        
    }
    ImGui::PopStyleColor();
    
    ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[HeadingText].COLOR_VALUE);
    ImGui::Text("Number of rows");
    ImGui::PushStyleColor(ImGuiCol_FrameBg, color_schemes[active_color_scheme].DATA[FrameBackground].COLOR_VALUE);
    ImGui::SliderInt("##numofrows", &r, 1, MAX_ROWS_PER_PATTERN);
    ImGui::PopStyleColor(3);
    
    
    ImGui::PopItemWidth();
    
    float modal_width = ImGui::GetWindowSize().x;
    float modal_height = ImGui::GetWindowSize().y;
    ImGui::SetCursorPos(ImVec2(modal_width- 180, modal_height - 30));
    
    ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);
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
    ImGui::PopStyleColor();
    ImGui::EndPopup();
}

ImGui::PopStyleColor();