#define MAX_TRACKS_PER_MODULE 32
#define MAX_PATTERNS_PER_MODULE 32
#define MAX_INSTRUMENTS_PER_MODULE 32
#define MAX_SAMPLES_PER_MODULE 32
#define MAX_ROWS_PER_PATTERN 512
#define MAX_BPM 512

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

struct PATTERN
{
    std::string NAME;
    int ROWS;
    int OFFSET;
};

struct INSTRUMENT
{
    std::string NAME;
    int SAMPLE_MAP; // temp, array? vector?
};

struct TRACK
{
    int VOLUME;
    float PAN;
    bool MUTE;
    bool SOLO;
};

struct SAMPLE
{
    std::string FILENAME;
    std::string NAME;
    int LOOP_TYPE; // temp, array? vector?
};

struct NOTE_DATA
{
	std::string NAME;
	float FREQUENCY;
	int INSTRUMENT;
	float VOLUME;
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


int col_btn_repeat = IM_COL32(255,130,0,255);
int col_title_text = IM_COL32(255,130,0,255);
int col_active_cell = IM_COL32(255,130,0,255);
int col_active_row = IM_COL32(77,77,135,255);
int col_nth_row_highlight = IM_COL32(60,60,60,255);
int col_selection = IM_COL32(90,90,90,255);
int col_button = IM_COL32(0,0,0,255);
int col_column_separator = IM_COL32(255,0,0,255);

int tracks = 8;
int active_pattern = 0;
int active_instrument = -1;
int octave = 5;
int bpm = 125;
int ticks_per_row = 6;
int step = 0;
bool btn_repeat = false;
bool btn_repeat_changed = false;
int nth_row_highlight = 8;

int key_repeat_delay = 20;
int key_repeat_counter = key_repeat_delay;

bool selection_begin = false;
bool selection_exists = false;

std::array<std::string, 13> toolbar_tooltips = {"New","Open","Save","Save as","Play",
	"Play pattern","Pause","Stop","Repeat","Cut","Copy","Paste","Settings"};

bool show_demo = false;
std::vector<sf::Texture> toolbar_buttons;

void load_textures()
{
	int x_offset = 0;
	
	for (int i = 0; i < 14; ++i)
	{
	    sf::Texture t;
	    if (!t.loadFromFile("test.png", sf::IntRect(x_offset, 0, 24, 24)))
	    {
	        std::cout << "error";
	    }
		
	    toolbar_buttons.push_back(t);
	    x_offset += 24;
	}
}

bool create_pattern(std::vector<PATTERN> &patterns_list, int rows, std::vector<std::vector<NOTE_DATA>> &module)
{
	int last_pattern_rows = 0;
	int last_pattern_offset = 0;
	int last_pattern_index = patterns_list.size() - 1;
	int offset;
	
	if (patterns_list.size() > 0)
	{
		last_pattern_rows = patterns_list.back().ROWS;
		last_pattern_offset = patterns_list.back().OFFSET;
	}
	
	offset = last_pattern_rows + last_pattern_offset;
	
	if (patterns_list.size() == MAX_PATTERNS_PER_MODULE || rows > MAX_ROWS_PER_PATTERN)
	{
		return false;
	}
	
	PATTERN new_pattern;
	new_pattern.NAME = "Pattern" + std::to_string(last_pattern_index + 1);
	new_pattern.ROWS = rows;
	new_pattern.OFFSET = offset;
	
	patterns_list.push_back(new_pattern);
	
	// grow module
    for (int i = new_pattern.OFFSET; i < new_pattern.OFFSET + new_pattern.ROWS; ++i)
    {
        std::vector<NOTE_DATA> row;
		
        for (int j = 0; j < tracks; ++j)
        {
            NOTE_DATA cur_track_row_data;
			
            cur_track_row_data.NAME = "---";
            cur_track_row_data.FREQUENCY = 0.0f;
            cur_track_row_data.INSTRUMENT = -1;
            cur_track_row_data.VOLUME = 0.0f;
            cur_track_row_data.FX = -1;
            cur_track_row_data.FX_PARAM = -1;
            row.push_back(cur_track_row_data);
        }
		
        module.push_back(row);
    }
	
	return true;
}

bool create_instrument(std::vector<INSTRUMENT> &instruments_list)
{
	int last_instrument_index = instruments_list.size() - 1;
	
	if (instruments_list.size() == MAX_INSTRUMENTS_PER_MODULE)
	{
		return false;
	}
	
	INSTRUMENT new_instrument;
	new_instrument.NAME = "Instrument" + std::to_string(last_instrument_index + 1);
	new_instrument.SAMPLE_MAP = 0; // TEMP!
	
	instruments_list.push_back(new_instrument);
	
	return true;
}

bool create_track(std::vector<TRACK> &tracks_list)
{
	if (tracks_list.size() == MAX_TRACKS_PER_MODULE)
	{
		return false;
	}
	
	TRACK new_track;
	new_track.VOLUME = 64;
	new_track.PAN = 0.0f;
	new_track.MUTE = false;
	new_track.SOLO = false;
	
	tracks_list.push_back(new_track);
	
	return true;
}

void cell_set(int row, int track, NOTE_DATA nd, std::vector<std::vector<NOTE_DATA>> &module)
{
	module[row][track].NAME = nd.NAME;
	module[row][track].VOLUME = nd.VOLUME;
}