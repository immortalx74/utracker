void IniLoadSettings()
{
    INI::File ft;
    
    if(!ft.Load("settings.ini"))
    {
        //
    }
    
    std::string binding_name;
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
    
    ft.Save("settings.ini");
    
}