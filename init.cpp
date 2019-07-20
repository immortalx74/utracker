//===========================================================================
//INI::File ft;
//
//if (!ft.Load("settings.ini"))
//{
//print("INI ERROR!");
//}
//
//for (int i = 0; i < key_binding.size(); ++i)
//{
//std::string color_name = key_binding[i].FRIENDLY_NAME;
//std::string final_string = "Colors:" + color_name;
//ft.SetValue(final_string, 50);
//}
//
//ft.Save("settings.ini");

//===========================================================================



LoadTextures();

io.ConfigWindowsMoveFromTitleBarOnly = true;
ImVec4* colors = ImGui::GetStyle().Colors;
colors[ImGuiCol_Separator] = col_column_separator;
colors[ImGuiCol_SeparatorHovered] = col_column_separator;
colors[ImGuiCol_SeparatorActive] = col_column_separator;

ImGuiStyle& style = ImGui::GetStyle();
style.FrameBorderSize = 1.0f;

FMOD::System     *fsystem;
FMOD::Sound      *sound;
FMOD::Channel    *channel = 0;
FMOD::ChannelGroup *channelgroup;
FMOD_RESULT       result;

result = FMOD::System_Create(&fsystem);
ERRCHECK(result);

fsystem->setOutput(FMOD_OUTPUTTYPE_WASAPI);

fsystem->setDSPBufferSize(128, 4);

result = fsystem->init(1000, FMOD_INIT_NORMAL, 0);
ERRCHECK(result);

result = fsystem->createChannelGroup("mychannels", &channelgroup);
ERRCHECK(result);

// create default tracks
for (int i = 0; i < tracks; ++i)
{
    CreateTrack(tracks_list, fsystem);
}

CreatePattern(patterns_list, 64, module); // create default PATTERN_
CreateInstrument(instruments_list); // create default instrument (serves as "no instrument" equivalent of MPT)

float grid_scroll_x = 0.0f;
float grid_scroll_y = 0.0f;

float mousex = 0;
float mousey = 0;
float winx = 0;
float winy = 0;
bool key_pressed = false;

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


IniLoadSettings();

