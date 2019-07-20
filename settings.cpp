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
    
    static bool check_ctrl = false;
    static bool check_alt = false;
    static bool check_shift = false;
    static int selected = 0;
    std::string binding_name;
    std::string section_and_name;
    INI::File ft;
    ft.Load("settings.ini");
    
    std::string friendly_key_name;
    
    switch (page)
    {
        case 0: // keyboard
        ImGui::SetNextWindowPos(ImVec2(parent_pos.x + 100, parent_pos.y + 24));
        ImGui::BeginChild("##page0", ImVec2(260,338), true);
        
        for (int i = 0; i < key_binding.size(); ++i)
        {
            if (ImGui::Selectable(key_binding[i].FRIENDLY_NAME.c_str(), selected == i))
            {
                selected = i;
            }
        }
        
        ImGui::EndChild();
        
        ImGui::SetCursorPos(ImVec2(370, 28));
        if (ImGui::Checkbox("CTRL", &key_binding[selected].MDFR_CTRL))
        {
            binding_name = key_binding[selected].FRIENDLY_NAME;
            section_and_name= "KeyBindings:" + binding_name;
            ft.SetArrayValue(section_and_name, 0, key_binding[selected].MDFR_CTRL);
            ft.Save("settings.ini");
        }
        
        ImGui::SetCursorPos(ImVec2(370, 58));
        if (ImGui::Checkbox("ALT", &key_binding[selected].MDFR_ALT))
        {
            binding_name = key_binding[selected].FRIENDLY_NAME;
            section_and_name= "KeyBindings:" + binding_name;
            ft.SetArrayValue(section_and_name, 1, key_binding[selected].MDFR_ALT);
            ft.Save("settings.ini");
        }
        
        ImGui::SetCursorPos(ImVec2(370, 88));
        if (ImGui::Checkbox("SHIFT", &key_binding[selected].MDFR_SHIFT))
        {
            binding_name = key_binding[selected].FRIENDLY_NAME;
            section_and_name= "KeyBindings:" + binding_name;
            ft.SetArrayValue(section_and_name, 2, key_binding[selected].MDFR_SHIFT);
            ft.Save("settings.ini");
        }
        
        ImGui::SetCursorPos(ImVec2(370, 128));
        
        ImGui::Text("Key binding:");
        
        ImGui::SetCursorPos(ImVec2(370, 148));
        
        ImGui::PushItemWidth(208);
        
        if (ImGui::BeginCombo("##keynamesarray", key_names[key_binding[selected].KEY].c_str()))
        {
            for (int i = 0; i < key_names.size(); ++i)
            {
                binding_name = key_binding[selected].FRIENDLY_NAME;
                section_and_name= "KeyBindings:" + binding_name;
                
                if (ImGui::Selectable(key_names[i].c_str(), key_binding[selected].KEY == i))
                {
                    key_binding[selected].KEY = i;
                    ft.SetArrayValue(section_and_name, 3, key_binding[selected].KEY);
                    ft.Save("settings.ini");
                }
            }
            
            ImGui::EndCombo();
        }
        
        ImGui::PopItemWidth();
        break;
        
        case 1: // colors
        ImGui::SetNextWindowPos(ImVec2(parent_pos.x + 100, parent_pos.y + 24));
        ImGui::BeginChild("##page1", ImVec2(260,338), true);
        
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
        ImGui::ColorEdit4("Window Background", (float*)&col_window_bg, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4("Frame Background", (float*)&col_frame_bg, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4("Pattern Background", (float*)&col_grid_bg, ImGuiColorEditFlags_NoInputs);
        
        ImGui::EndChild();
        break;
    }
    
    ImGui::SetCursorPos(ImVec2(364, 370));
    
    if (ImGui::Button("Reset to Defaults"))
    {
        col_title_text = DEFAULT_COL_TITLE_TEXT;
        col_active_cell = DEFAULT_COL_ACTIVE_CELL;
        col_active_cell_border = DEFAULT_COL_ACTIVE_CELL_BORDER;
        col_active_row = DEFAULT_COL_ACTIVE_ROW;
        col_nth_row_highlight = DEFAULT_COL_NTH_ROW_HIGHLIGHT;
        col_selection = DEFAULT_COL_SELECTION;
        col_button = DEFAULT_COL_BUTTON;
        col_note = DEFAULT_COL_NOTE;
        col_instrument = DEFAULT_COL_INSTRUMENT;
        col_volume = DEFAULT_COL_VOLUME;
        col_window_bg = DEFAULT_COL_WINDOW_BG;
        col_frame_bg = DEFAULT_COL_FRAME_BG;
        col_grid_bg = DEFAULT_COL_GRID_BG;
        
        key_binding[Cut] = DEFAULT_BINDING_CUT;
        key_binding[Copy] = DEFAULT_BINDING_COPY;
        key_binding[Paste] = DEFAULT_BINDING_PASTE;
        key_binding[SelectAll] = DEFAULT_BINDING_SELECT_ALL;
        key_binding[NextInstrument] = DEFAULT_BINDING_NEXT_INSTRUMENT;
        key_binding[PreviousInstrument] = DEFAULT_BINDING_PREVIOUS_INSTRUMENT;
        key_binding[CursorUp] = DEFAULT_BINDING_CURSOR_UP;
        key_binding[CursorDown] = DEFAULT_BINDING_CURSOR_DOWN;
        key_binding[CursorLeft] = DEFAULT_BINDING_CURSOR_LEFT;
        key_binding[CursorRight] = DEFAULT_BINDING_CURSOR_RIGHT;
        key_binding[NextPattern] = DEFAULT_BINDING_NEXT_PATTERN;
        key_binding[PreviousPattern] = DEFAULT_BINDING_PREVIOUS_PATTERN;
        key_binding[NextOctave] = DEFAULT_BINDING_NEXT_OCTAVE;
        key_binding[PreviousOctave] = DEFAULT_BINDING_PREVIOUS_OCTAVE;
        key_binding[IncreaseStep] = DEFAULT_BINDING_INCREASE_STEP;
        key_binding[DecreaseStep] = DEFAULT_BINDING_DECREASE_STEP;
        
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("OK", ImVec2(80,0)))
    {
        is_settings_open= false;
        ImGui::CloseCurrentPopup();
    }
    
    ImGui::SameLine();
    
    ImGui::EndPopup();
}