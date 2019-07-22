LoadTextures();

io.ConfigWindowsMoveFromTitleBarOnly = true;
ImVec4* colors = ImGui::GetStyle().Colors;
colors[ImGuiCol_Separator] = col_track_separator;
colors[ImGuiCol_SeparatorHovered] = col_track_separator;
colors[ImGuiCol_SeparatorActive] = col_track_separator;

ImGuiStyle& style = ImGui::GetStyle();
style.FrameBorderSize = 1.0f;

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

color_info[HeadingText] = {"Heading Text", col_heading_text, DEFAULT_COL_HEADING_TEXT};
color_info[Cursor] = {"Cursor", col_cursor, DEFAULT_COL_CURSOR};
color_info[CursorBorder] = {"Cursor Border", col_cursor_border, DEFAULT_COL_CURSOR_BORDER};
color_info[CurrentRow] = {"Current Row", col_current_row, DEFAULT_COL_CURRENT_ROW};
color_info[NthRowHighlight] = {"Nth Row Highlight", col_nth_row_highlight, DEFAULT_COL_NTH_ROW_HIGHLIGHT};
color_info[SelectionRectangle] = {"Selection Rectangle", col_selection_rectangle, DEFAULT_COL_SELECTION_RECTANGLE};
color_info[Buttons] = {"Buttons", col_buttons, DEFAULT_COL_BUTTONS};
color_info[NoteText] = {"Note Text", col_note_text, DEFAULT_COL_NOTE_TEXT};
color_info[InstrumentText] = {"Instrument Text", col_instrument_text, DEFAULT_COL_INSTRUMENT_TEXT};
color_info[VolumeText] = {"Volume Text", col_volume_text, DEFAULT_COL_VOLUME_TEXT};
color_info[WindowBackground] = {"Window Background", col_window_background, DEFAULT_COL_WINDOW_BACKGROUND};
color_info[FrameBackground] = {"Frame Background", col_frame_background, DEFAULT_COL_FRAME_BACKGROUND};
color_info[PatternBackground] = {"Pattern Background", col_pattern_background, DEFAULT_COL_PATTERN_BACKGROUND};
color_info[ToggleButton] = {"Toggle Button", col_toggle_button, DEFAULT_COL_TOGGLE_BUTTON};
color_info[MuteButton] = {"Mute Button", col_mute_button, DEFAULT_COL_MUTE_BUTTON};
color_info[Text] = {"Text", col_text, DEFAULT_COL_TEXT};
color_info[PatternForeground] = {"Pattern Foreground", col_pattern_foreground, DEFAULT_COL_PATTERN_FOREGROUND};

IniLoadSettings();
IniGetColorSchemes();

char drivername[] = "";
int rate = 0;
FMOD_OUTPUTTYPE output;
fsystem->getDriverInfo(0, drivername, 50, 0, &rate, 0, 0);

fsystem->getOutput(&output);
print(drivername, rate, output);

