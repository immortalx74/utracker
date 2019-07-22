void IniLoadSettings()
{
    INI::File ft;
    
    if(!ft.Load("settings.ini"))
    {
        //
        return;
    }
    
    std::string binding_name;
    std::string color_name;
    std::string section_and_name;
    
    // key bindings
    for (int i = 0; i < key_binding.size(); ++i)
    {
        binding_name = key_binding[i].FRIENDLY_NAME;
        section_and_name= "KeyBindings:" + binding_name;
        
        if (ft.GetValue(section_and_name).AsArray()[3].AsInt() < 0)
        {
            ft.SetArrayValue(section_and_name, 0, key_binding[i].MDFR_CTRL);
            ft.SetArrayValue(section_and_name, 1, key_binding[i].MDFR_ALT);
            ft.SetArrayValue(section_and_name, 2, key_binding[i].MDFR_SHIFT);
            ft.SetArrayValue(section_and_name, 3, key_binding[i].KEY);
        }
        
        key_binding[i].MDFR_CTRL = ft.GetValue(section_and_name).AsArray()[0].AsBool();
        key_binding[i].MDFR_ALT = ft.GetValue(section_and_name).AsArray()[1].AsBool();
        key_binding[i].MDFR_SHIFT = ft.GetValue(section_and_name).AsArray()[2].AsBool();
        key_binding[i].KEY = ft.GetValue(section_and_name).AsArray()[3].AsInt();
        
    }
    
    // colors
    for (int j = 0; j < color_info.size(); ++j)
    {
        color_name = color_info[j].COLOR_NAME;
        section_and_name = "Colors:" + color_name;
        
        if (ft.GetValue(section_and_name).AsArray()[0].AsString() == "")
        {
            ft.SetArrayValue(section_and_name, 0, std::to_string(color_info[j].COLOR_VALUE.x));
            ft.SetArrayValue(section_and_name, 1, std::to_string(color_info[j].COLOR_VALUE.y));
            ft.SetArrayValue(section_and_name, 2, std::to_string(color_info[j].COLOR_VALUE.z));
            ft.SetArrayValue(section_and_name, 3, std::to_string(color_info[j].COLOR_VALUE.w));
        }
        
        color_info[j].COLOR_VALUE.x = (float)ft.GetValue(section_and_name).AsArray()[0].AsDouble();
        color_info[j].COLOR_VALUE.y = (float)ft.GetValue(section_and_name).AsArray()[1].AsDouble();
        color_info[j].COLOR_VALUE.z = (float)ft.GetValue(section_and_name).AsArray()[2].AsDouble();
        color_info[j].COLOR_VALUE.w = (float)ft.GetValue(section_and_name).AsArray()[3].AsDouble();
    }
    
    
    ft.Save("settings.ini");
    
}

void IniSaveDefaults()
{
    std::string binding_name;
    bool binding_ctrl;
    bool binding_alt;
    bool binding_shift;
    int binding_key;
    
    std::string color_name;
    std::string section_and_name;
    
    INI::File ft;
    ft.Load("settings.ini");
    
    for (int i = 0; i < color_info.size(); ++i)
    {
        color_name = color_info[i].COLOR_NAME;
        section_and_name = "Colors:" + color_name;
        
        color_info[i].COLOR_VALUE = color_info[i].DEFAULT_COLOR_VALUE;
        
        float valx = (color_info[i].COLOR_VALUE.x * 100) / 100;
        float valy = (color_info[i].COLOR_VALUE.y * 100) / 100;
        float valz = (color_info[i].COLOR_VALUE.z * 100) / 100;
        float valw = (color_info[i].COLOR_VALUE.w * 100) / 100;
        ft.SetArrayValue(section_and_name, 0, valx);
        ft.SetArrayValue(section_and_name, 1, valy);
        ft.SetArrayValue(section_and_name, 2, valz);
        ft.SetArrayValue(section_and_name, 3, valw);
    }
    
    
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
    
    for (int j = 0; j < key_binding.size(); ++j)
    {
        binding_name = key_binding[j].FRIENDLY_NAME;
        section_and_name = "KeyBindings:" + binding_name;
        
        binding_ctrl = key_binding[j].MDFR_CTRL;
        binding_alt = key_binding[j].MDFR_ALT;
        binding_shift = key_binding[j].MDFR_SHIFT;
        binding_key = key_binding[j].KEY;
        
        ft.SetArrayValue(section_and_name, 0, binding_ctrl);
        ft.SetArrayValue(section_and_name, 1, binding_alt);
        ft.SetArrayValue(section_and_name, 2, binding_shift);
        ft.SetArrayValue(section_and_name, 3, binding_key);
    }
    
    
    
    ft.Save("settings.ini");
}

void IniGetColorSchemes()
{
    INI::File ft;
    
    if(!ft.Load("settings.ini"))
    {
        //
        return;
    }
    
    int i = 0;
    
    for (INI::File::sections_iter it = ft.SectionsBegin(); it != ft.SectionsEnd(); ++it)
	{
		
        if (it->first != "Colors" && it->first != "KeyBindings")
        {
            std::string section_name = "";
            section_name = it->first;
            section_name.erase(0, 12);
            color_schemes[i].NAME = section_name;
            
            color_schemes[i].DATA.COLOR_VALUE.x = (float)ft.GetValue(it->first).AsArray()[0].AsDouble();
            color_schemes[i].DATA.COLOR_VALUE.y = (float)ft.GetValue(it->first).AsArray()[1].AsDouble();
            color_schemes[i].DATA.COLOR_VALUE.z = (float)ft.GetValue(it->first).AsArray()[2].AsDouble();
            color_schemes[i].DATA.COLOR_VALUE.w = (float)ft.GetValue(it->first).AsArray()[3].AsDouble();
            i++;
        }
    }
    return;
}