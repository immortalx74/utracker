ImGui::SetNextWindowSize(ImVec2(400, 240));

if (ImGui::BeginPopupModal("Settings", &ppp, ImGuiWindowFlags_NoResize))
{
    if (ImGui::Button("OK", ImVec2(80,0)))
    {
        ppp = false;
        ImGui::CloseCurrentPopup();
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Cancel", ImVec2(80,0)))
    {
        ppp = false;
        ImGui::CloseCurrentPopup();
    }
    
    ImGui::EndPopup();
}