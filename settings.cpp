ImGui::SetNextWindowSize(ImVec2(600, 400));

if (ImGui::BeginPopupModal("Settings", &is_settings_open, ImGuiWindowFlags_NoResize))
{
    static int page = 0;
    
    if (ImGui::Button("General", ImVec2(80, 0)))
    {
        page = 0;
    }
    
    if (ImGui::Button("Colors", ImVec2(80, 0)))
    {
        page = 1;
    }
    
    ImVec2 parent_pos = ImGui::GetWindowPos();
    ImGui::SetNextWindowPos(ImVec2(parent_pos.x + 100, parent_pos.y + 24));
    ImGui::BeginChild("##pagecontent", ImVec2(480,320), true);
    
    switch (page)
    {
        case 0: // general
        ImGui::Text("heyyyy!");
        break;
        
        case 1: // colors
        ImGui::Text("Heading Text");
        ImGui::Text("Cursor");
        ImGui::Text("Cursor Border");
        ImGui::Text("Current Row");
        ImGui::Text("Nth Row Highlight");
        ImGui::Text("Selection Rectangle");
        ImGui::Text("Buttons");
        ImGui::Text("Note Text");
        ImGui::Text("Instrument Text");
        ImGui::Text("Volume Text");
        
        break;
    }
    
    ImGui::EndChild();
    
    ImGui::SetCursorPos(ImVec2(420, 370));
    
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