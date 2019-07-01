LoadTextures();

io.ConfigWindowsMoveFromTitleBarOnly = true;
ImVec4* colors = ImGui::GetStyle().Colors;
colors[ImGuiCol_Separator] = col_column_separator;
colors[ImGuiCol_SeparatorHovered] = col_column_separator;
colors[ImGuiCol_SeparatorActive] = col_column_separator;

FMOD::System     *fsystem;
FMOD::Sound      *sound;
FMOD::Channel    *channel = 0;
FMOD::ChannelGroup *channelgroup;
FMOD_RESULT       result;

result = FMOD::System_Create(&fsystem);
ERRCHECK(result);

fsystem->setOutput(FMOD_OUTPUTTYPE_WASAPI);

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

active_cell.X = 306;
active_cell.Y = 132;
active_cell.ROW = 0;
active_cell.COL = 0;

SELECTION selection;
selection.START_X = 0;
selection.START_Y = 0;
selection.END_X = 0;
selection.END_Y = 0;

float grid_scroll_x = 0.0f;
float grid_scroll_y = 0.0f;

float mousex = 0;
float mousey = 0;
float winx = 0;
float winy = 0;
bool key_pressed = false;