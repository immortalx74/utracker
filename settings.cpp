ImGui::SetNextWindowSize(ImVec2(588, 400));
ImGui::PushStyleColor(ImGuiCol_PopupBg, color_schemes[active_color_scheme].DATA[WindowBackground].COLOR_VALUE);

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
    if (ImGui::Button("Sound", ImVec2(80, 0)))
    {
        page = 2;
    }
    
    if (ImGui::Button("About", ImVec2(80, 0)))
    {
        page = 3;
    }
    
    ImVec2 parent_pos = ImGui::GetWindowPos();
    
    static bool check_ctrl = false;
    static bool check_alt = false;
    static bool check_shift = false;
    static int selected = 0;
    std::string binding_name;
    std::string color_name;
    std::string section_and_name;
    INI::File ft;
    ft.Load("settings.ini");
    
    std::string friendly_key_name;
    
    switch (page)
    {
        case 0: // keyboard
        {
            ImGui::SetNextWindowPos(ImVec2(parent_pos.x + 100, parent_pos.y + 24));
            ImGui::PushStyleColor(ImGuiCol_ChildBg, color_schemes[active_color_scheme].DATA[FrameBackground].COLOR_VALUE);
            ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);
            ImGui::BeginChild("##page0", ImVec2(260,338), true);
            
            for (int i = 0; i < key_binding.size(); ++i)
            {
                if (ImGui::Selectable(key_binding[i].FRIENDLY_NAME.c_str(), selected == i))
                {
                    selected = i;
                }
            }
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
            
            
            ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[HeadingText].COLOR_VALUE);
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
            ImGui::PopStyleColor();
            
            ImGui::SetCursorPos(ImVec2(370, 148));
            
            ImGui::PushItemWidth(208);
            
            ImGui::PushStyleColor(ImGuiCol_PopupBg, color_schemes[active_color_scheme].DATA[FrameBackground].COLOR_VALUE);
            ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);
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
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopItemWidth();
        }
        break;
        
        case 1: // colors
        {
            ImGui::SetNextWindowPos(ImVec2(parent_pos.x + 100, parent_pos.y + 24));
            ImGui::PushStyleColor(ImGuiCol_ChildBg, color_schemes[active_color_scheme].DATA[FrameBackground].COLOR_VALUE);
            
            ImGui::BeginChild("##page1", ImVec2(260,338), true);
            
            for (int j = 0; j < color_info.size(); ++j)
            {
                color_name = color_info[j].COLOR_NAME;
                std::string subsection = color_schemes[active_color_scheme].NAME;
                
                
                ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);
                if (ImGui::ColorEdit4(color_info[j].COLOR_NAME.c_str(), (float*)&color_schemes[active_color_scheme].DATA[j].COLOR_VALUE, ImGuiColorEditFlags_NoInputs))
                {
                    float valx = (color_schemes[active_color_scheme].DATA[j].COLOR_VALUE.x * 100) / 100;
                    float valy = (color_schemes[active_color_scheme].DATA[j].COLOR_VALUE.y * 100) / 100;
                    float valz = (color_schemes[active_color_scheme].DATA[j].COLOR_VALUE.z * 100) / 100;
                    float valw = (color_schemes[active_color_scheme].DATA[j].COLOR_VALUE.w * 100) / 100;
                    
                    ft.GetSection("ColorScheme")->GetSubSection(subsection)->SetArrayValue(color_name, 0, valx);
                    ft.GetSection("ColorScheme")->GetSubSection(subsection)->SetArrayValue(color_name, 1, valy);
                    ft.GetSection("ColorScheme")->GetSubSection(subsection)->SetArrayValue(color_name, 2, valz);
                    ft.GetSection("ColorScheme")->GetSubSection(subsection)->SetArrayValue(color_name, 3, valw);
                    
                    ft.Save("settings.ini");
                }
                ImGui::PopStyleColor();
            }
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
            
            //colorschemes========================================
            ImGui::SetCursorPos(ImVec2(370, 28));
            ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[HeadingText].COLOR_VALUE);
            ImGui::Text("Color Schemes");
            ImGui::PopStyleColor();
            
            ImGui::SetCursorPos(ImVec2(370, 48));
            ImGui::PushStyleColor(ImGuiCol_FrameBg, color_schemes[active_color_scheme].DATA[FrameBackground].COLOR_VALUE);
            ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);
            if (ImGui::ListBoxHeader("##colorschemes", ImVec2(208, 224)))
            {   
                for (int i = 0; i < color_scheme_count; ++i)
                {
                    std::string section_name = color_schemes[i].NAME;
                    
                    if (ImGui::Selectable(section_name.c_str(), active_color_scheme == i))
                    {
                        active_color_scheme = i;
                        
                        ft.SetValue("Main:CurrentColorScheme", i);
                        ft.Save("settings.ini");
                    }
                }
            }
            ImGui::ListBoxFooter();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            
            ImGui::SetCursorPos(ImVec2(370, 280));
            
            //====================================================
        }
        break;
        
        case 2: // sound
        {
            ImGui::SetNextWindowPos(ImVec2(parent_pos.x + 100, parent_pos.y + 24));
            ImGui::PushStyleColor(ImGuiCol_ChildBg, color_schemes[active_color_scheme].DATA[WindowBackground].COLOR_VALUE);
            ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);
            ImGui::BeginChild("##page2", ImVec2(480,140), true);//260 338
            
            for (int i = 0; i < audio_devices.size(); ++i)
            {
                if (ImGui::Selectable(audio_devices[i].NAME.c_str(), active_audio_device == i))
                {
                    active_audio_device= i;
                    fsystem->setDriver(active_audio_device);
                }
            }
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
        }
        break;
        
        case 3: // about
        {
            ImGui::SetNextWindowPos(ImVec2(parent_pos.x + 100, parent_pos.y + 24));
            ImGui::PushStyleColor(ImGuiCol_ChildBg, color_schemes[active_color_scheme].DATA[WindowBackground].COLOR_VALUE);
            ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);
            ImGui::BeginChild("##page3", ImVec2(480,338), true);
            
            ImGui::PushStyleColor(ImGuiCol_FrameBg, color_schemes[active_color_scheme].DATA[FrameBackground].COLOR_VALUE);
            ImGui::InputTextMultiline("##aboutbox", ABOUT_TEXT, 600, ImVec2(450,324));
            ImGui::PopStyleColor();
            
            ImGui::EndChild();
            
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
        }
        break;
    }
    
    ImGui::SetCursorPos(ImVec2(364, 370));
    
    if (ImGui::Button("Reset to Defaults"))
    {
        IniSaveDefaults();
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

ImGui::PopStyleColor();