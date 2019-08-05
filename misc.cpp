#define ABOUT_TEXT "Tracker v0.1 alpha 2019\n\
By John Dodis (Immortalx) jfdodis@yahoo.com\n\
\n\
Credits:\n\
\n\
Omar Cornut for the amazing Dear ImGui UI library\n\
https://github.com/ocornut/imgui\n\
\n\
Firelight Technologies for the powerful FMOD audio library\n\
https://www.fmod.com/\n\
\n\
Sam Hocevar for the suprisingly easy portable file dialogs\n\
https://github.com/samhocevar/portable-file-dialogs\n\
\n\
Lek-sys for his straight-forward ini parser\n\
https://github.com/Lek-sys/LeksysINI\n\
\n\
Last but not least, my son for helping me out with everything!\n\
https://github.com/mdodis"

//hard limits
#define MAX_TRACKS_PER_MODULE 32
#define MAX_PATTERNS_PER_MODULE 32
#define MAX_INSTRUMENTS_PER_MODULE 32
#define MAX_SAMPLES_PER_MODULE 32
#define MAX_ROWS_PER_PATTERN 512
#define MAX_BPM 512

//default app settings
#define DEFAULT_NTH_ROW_HIGHLIGHT 8
#define DEFAULT_TRACK_COUNT 8
#define DEFAULT_BPM 125
#define DEFAULT_ROWS_PER_BEAT 4

//default colors
#define DEFAULT_COL_TOGGLE_BUTTON {1.0f, 0.509f, 0.0f, 1.0f}
#define DEFAULT_COL_HEADING_TEXT {1.0f, 0.509f, 0.0f, 1.0f}
#define DEFAULT_COL_CURSOR {1.0f, 0.509f, 0.0f, 1.0f}
#define DEFAULT_COL_CURSOR_BORDER {1.0f, 1.0f, 1.0f, 1.0f}
#define DEFAULT_COL_CURRENT_ROW {0.301f, 0.301f, 0.529f, 1.0f}
#define DEFAULT_COL_NTH_ROW_HIGHLIGHT {0.235f, 0.235f, 0.235f, 1.0f}
#define DEFAULT_COL_SELECTION_RECTANGLE {0.352f, 0.352f, 0.352f, 1.0f}
#define DEFAULT_COL_BUTTONS {0.0f, 0.0f, 0.0f, 1.0f}
#define DEFAULT_COL_MUTE_BUTTON {0.156f, 0.156f, 0.941f, 1.0f}
#define DEFAULT_COL_TRACK_SEPARATOR {1.00f, 1.00f, 1.00f, 1.00f}
#define DEFAULT_COL_NOTE_TEXT {0.392f, 0.509f, 0.784f, 1.00f}
#define DEFAULT_COL_INSTRUMENT_TEXT {0.784f, 0.784f, 0.235f, 1.00f}
#define DEFAULT_COL_VOLUME_TEXT {0.078f, 0.666f, 0.078f, 1.00f}
#define DEFAULT_COL_PANEL_BACKGROUND {0.06f, 0.06f, 0.06f, 0.94f}
#define DEFAULT_COL_FRAME_BACKGROUND {0.16f, 0.29f, 0.48f, 0.54f}
#define DEFAULT_COL_PATTERN_BACKGROUND {0.06f, 0.06f, 0.06f, 0.94f}
#define DEFAULT_COL_TEXT {1.0f, 1.0f, 1.0f, 1.0f}
#define DEFAULT_COL_PATTERN_FOREGROUND {1.0f, 1.0f, 1.0f, 1.0f}
#define DEFAULT_COL_WINDOW_BACKGROUND {0.0f, 0.0f, 0.0f, 1.0f}


