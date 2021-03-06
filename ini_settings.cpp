bool IniLoadSettings()
{
    INI::File ft;
    
    if(!ft.Load("settings.ini"))
    {
        //
        return true;
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
    
    // get color scheme
    int acs = 0;
    
    if (ft.GetSection("Main")->GetValue("CurrentColorScheme", -1).AsInt() < 0)
    {
        ft.SetValue("Main:CurrentColorScheme", 0);
    }
    acs = ft.GetSection("Main")->GetValue("CurrentColorScheme").AsInt();
    active_color_scheme = acs;
    
    // window size/pos
    if (ft.GetSection("Main")->GetValue("WindowX", -1).AsInt() < 0)
    {
        ft.SetValue("Main:WindowX", 0);
    }
    if (ft.GetSection("Main")->GetValue("WindowY", -1).AsInt() < 0)
    {
        ft.SetValue("Main:WindowY", 0);
    }
    if (ft.GetSection("Main")->GetValue("WindowWidth", -1).AsInt() < 0)
    {
        ft.SetValue("Main:WindowWidth", window_metrics.WIDTH);
    }
    if (ft.GetSection("Main")->GetValue("WindowHeight", -1).AsInt() < 0)
    {
        ft.SetValue("Main:WindowHeight", window_metrics.HEIGHT);
    }
    
    window_metrics.X = ft.GetSection("Main")->GetValue("WindowX").AsInt();
    window_metrics.Y = ft.GetSection("Main")->GetValue("WindowY").AsInt();
    window_metrics.WIDTH = ft.GetSection("Main")->GetValue("WindowWidth").AsInt();
    window_metrics.HEIGHT = ft.GetSection("Main")->GetValue("WindowHeight").AsInt();
    
    if (ft.Save("settings.ini"))
    {
        return true;
    }
    
    return false;
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
    key_binding[PatternSetFocus] = DEFAULT_BINDING_PATTERN_SET_FOCUS;
    key_binding[SelectTrack] = DEFAULT_BINDING_SELECT_TRACK;
    key_binding[ClearSelection] = DEFAULT_BINDING_CLEAR_SELECTION;
    key_binding[Delete] = DEFAULT_BINDING_DELETE;
    
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

bool IniGetColorSchemes()
{
    INI::File ft;
    
    if(!ft.Load("settings.ini"))
    {
        //
        return true;
    }
    
    int i = 0;
    
    for (INI::File::sections_iter it = ft.SectionsBegin(); it != ft.SectionsEnd(); ++it)
	{
        
        if (it->first != "Colors" && it->first != "KeyBindings" && it->first != "Main")
        {
            std::string section_name = "";
            section_name = it->first;
            section_name.erase(0, 12);
            color_schemes[i].NAME = section_name;
            
            for (int j = 0; j < color_info.size(); ++j)
            {
                std::string value_name = color_info[j].COLOR_NAME;
                color_schemes[i].DATA[j].COLOR_VALUE.x = (float)ft.GetSection("ColorScheme")->GetSubSection(section_name)->GetValue(value_name).AsArray()[0].AsDouble();
                color_schemes[i].DATA[j].COLOR_VALUE.y = (float)ft.GetSection("ColorScheme")->GetSubSection(section_name)->GetValue(value_name).AsArray()[1].AsDouble();
                color_schemes[i].DATA[j].COLOR_VALUE.z = (float)ft.GetSection("ColorScheme")->GetSubSection(section_name)->GetValue(value_name).AsArray()[2].AsDouble();
                color_schemes[i].DATA[j].COLOR_VALUE.w = (float)ft.GetSection("ColorScheme")->GetSubSection(section_name)->GetValue(value_name).AsArray()[3].AsDouble();
            }
            
            i++;
        }
    }
    color_scheme_count = i;
    
    return true;
}

bool IniSaveWindow(sf::RenderWindow &w)
{
    INI::File ft;
    
    if(!ft.Load("settings.ini"))
    {
        //
        return true;
    }
    
    ImVec2 cur_window_size = w.getSize();
    ImVec2 cur_window_pos = w.getPosition();
    print(cur_window_pos.y);
    
    ft.SetValue("Main:WindowX", cur_window_pos.x);
    ft.SetValue("Main:WindowY", cur_window_pos.y);
    ft.SetValue("Main:WindowWidth", cur_window_size.x);
    ft.SetValue("Main:WindowHeight", cur_window_size.y);
    
    
    if (ft.Save("settings.ini"))
    {
        return true;
    }
    
    return false;
}