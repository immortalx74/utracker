#define MAX_TRACKS_PER_MODULE 32
#define MAX_PATTERNS_PER_MODULE 32
#define MAX_INSTRUMENTS_PER_MODULE 32
#define MAX_SAMPLES_PER_MODULE 32
#define MAX_ROWS_PER_PATTERN 512
#define MAX_BPM 512

std::future<bool> future_play;
std::future<bool> future_tick;
// int playrow = 0;
LARGE_INTEGER tick_start;
LARGE_INTEGER tick_end;
LARGE_INTEGER tick_freq;

struct UI_SIZING
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
	float TRACK_SLIDERS_WIDTH = 104;
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

enum LCA
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct ACTIVE_CELL
{
	int X;
	int Y;
	int ROW;
	int COL;
	LCA LAST_CURSOR_ACTION;
};

struct SELECTION
{
	int START_X;
	int START_Y;
	int END_X;
	int END_Y;
};

enum APP_STATE
{
	PLAY_MODULE,
	PLAY_PATTERN,
    END_PATTERN,
	PLAYING,
	EDITOR
};

APP_STATE application_state = EDITOR;
UI_SIZING UI;
ACTIVE_CELL active_cell;

int col_btn_repeat = IM_COL32(255,130,0,255);
int col_title_text = IM_COL32(255,130,0,255);
int col_active_cell = IM_COL32(255,130,0,255);
int col_active_cell_border = IM_COL32(255,255,255,255);
int col_active_row = IM_COL32(77,77,135,255);
int col_nth_row_highlight = IM_COL32(60,60,60,255);
int col_selection = IM_COL32(90,90,90,255);
int col_button = IM_COL32(0,0,0,255);
int col_row_headers = IM_COL32(80,80,80,255);
ImVec4 col_column_separator = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

int col_note = IM_COL32(100,130,200,255);
int col_instrument = IM_COL32(200,200,60,255);
int col_volume = IM_COL32(20,170,20,255);

int tracks = 8;
int active_pattern = 0;
int active_instrument = 0;
int active_sample = 0;
int octave = 5;
int samplemap_octave = 0;
int bpm = 125;
int ticks_per_row = 6;
int step = 0;
bool btn_repeat = false;
bool btn_repeat_changed = false;
int nth_row_highlight = 8;

int key_repeat_delay = 20;
int key_repeat_counter = key_repeat_delay;
bool key_pressed = false;

bool selection_begin = false;
bool selection_exists = false;

std::array<std::string, 13> toolbar_tooltips = {"New","Open","Save","Save as","Play",
	"Play pattern","Pause","Stop","Repeat","Cut","Copy","Paste","Settings"};

bool show_demo = false;
std::vector<sf::Texture> toolbar_buttons;

// create containers
std::vector<PATTERN_> patterns_list;
std::vector<INSTRUMENT> instruments_list;
std::vector<SAMPLE> samples_list;
std::vector<TRACK> tracks_list;
std::vector<std::vector<NOTE_DATA>> module;