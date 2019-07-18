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
#define DEFAULT_COL_TOGGLE_BUTTON {1.0f, 0.509f, 0.0f, 1.0f};
#define DEFAULT_COL_TITLE_TEXT {1.0f, 0.509f, 0.0f, 1.0f};
#define DEFAULT_COL_ACTIVE_CELL {1.0f, 0.509f, 0.0f, 1.0f};
#define DEFAULT_COL_ACTIVE_CELL_BORDER {1.0f, 1.0f, 1.0f, 1.0f};
#define DEFAULT_COL_ACTIVE_ROW {0.301f, 0.301f, 0.529f, 1.0f};
#define DEFAULT_COL_NTH_ROW_HIGHLIGHT {0.235f, 0.235f, 0.235f, 1.0f};
#define DEFAULT_COL_SELECTION {0.352f, 0.352f, 0.352f, 1.0f};
#define DEFAULT_COL_BUTTON {0.0f, 0.0f, 0.0f, 1.0f};
#define DEFAULT_COL_ROW_HEADERS {0.313f, 0.313f, 0.313f, 1.0f};
#define DEFAULT_COL_MUTE {0.156f, 0.156f, 0.941f, 1.0f};
#define DEFAULT_COL_COLUMN_SEPARATOR {1.00f, 1.00f, 1.00f, 1.00f};
#define DEFAULT_COL_NOTE {0.392f, 0.509f, 0.784f, 1.00f};
#define DEFAULT_COL_INSTRUMENT {0.784f, 0.784f, 0.235f, 1.00f};
#define DEFAULT_COL_VOLUME {0.078f, 0.666f, 0.078f, 1.00f};

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
    DecreaseStep
};

APP_STATE application_state = EDITOR;
UI_METRICS UI;
ACTIVE_CELL active_cell;
std::array<KEYBOARD_BINDING, 20> key_binding;

ImVec4 col_toggle_button = DEFAULT_COL_TOGGLE_BUTTON;
ImVec4 col_title_text = DEFAULT_COL_TITLE_TEXT;
ImVec4 col_active_cell = DEFAULT_COL_ACTIVE_CELL;
ImVec4 col_active_cell_border = DEFAULT_COL_ACTIVE_CELL_BORDER;
ImVec4 col_active_row = DEFAULT_COL_ACTIVE_ROW;
ImVec4 col_nth_row_highlight = DEFAULT_COL_NTH_ROW_HIGHLIGHT;
ImVec4 col_selection = DEFAULT_COL_SELECTION;
ImVec4 col_button = DEFAULT_COL_BUTTON;
ImVec4 col_row_headers = DEFAULT_COL_ROW_HEADERS;
ImVec4 col_mute = DEFAULT_COL_MUTE;
ImVec4 col_column_separator = DEFAULT_COL_COLUMN_SEPARATOR;
ImVec4 col_note = DEFAULT_COL_NOTE;
ImVec4 col_instrument = DEFAULT_COL_INSTRUMENT;
ImVec4 col_volume = DEFAULT_COL_VOLUME;

int tracks = DEFAULT_TRACK_COUNT;
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
;
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