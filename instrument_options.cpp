ImGui::SetNextWindowSize(ImVec2(UI.INSTRUMENT_OPTIONS_MODAL_WIDTH+200, UI.INSTRUMENT_OPTIONS_MODAL_HEIGHT+200));
ImGui::PushStyleColor(ImGuiCol_PopupBg, color_schemes[active_color_scheme].DATA[WindowBackground].COLOR_VALUE);

if (ImGui::BeginPopupModal("Instrument options", &p_opened, ImGuiWindowFlags_NoResize))
{
    static char current_instrument_name[24] = "";
    static bool enable_write = false;
    
    if (!enable_write)
    {
        instruments_list[active_instrument].TEMP_MAP = instruments_list[active_instrument].SAMPLE_MAP;
        strcpy(current_instrument_name, instruments_list[active_instrument].NAME.c_str());
        enable_write = true;
    }
    
    ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);
    ImGui::Text("Instrument name");
    ImGui::PopStyleColor();
    
    ImGui::PushItemWidth(200);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, color_schemes[active_color_scheme].DATA[FrameBackground].COLOR_VALUE);
    ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);
    ImGui::InputText("##instrumentnname", current_instrument_name, 24, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank);
    ImGui::PopStyleColor(2);
    ImGui::PopItemWidth();
    
    ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[HeadingText].COLOR_VALUE);
    ImGui::Spacing();
    ImGui::Spacing();
    //======================================================
    ImGui::Text("Sample map");
    
    ImGui::PushItemWidth(UI.LEFT_SLIDERS_WIDTH);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, color_schemes[active_color_scheme].DATA[FrameBackground].COLOR_VALUE);
    ImGui::SliderInt("Octave", &samplemap_octave, 0, 9);ImGui::SameLine();
    ImGui::PopStyleColor();
    ImGui::PopItemWidth();
    
    ImGui::SetCursorPosX(188);
    
    std::string oct_str = std::to_string(samplemap_octave);
    std::string octave_notes = 
        "C-" + oct_str +
        " C#" + oct_str +
        " D-" + oct_str +
        " D#" + oct_str +
        " E-" + oct_str +
        " F-" + oct_str +
        " F#" + oct_str +
        " G-" + oct_str +
        " G#" + oct_str +
        " A-" + oct_str +
        " A#" + oct_str +
        " B-" + oct_str;
    
    ImGui::Text(octave_notes.c_str());
    ImGui::PopStyleColor();
    
    int relative_cell_x;
    int relative_cell_y;
    
    
    ImGui::PushStyleColor(ImGuiCol_ChildBg, color_schemes[active_color_scheme].DATA[FrameBackground].COLOR_VALUE);
    ImGui::BeginChild("##samplemap", ImVec2(540,200), true);
    
    ImVec2 csp = ImGui::GetCursorScreenPos();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    float cpx = csp.x;
    float cpy = csp.y;
    int list_size = samples_list.size();
    int xoff = 168;
    float so = ImGui::GetScrollY();
    
    int mrelative_x = ImGui::GetMousePos().x - 176 - ImGui::GetWindowPos().x;
    int mrelative_y = ImGui::GetMousePos().y - 8 - ImGui::GetWindowPos().y;
    
    dl->AddLine(ImVec2(cpx,cpy+so), ImVec2(cpx,cpy+(list_size * 17)+so),ImColor(color_schemes[active_color_scheme].DATA[TrackSeparator].COLOR_VALUE));
    
    // vertical separators
    for (int j = 0; j < 13; ++j)
    {
        dl->AddLine(ImVec2(cpx+xoff,cpy+so), ImVec2(cpx+xoff,cpy+(list_size * 17)+so),ImColor(color_schemes[active_color_scheme].DATA[TrackSeparator].COLOR_VALUE));
        xoff += 28;
    }
    
    // horizontal separators & sample names
    static int selected_sample = 0;
    
    for (int i = 0; i < samples_list.size(); ++i)
    {
        dl->AddLine(ImVec2(ImGui::GetWindowPos().x+8,ImGui::GetWindowPos().y+8-so),
                    ImVec2(ImGui::GetWindowPos().x+512,ImGui::GetWindowPos().y+8-so),
                    ImColor(color_schemes[active_color_scheme].DATA[TrackSeparator].COLOR_VALUE));
        
        std::string full_name = samples_list[i].NAME;
        std::string trunc_name = full_name.substr(0, 20);
        if (i <= 9)
        {
            trunc_name = "0" + std::to_string(i) + ":" + trunc_name;
        }
        else
        {
            trunc_name = std::to_string(i) + ":" + trunc_name;
        }
        
        ImVec4 col;
        if (selected_sample == i)
        {
            col = color_schemes[active_color_scheme].DATA[Cursor].COLOR_VALUE;
        }
        else
        {
            col = color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE;
        }
        
        ImGui::PushStyleColor(ImGuiCol_Text, col);
        ImGui::Text(trunc_name.c_str());
        ImGui::PopStyleColor();
        
        csp = ImGui::GetCursorPos();
        cpx = csp.x;
        cpy = csp.y;
        dl->AddLine(ImVec2(ImGui::GetWindowPos().x+cpx,ImGui::GetWindowPos().y+cpy-so),
                    ImVec2(ImGui::GetWindowPos().x+cpx+504,ImGui::GetWindowPos().y+cpy-so),
                    ImColor(color_schemes[active_color_scheme].DATA[TrackSeparator].COLOR_VALUE));
        
        int xxoff = 0;
        for (int k = 0; k < 12; ++k)
        {
            if (instruments_list[active_instrument].TEMP_MAP[(12 * samplemap_octave) + k] == i)
            {
                dl->AddRectFilled(ImVec2(ImGui::GetWindowPos().x+177 + xxoff,ImGui::GetWindowPos().y+cpy-16-so),
                                  ImVec2(ImGui::GetWindowPos().x+177 + xxoff + 26,ImGui::GetWindowPos().y+cpy-so),
                                  ImColor(color_schemes[active_color_scheme].DATA[Cursor].COLOR_VALUE));
            }
            
            xxoff += 28;
        }
    }
    // get cell
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
    {
        // xoff=176, xmax=335, yoff=8
        relative_cell_x = mrelative_x / 28;
        relative_cell_y = (mrelative_y + ImGui::GetScrollY()) / 17;
        
        if (ImGui::IsMouseClicked(0))
        {
            if (relative_cell_x >=0 && relative_cell_x <= 11 && relative_cell_y >= 0 && relative_cell_y <= samples_list.size() - 1 && mrelative_x >= 0 && mrelative_y >= 0)
            {
                instruments_list[active_instrument].TEMP_MAP[(12 * samplemap_octave) + relative_cell_x] = relative_cell_y;
            }
            else if (relative_cell_x < 0 && relative_cell_y >= 0 && relative_cell_y <= samples_list.size() - 1 && mrelative_x < 0 && mrelative_y >= 0)
            {
                selected_sample = relative_cell_y;
            }
        }
    }
    
    ImGui::EndChild();
    ImGui::PopStyleColor();
    //=====================================================
    
    // Assign sample to octave(s)
    if (selected_sample >= 0 && selected_sample < samples_list.size())
    {
        ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[HeadingText].COLOR_VALUE);
        ImGui::Text("Assign selected sample to:");
        ImGui::PopStyleColor();
        
        ImGui::PushStyleColor(ImGuiCol_Button, color_schemes[active_color_scheme].DATA[Buttons].COLOR_VALUE);
        ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);
        if (ImGui::Button("Current octave"))
        {
            for (int i = 0; i < 12; ++i)
            {
                instruments_list[active_instrument].TEMP_MAP[(12 * samplemap_octave) + i]= selected_sample;
            }
        }
        
        ImGui::SameLine();
        
        if (ImGui::Button("All octaves"))
        {
            for (int i = 0; i < 120; ++i)
            {
                instruments_list[active_instrument].TEMP_MAP[i]= selected_sample;
            }
        }
        
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }
    
    float modal_width = ImGui::GetWindowSize().x;
    float modal_height = ImGui::GetWindowSize().y;
    ImGui::SetCursorPos(ImVec2(modal_width- 180, modal_height - 30));
    
    ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);
    
    if (ImGui::Button("OK", ImVec2(80,0)))
    {
        enable_write = false;
        
        if (strlen(current_instrument_name) != 0)
        {
            instruments_list[active_instrument].NAME = current_instrument_name;
        }
        
        instruments_list[active_instrument].SAMPLE_MAP = instruments_list[active_instrument].TEMP_MAP;
        ImGui::CloseCurrentPopup();
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Cancel", ImVec2(80,0)))
    {
        enable_write = false;
        instruments_list[active_instrument].TEMP_MAP = instruments_list[active_instrument].SAMPLE_MAP;
        ImGui::CloseCurrentPopup();
    }
    
    ImGui::PopStyleColor();
    ImGui::EndPopup();
}

ImGui::PopStyleColor();