//default keyboard bindings
#define DEFAULT_BINDING_CURSOR_UP {"Cursor Up", false, false, false, ImGuiKey_UpArrow}
#define DEFAULT_BINDING_CURSOR_DOWN {"Cursor Down", false, false, false, ImGuiKey_DownArrow}
#define DEFAULT_BINDING_CURSOR_LEFT {"Cursor Left", false, false, false, ImGuiKey_LeftArrow}
#define DEFAULT_BINDING_CURSOR_RIGHT {"Cursor Right", false, false, false, ImGuiKey_RightArrow}
#define DEFAULT_BINDING_CUT {"Cut", true, false, false, ImGuiKey_X}
#define DEFAULT_BINDING_COPY {"Copy", true, false, false, ImGuiKey_C}
#define DEFAULT_BINDING_PASTE {"Paste", true, false, false, ImGuiKey_V}
#define DEFAULT_BINDING_SELECT_ALL {"Select All", true, false, false, ImGuiKey_A}
#define DEFAULT_BINDING_NEXT_INSTRUMENT {"Next Instrument", true, false, false, ImGuiKey_DownArrow}
#define DEFAULT_BINDING_PREVIOUS_INSTRUMENT {"Previous Instrument", true, false, false, ImGuiKey_UpArrow}
#define DEFAULT_BINDING_NEXT_PATTERN {"Next Pattern", false, false, false, ImGuiKey_NumpadPlus}
#define DEFAULT_BINDING_PREVIOUS_PATTERN {"Previous Pattern", false, false, false, ImGuiKey_NumpadMinus}
#define DEFAULT_BINDING_NEXT_OCTAVE {"Next Octave", false, false, false, ImGuiKey_Multiply}
#define DEFAULT_BINDING_PREVIOUS_OCTAVE {"Previous Octave", false, false, false, ImGuiKey_Divide}
#define DEFAULT_BINDING_INCREASE_STEP {"Increase Step", true, false, false, ImGuiKey_RightArrow}
#define DEFAULT_BINDING_DECREASE_STEP {"Decrease Step", true, false, false, ImGuiKey_LeftArrow}
#define DEFAULT_BINDING_PATTERN_SET_FOCUS {"Pattern Set Focus", false, false, false, ImGuiKey_Enter}
#define DEFAULT_BINDING_SELECT_TRACK {"Select Track", true, false, false, ImGuiKey_L}
#define DEFAULT_BINDING_CLEAR_SELECTION {"Clear Selection", false, false, false, ImGuiKey_Escape}
#define DEFAULT_BINDING_DELETE {"Delete", false, false, false, ImGuiKey_Delete}

std::future<bool> future_play;
std::future<bool> future_tick;

struct UI_METRICS
{
    float MARGIN = 4;
    
    float LEFT_PANE_X = 0;
    float LEFT_PANE_Y = 0;
    float LEFT_PANE_WIDTH = 260;
    float LEFT_PANE_HEIGHT = 0;
    
    float TOOLBAR_X = 0;
    float TOOLBAR_Y = 0;
    float TOOLBAR_WIDTH = 0;
    float TOOLBAR_HEIGHT = 48;
    
    float MAIN_X = 0;
    float MAIN_Y = 0;
    float MAIN_WIDTH = 0;
    float MAIN_HEIGHT = 0;
    float MAIN_PADDING = 70;
    
    float GRID_X = 0;
    float GRID_Y = 0;
    float GRID_WIDTH = 0;
    float GRID_HEIGHT = 0;
    
    float PATTERNS_LIST_X = 0;
    float PATTERNS_LIST_Y = 0;
    float PATTERNS_LIST_WIDTH = 200;
    float PATTERNS_LIST_HEIGHT = 187;
    
    float INSTRUMENTS_LIST_X = 0;
    float INSTRUMENTS_LIST_Y = 0;
    float INSTRUMENTS_LIST_WIDTH = 200;
    float INSTRUMENTS_LIST_HEIGHT = 187;
    
    float SAMPLES_LIST_X = 0;
    float SAMPLES_LIST_Y = 0;
    float SAMPLES_LIST_WIDTH = 200;
    float SAMPLES_LIST_HEIGHT = 187;
    
    
    float LEFT_SLIDERS_WIDTH = 100;
    
    float PATTERN_OPTIONS_MODAL_WIDTH = 400;
    float PATTERN_OPTIONS_MODAL_HEIGHT = 200;
    float INSTRUMENT_OPTIONS_MODAL_WIDTH = 400;
    float INSTRUMENT_OPTIONS_MODAL_HEIGHT = 200;
    
    float TRACK_WIDTH = 120;
    float CELL_WIDTH = 30;
    float CELL_HEIGHT = 17;
    
