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
	    if (!t.loadFromFile("res/toolbar.png", sf::IntRect(x_offset, 0, 24, 24)))
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
		new_instrument.NAME = "00:No Instrument";
	}
	else
	{
		// new_instrument.NAME = "Instrument:" + std::to_string(last_instrument_index + 1);	
		if (instruments_list.size() <= 9)
		{
			new_instrument.NAME = "0" + std::to_string(last_instrument_index + 1) + ":(no name)";
		}
		else
		{
			new_instrument.NAME = std::to_string(last_instrument_index + 1) + ":(no name)";
		}
	}
	
	// TODO: Add default sample map here
    
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
		module[row][col / 4].FREQUENCY = nd.FREQUENCY;
		module[row][col / 4].INSTRUMENT = nd.INSTRUMENT;
		module[row][col / 4].VOLUME = nd.VOLUME;
	}
	if (col % 4 == 1) module[row][col / 4].INSTRUMENT = nd.INSTRUMENT;
	if (col % 4 == 2) module[row][col / 4].VOLUME = nd.VOLUME;
	if (col % 4 == 3) module[row][col / 4].FX = nd.FX;
}

int NoteToSample(std::string note, int instrument)
{
    int position = 0;
	std::string base = note.substr(0,2);
	int octave = (int)note.at(2)-'0'; // mike's hack
    
	std::array<std::string, 12> note_sequence {"C-","C#","D-","D#","E-","F-","F#","G-","G#","A-","A#","B-"};
    
	for (int i = 0; i < note_sequence.size(); ++i)
	{
		if (base == note_sequence[i])
		{
			position = i;
			break;
		}
	}
    
    int sample_pos = (octave * 12) + position;
    int sample = instruments_list[instrument].SAMPLE_MAP[sample_pos];
    return sample;
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

double NoteToFrequency(std::string note)
{
	int position = 0;
	std::string base = note.substr(0,2);
	int octave = (int)note.at(2)-'0'; // mike's hack
    
	std::array<std::string, 12> note_sequence {"C-","C#","D-","D#","E-","F-","F#","G-","G#","A-","A#","B-"};
    
	for (int i = 0; i < note_sequence.size(); ++i)
	{
		if (base == note_sequence[i])
		{
			position = i;
			break;
		}
	}
    
	double n = (1378.125 * pow(2, octave)) * pow(1.059463, position); // 1378.125 = C-0 frequency
	return n;
}

float ConvertRange (float from_min, float from_max, float to_min, float to_max, float value)
{
	float result = (((value - from_min) / (from_max - from_min)) * (to_max - to_min)) + to_min;
	return result;
}


bool LoadSample(
std::vector<SAMPLE> &samples_list,
std::string filename,
FMOD::System *fsystem)
{
	int last_sample_index = samples_list.size() - 1;
	
	if (samples_list.size() == MAX_SAMPLES_PER_MODULE)
	{
		return false;
	}
	
    // Extract filename from full path. NOTE: Multiple selection has backslash, single selection a forward slash.
    int n = filename.rfind("/");
    if (n == -1)
    {
        n = filename.rfind("\\");
    }
    std::string name = filename.substr(n+1);
    
    FMOD::Sound *snd;
    FMOD_RESULT result;
    result = fsystem->createSound(filename.c_str(), FMOD_SOFTWARE | FMOD_CREATESAMPLE, 0, &snd);
    
	SAMPLE new_sample;
	new_sample.FILENAME = filename;
	new_sample.NAME = name;
    new_sample.SOUND = snd;
    //TODO: Check if a sample with the same name is loaded already. If it has been loaded DON'T add it!!!
    
	samples_list.push_back(new_sample);
    return true;
}