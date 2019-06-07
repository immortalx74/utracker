#define MAX_TRACKS_PER_MODULE 32
#define MAX_PATTERNS_PER_MODULE 32
#define MAX_INSTRUMENTS_PER_MODULE 32
#define MAX_SAMPLES_PER_MODULE 32
#define MAX_ROWS_PER_PATTERN 512
#define MAX_BPM 512

std::future<bool> globalfut;
std::future<bool> globalfut2;
int playrow = 0;
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
	PLAYING,
	EDITOR
};

APP_STATE application_state = EDITOR;
int col_btn_repeat = IM_COL32(255,130,0,255);
int col_title_text = IM_COL32(255,130,0,255);
int col_active_cell = IM_COL32(255,130,0,255);
int col_active_row = IM_COL32(77,77,135,255);
int col_nth_row_highlight = IM_COL32(60,60,60,255);
int col_selection = IM_COL32(90,90,90,255);
int col_button = IM_COL32(0,0,0,255);
int col_column_separator = IM_COL32(255,0,0,255);
int col_row_headers = IM_COL32(80,80,80,255);

int col_note = IM_COL32(100,130,200,255);
int col_instrument = IM_COL32(200,200,60,255);
int col_volume = IM_COL32(20,170,20,255);

int tracks = 8;
int active_pattern = 0;
int active_instrument = 0;
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
	"Play PATTERN_","Pause","Stop","Repeat","Cut","Copy","Paste","Settings"};

bool show_demo = false;
std::vector<sf::Texture> toolbar_buttons;


void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

void LoadTextures()
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

bool CreatePattern(std::vector<PATTERN_> &patterns_list, int rows, std::vector<std::vector<NOTE_DATA>> &module)
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
	
	PATTERN_ new_pattern;
	new_pattern.NAME = "PATTERN:" + std::to_string(last_pattern_index + 1);
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
            cur_track_row_data.INSTRUMENT = 0;
            cur_track_row_data.VOLUME = 0;
            cur_track_row_data.FX = -1;
            cur_track_row_data.FX_PARAM = -1;
            row.push_back(cur_track_row_data);
        }
		
        module.push_back(row);
    }
	
	return true;
}

bool CreateInstrument(std::vector<INSTRUMENT> &instruments_list)
{
	int last_instrument_index = instruments_list.size() - 1;
	
	if (instruments_list.size() == MAX_INSTRUMENTS_PER_MODULE)
	{
		return false;
	}
	
	INSTRUMENT new_instrument;
	if (instruments_list.size() == 0)
	{
		new_instrument.NAME = "No Instrument";
	}
	else
	{
		new_instrument.NAME = "Instrument:" + std::to_string(last_instrument_index + 1);	
	}
	
	new_instrument.SAMPLE_MAP = 0; // TEMP!
	
	instruments_list.push_back(new_instrument);
	
	return true;
}

bool CreateTrack(std::vector<TRACK> &tracks_list)
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

void CellSet(int row, int col, NOTE_DATA nd, std::vector<std::vector<NOTE_DATA>> &module)
{
	if (col % 4 == 0)
	{
		module[row][col / 4].NAME = nd.NAME;
		module[row][col / 4].INSTRUMENT = nd.INSTRUMENT;
		module[row][col / 4].VOLUME = nd.VOLUME;
	}
	if (col % 4 == 1) module[row][col / 4].INSTRUMENT = nd.INSTRUMENT;
	if (col % 4 == 2) module[row][col / 4].VOLUME = nd.VOLUME;
	if (col % 4 == 3) module[row][col / 4].FX = nd.FX;
}

std::string KeyToNote(int key, int cur_octave)
{
	// std::array<std::string, 13> notes = {"C-","C#","D-","D#","E-","F-","F#","G-","G#","A-","A#","B-","B#"};
	std::string note;
	
	switch (key)
	{
		// A B C D E F G H I J K  L   M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z
		// 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
		case 16: 	note = "C-"; cur_octave--; break; //Q key
		case 22: 	note = "C#"; cur_octave--; break; //W key
		case 4: 	note = "D-"; cur_octave--; break; //E key
		case 17: 	note = "D#"; cur_octave--; break; //R key
		case 19: 	note = "E-"; cur_octave--; break; //T key
		case 24: 	note = "F-"; cur_octave--; break; //Y key
		case 20: 	note = "F#"; cur_octave--; break; //U key
		case 8: 	note = "G-"; cur_octave--; break; //I key
		case 14: 	note = "G#"; cur_octave--; break; //O key
		case 15: 	note = "A-"; cur_octave--; break; //P key
		case 46: 	note = "A#"; cur_octave--; break; //[ key
		case 47: 	note = "B-"; cur_octave--; break; //] key

		case 0: 	note = "C-"; break; //A key
		case 18: 	note = "C#"; break; //S key
		case 3: 	note = "D-"; break; //D key
		case 5: 	note = "D#"; break; //F key
		case 6: 	note = "E-"; break; //G key
		case 7: 	note = "F-"; break; //H key
		case 9: 	note = "F#"; break; //J key
		case 10: 	note = "G-"; break; //K key
		case 11: 	note = "G#"; break; //L key
		case 48: 	note = "A-"; break; //; key
		case 51: 	note = "A#"; break; //' key

		case 25: 	note = "C-"; cur_octave++; break; //Z key
		case 23: 	note = "C#"; cur_octave++; break; //X key
		case 2: 	note = "D-"; cur_octave++; break; //C key
		case 21: 	note = "D#"; cur_octave++; break; //V key
		case 1: 	note = "E-"; cur_octave++; break; //B key
		case 13: 	note = "F-"; cur_octave++; break; //N key
		case 12: 	note = "F#"; cur_octave++; break; //M key
		case 49: 	note = "G-"; cur_octave++; break; //, key
		case 50: 	note = "G#"; cur_octave++; break; //. key
		case 52: 	note = "A-"; cur_octave++; break; /// key

		default:	return "invalid";
	}

	return note + std::to_string(cur_octave);
}

int KeyToInstrument(int key)
{
	int instr;

	switch (key)
	{
		case 26:case 75:	instr = 0; break; // 0
		case 27:case 76:	instr = 1; break; // 0
		case 28:case 77:	instr = 2; break; // 0
		case 29:case 78:	instr = 3; break; // 0
		case 30:case 79:	instr = 4; break; // 0
		case 31:case 80:	instr = 5; break; // 0
		case 32:case 81:	instr = 6; break; // 0
		case 33:case 82:	instr = 7; break; // 0
		case 34:case 83:	instr = 8; break; // 0
		case 35:case 84:	instr = 9; break; // 0

		default:	return -1;
	}
	return instr;
}

int KeyToVolume(int key)
{
	int vol;

	switch (key)
	{
		case 26:case 75:	vol = 0; break; // 0
		case 27:case 76:	vol = 1; break; // 0
		case 28:case 77:	vol = 2; break; // 0
		case 29:case 78:	vol = 3; break; // 0
		case 30:case 79:	vol = 4; break; // 0
		case 31:case 80:	vol = 5; break; // 0
		case 32:case 81:	vol = 6; break; // 0
		case 33:case 82:	vol = 7; break; // 0
		case 34:case 83:	vol = 8; break; // 0
		case 35:case 84:	vol = 9; break; // 0

		default:	return -1;
	}
	return vol;
}