    float TRACK_HEADERS_START = 46;
    float TRACK_HEADERS_HEIGHT = 60;
    float TRACK_SLIDERS_WIDTH = 108;
};

struct WINDOW_METRICS
{
    int X = 0;
    int Y = 0;
    int WIDTH = 1024;
    int HEIGHT = 920;
};

WINDOW_METRICS window_metrics;

struct PATTERN_
{
    std::string NAME;
    int ROWS;
    int OFFSET;
};

struct INSTRUMENT
{
    std::string NAME;
    std::array<int,120> SAMPLE_MAP = {0}; // 120 = 10 octaves X 12 notes
    std::array<int,120> TEMP_MAP = {0};
};

struct TRACK
{
    int VOLUME;
    float PAN;
    bool MUTE;
    bool SOLO;
    FMOD::Channel *CHANNEL;
    FMOD::ChannelGroup *CHANNELGROUP;
};

struct SAMPLE
{
    std::string NAME;
    std::string FILENAME;
    FMOD::Sound *SOUND;
    int LOOP_TYPE; // temp, array? vector?
};

struct NOTE_DATA
{
    std::string NAME;
    float FREQUENCY;
    int INSTRUMENT;
    int VOLUME;
    int FX;
    int FX_PARAM;
};

enum CELL_TYPE
{
    CELL_NOTE,
    CELL_INSTR,
    CELL_VOL,
    CELL_FX
};

struct CLIPBOARD
{
    int START_ROW = 0;
    int START_COL = 0;
    int END_ROW = 0;
    int END_COL = 0;
    CELL_TYPE FIRST_CELL = CELL_NOTE;
    std::vector<std::vector<NOTE_DATA>> CLIPBOARD_DATA;
};

enum LCA
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct ACTIVE_CELL
{
    int X = 306;
    int Y = 132;
    int ROW = 0;
    int COL = 0;
    LCA LAST_CURSOR_ACTION;
};

struct SELECTION
{
    int START_X = 0;
    int START_Y = 0;
    int END_X = 0;
    int END_Y = 0;
    
    int START_COL;
    int START_ROW;
    int END_COL;
    int END_ROW;
};

enum APP_STATE
{
    PLAY_MODULE,
    PLAY_PATTERN,
    END_PATTERN,
    PLAYING,
    EDITOR
};

struct KEYBOARD_BINDING
{
    std::string FRIENDLY_NAME;
    bool MDFR_CTRL = false;
    bool MDFR_ALT = false;
    bool MDFR_SHIFT = false;
    int KEY = 0;
};



enum BINDING_NAMES
{
    CursorUp,
    CursorDown,
    CursorLeft,
    CursorRight,
    Cut,
    Copy,
    Paste,
    SelectAll,
    NextInstrument,
    PreviousInstrument,
    NextPattern,
    PreviousPattern,
    NextOctave,
    PreviousOctave,
    IncreaseStep,
    DecreaseStep,
    PatternSetFocus,
    SelectTrack,
    ClearSelection,
    Delete
};



APP_STATE application_state = EDITOR;
UI_METRICS UI;
ACTIVE_CELL active_cell;
std::array<KEYBOARD_BINDING, 20> key_binding; // change size when adding new binding

ImVec4 col_toggle_button = DEFAULT_COL_TOGGLE_BUTTON;
ImVec4 col_heading_text = DEFAULT_COL_HEADING_TEXT;
ImVec4 col_cursor = DEFAULT_COL_CURSOR;
ImVec4 col_cursor_border = DEFAULT_COL_CURSOR_BORDER;
ImVec4 col_current_row = DEFAULT_COL_CURRENT_ROW;
ImVec4 col_nth_row_highlight = DEFAULT_COL_NTH_ROW_HIGHLIGHT;
ImVec4 col_selection_rectangle = DEFAULT_COL_SELECTION_RECTANGLE;
ImVec4 col_buttons = DEFAULT_COL_BUTTONS;
ImVec4 col_mute_button = DEFAULT_COL_MUTE_BUTTON;
ImVec4 col_track_separator = DEFAULT_COL_TRACK_SEPARATOR;
ImVec4 col_note_text = DEFAULT_COL_NOTE_TEXT;
ImVec4 col_instrument_text = DEFAULT_COL_INSTRUMENT_TEXT;
ImVec4 col_volume_text = DEFAULT_COL_VOLUME_TEXT;
ImVec4 col_panel_background = DEFAULT_COL_PANEL_BACKGROUND;
ImVec4 col_frame_background = DEFAULT_COL_FRAME_BACKGROUND;
ImVec4 col_pattern_background = DEFAULT_COL_PATTERN_BACKGROUND;
ImVec4 col_text = DEFAULT_COL_TEXT;
ImVec4 col_pattern_foreground = DEFAULT_COL_PATTERN_FOREGROUND;
ImVec4 col_window_background = DEFAULT_COL_WINDOW_BACKGROUND;

struct TOGGLE_BUTTONS
{
    bool BTN_REPEAT = false;
    bool BTN_REPEAT_CHANGED = false;
    bool BTN_PAUSE = false;
    bool BTN_PAUSE_CHANGED = false;
};

TOGGLE_BUTTONS toggle_buttons;

int active_pattern = 0;
int active_instrument = 0;
int active_sample = 0;
int octave = 5;
int samplemap_octave = 0;
int bpm = DEFAULT_BPM;
int rows_per_beat = DEFAULT_ROWS_PER_BEAT;
int step = 0;
int master_volume = 64;
bool btn_repeat = false;
bool btn_repeat_changed = false;
int nth_row_highlight = DEFAULT_NTH_ROW_HIGHLIGHT;

bool key_pressed = false;

bool selection_begin = false;
bool selection_exists = false;

bool is_settings_open = false;

std::array<std::string, 13> toolbar_tooltips = {"New","Open","Save","Save as","Play",
    "Play pattern","Pause","Stop","Repeat","Cut","Copy","Paste","Settings"};

bool show_demo = false;

std::vector<sf::Texture> toolbar_buttons;
std::vector<sf::Texture> buttons;

// create containers
std::vector<PATTERN_> patterns_list;
std::vector<INSTRUMENT> instruments_list;
std::vector<SAMPLE> samples_list;
std::vector<TRACK> tracks_list;
std::vector<std::vector<NOTE_DATA>> module;

SELECTION selection;

CLIPBOARD clipboard;

struct COLOR_INFO
{
    std::string COLOR_NAME;
    ImVec4 COLOR_VALUE;
    ImVec4 DEFAULT_COLOR_VALUE;
};

struct COLOR_SCHEME
{
    std::string NAME;
    std::array<COLOR_INFO, 19> DATA;
};

enum APP_COLORS
{
    HeadingText,
    Cursor,
    CursorBorder,
    CurrentRow,
    NthRowHighlight,
    SelectionRectangle,
    Buttons,
    NoteText,
    InstrumentText,
    VolumeText,
    PanelBackground,
    FrameBackground,
    PatternBackground,
    ToggleButton,
    MuteButton,
    Text,
    PatternForeground,
    WindowBackground,
    TrackSeparator
};

// change size when adding new col definitions
std::array<COLOR_INFO, 19> color_info;

// 20 entries seem to be enough?
std::array<COLOR_SCHEME, 20> color_schemes;
int active_color_scheme = 0;
int color_scheme_count = 0;

// change size when adding new key name
std::array<std::string, 77> key_names = {
    "Tab",
    "Left Arrow",
    "Right Arrow",
    "Up Arrow",
    "Down Arrow",
    "Page Up",
    "Page Down",
    "Home",
    "End",
    "Insert",
    "Delete",
    "BackSpace",
    "Space",
    "Return",
    "Escape",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "Numpad0",
    "Numpad1",
    "Numpad2",
    "Numpad3",
    "Numpad4",
    "Numpad5",
    "Numpad6",
    "Numpad7",
    "Numpad8",
    "Numpad9",
    "+",
    "-",
    "/",
    "*"
};


FMOD::System     *fsystem;
FMOD::Sound      *sound;
FMOD::Channel    *channel = 0;
FMOD::ChannelGroup *channelgroup;
FMOD_RESULT       result;

FMOD::DSP *dsp;

struct AUDIO_DEVICE
{
    std::string NAME;
    int RATE = 0;
};

std::vector<AUDIO_DEVICE> audio_devices;
int active_audio_device = 0;

bool is_playing = false;

std::string module_filename = "";