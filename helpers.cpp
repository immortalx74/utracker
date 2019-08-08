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
	sf::Texture tb, bt;
    
	for (int i = 0; i < 14; ++i)
	{
	    if (!tb.loadFromFile("res/toolbar.png", sf::IntRect(x_offset, 0, 24, 24)))
	    {
	        std::cout << "error";
	    }
		
	    toolbar_buttons.push_back(tb);
	    x_offset += 24;
	}
    
    x_offset = 0;
    
    for (int j = 0; j < 6; ++j)
    {
        if (!bt.loadFromFile("res/buttons.png", sf::IntRect(x_offset, 0, 16, 16)))
	    {
	        std::cout << "error";
	    }
        buttons.push_back(bt);
        x_offset += 16;
    }
}

void ResizePattern(int new_row_count)
{
    int old_row_count = patterns_list[active_pattern].ROWS;
    int old_pattern_offset = patterns_list[active_pattern].OFFSET;
    
    if (new_row_count == old_row_count)
    {
        return;
    }
    else if (new_row_count < old_row_count)
    {
        module.erase(module.begin() + old_pattern_offset + new_row_count, module.begin() +
                     old_pattern_offset + old_row_count);
        patterns_list[active_pattern].ROWS = new_row_count;
        
        for (int i = active_pattern + 1; i < patterns_list.size(); ++i)
        {
            patterns_list[i].OFFSET -= old_row_count - new_row_count;
        }
    }
    else
    {
        patterns_list[active_pattern].ROWS = new_row_count;
        
        for (int i = active_pattern + 1; i < patterns_list.size(); ++i)
        {
            // this is wrong!!!
            patterns_list[i].OFFSET += new_row_count - old_row_count;
        }
        
        int start = old_pattern_offset + old_row_count;
        int end = start + new_row_count - old_row_count;
        
        for (int i = start; i < end; ++i)
        {
            std::vector<NOTE_DATA> row;
            
            for (int j = 0; j < tracks_list.size(); ++j)
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
    }
    
}

bool CreatePattern(std::vector<PATTERN_> &patterns_list,
                   int rows,
                   std::vector<std::vector<NOTE_DATA>> &module)
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
    new_pattern.NAME = "(no name)";
	new_pattern.ROWS = rows;
	new_pattern.OFFSET = offset;
	
	patterns_list.push_back(new_pattern);
	
	// grow module
    for (int i = new_pattern.OFFSET; i < new_pattern.OFFSET + new_pattern.ROWS; ++i)
    {
        std::vector<NOTE_DATA> row;
		
        for (int j = 0; j < tracks_list.size(); ++j)
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
    
    active_pattern = patterns_list.size() - 1;
	
	return true;
}

bool DeletePattern(std::vector<PATTERN_> &patterns_list, std::vector<std::vector<NOTE_DATA>> &module)
{
    if (patterns_list.size() == 1)
    {
        return false;
    }
    
    int old_pat_rows = patterns_list[active_pattern].ROWS;
    int start = patterns_list[active_pattern].OFFSET;
    int end = start + old_pat_rows;
    
    patterns_list.erase(patterns_list.begin() + active_pattern);
    
    if (active_pattern > patterns_list.size() - 1)
    {
        active_pattern = patterns_list.size() - 1;
        
        int new_size = module.size() - old_pat_rows;
        module.resize(new_size);
    }
    else
    {
        for (int i = active_pattern; i < patterns_list.size(); ++i)
        {
            patterns_list[i].OFFSET -= old_pat_rows;
        }
        module.erase(module.begin() + start, module.begin() + end);
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
        new_instrument.NAME = "(no name)";
    }
    
    instruments_list.push_back(new_instrument);
    active_instrument = instruments_list.size() - 1;
    
    return true;
}

bool DeleteInstrument()
{
    if (instruments_list.size() > 1)
    {
        instruments_list.erase(instruments_list.begin() + active_instrument);
        
        if (active_instrument > instruments_list.size() - 1)
        {
            active_instrument = instruments_list.size() - 1;
        }
        
        return true;
    }
    
    return false;
}

// position: occupies an existing index. If position is greater than last
// index, the new track gets pushed back and no module data needs to be moved
bool CreateTrack(std::vector<TRACK> &tracks_list, int position, bool init)
{
    if (tracks_list.size() == MAX_TRACKS_PER_MODULE)
    {
        return false;
    }
    
    int rows = module.size();
    int cols = tracks_list.size();
    
    // resize module
    if (!init)
    {
        if (position >= cols) // append to end of module
        {
            for (int i = 0; i < rows; ++i)
            {
                module[i].resize(cols + 1);
                
                module[i][cols].NAME = "---";
                module[i][cols].FREQUENCY = 0.0f;
                module[i][cols].INSTRUMENT = 0;
                module[i][cols].VOLUME = 0;
                module[i][cols].FX = -1;
                module[i][cols].FX_PARAM = -1;
            }
        }
        else // insert at position
        {
            NOTE_DATA track_data;
            
            track_data.NAME = "---";
            track_data.FREQUENCY = 0.0f;
            track_data.INSTRUMENT = 0;
            track_data.VOLUME = 0;
            track_data.FX = -1;
            track_data.FX_PARAM = -1;
            
            for (int i = 0; i < rows; ++i)
            {
                module[i].insert(module[i].begin() + position, track_data);
            }
        }
    }
    
    // check for an existing soloed track.
    // In that case set the new track to mute
    bool mt = false;
    
    for (int i = 0; i < tracks_list.size(); ++i)
    {
        if (tracks_list[i].SOLO)
        {
            mt = true;
        }
    }
    
    TRACK new_track;
    FMOD::ChannelGroup *new_channel_group;
    fsystem->createChannelGroup(0, &new_channel_group);
    
    new_track.VOLUME = 64;
    new_track.PAN = 0.0f;
    new_track.MUTE = mt;
    new_track.SOLO = false;
    new_track.CHANNELGROUP = new_channel_group;
    
    if (position >= cols)
    {
        tracks_list.push_back(new_track);
        return true;
    }
    else
    {
        tracks_list.insert(tracks_list.begin() + position, new_track);
    }
    
    return true;
}

bool DeleteTrack(std::vector<TRACK> &tracks_list, int position)
{
    if (tracks_list.size() == 1)
    {
        return false;
    }
    
    // shrink module
    int rows = module.size();
    for (int i = 0; i < rows; ++i)
    {
        module[i].erase(module[i].begin() + position);
    }
    
    tracks_list.erase(tracks_list.begin() + position);
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
    if (col % 4 == 3) module[row][col / 4].FX_PARAM = nd.FX_PARAM;
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
        
        case 55: return "= ="; // = key (note off)
        
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
    //double n = (261.7f * pow(2, octave)); // 1378.125 = C-0 frequency
    return n;
}

float ConvertRange (float from_min, float from_max, float to_min, float to_max, float value)
{
    float result = (((value - from_min) / (from_max - from_min)) * (to_max - to_min)) + to_min;
    return result;
}


bool LoadSample(std::vector<SAMPLE> &samples_list, std::string filename, FMOD::System *fsystem)
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
    else
    {
        filename.replace(n, 1, "\\");
    }
    
    std::string name = filename.substr(n+1);
    FMOD::Sound *snd;
    FMOD_RESULT result;
    result = fsystem->createSound(filename.c_str(), FMOD_CREATESAMPLE || FMOD_LOOP_NORMAL, 0, &snd);
    
    SAMPLE new_sample;
    new_sample.NAME = name;
    new_sample.FILENAME = filename;
    new_sample.SOUND = snd;
    new_sample.LOOP_TYPE = 0;
    
    samples_list.push_back(new_sample);
    
    active_sample = samples_list.size() - 1;
    return true;
}

bool DeleteSample()
{
    if (samples_list.size() == 0)
    {
        return false;
    }
    
    int current_note_sample;
    
    for (int i = 0; i < instruments_list.size(); ++i)
    {
        for (int j = 0; j < 120; ++j)
        {
            current_note_sample = instruments_list[i].SAMPLE_MAP[j];
            
            if (current_note_sample == active_sample)
            {
                instruments_list[i].SAMPLE_MAP[j] = 0;
            }
            
            else if (current_note_sample > active_sample)
            {
                instruments_list[i].SAMPLE_MAP[j] = current_note_sample - 1;
            }
        }
    }
    
    samples_list[active_sample].SOUND->release();
    samples_list.erase(samples_list.begin() + active_sample);
    
    if (active_sample > samples_list.size() - 1)
    {
        active_sample = samples_list.size() - 1;
    }
    
    return true;
}

void CopyToClipboard(int srow, int scol, int erow, int ecol, bool cut)
{
    scol /= 4;
    ecol /= 4;
    
    if (erow < srow)
    {
        clipboard.START_ROW = erow;
        clipboard.END_ROW = srow;
    }
    else
    {
        clipboard.START_ROW = srow;
        clipboard.END_ROW = erow;
    }
    
    if (ecol < scol)
    {
        clipboard.START_COL = ecol;
        clipboard.END_COL = scol;
    }
    else
    {
        clipboard.START_COL = scol;
        clipboard.END_COL = ecol;
    }
    
    int track_count = clipboard.END_COL - clipboard.START_COL + 1;
    int row_count = clipboard.END_ROW - clipboard.START_ROW + 1;
    int offset = patterns_list[active_pattern].OFFSET ;
    
    // single cell selection
    if (!selection_exists)
    {
        clipboard.START_ROW = active_cell.ROW + offset;
        clipboard.START_COL = active_cell.COL / 4;
        clipboard.END_ROW = clipboard.START_ROW;
        clipboard.END_COL = clipboard.START_COL;
        row_count = 1;
        track_count = 1;
    }
    
    clipboard.CLIPBOARD_DATA.resize(0);
    clipboard.CLIPBOARD_DATA.clear();
    clipboard.CLIPBOARD_DATA.shrink_to_fit();
    
    switch (scol % 4)
    {
        case 0:
        clipboard.FIRST_CELL = CELL_NOTE;
        break;
        
        case 1:
        clipboard.FIRST_CELL = CELL_INSTR;
        break;
        
        case 2:
        clipboard.FIRST_CELL = CELL_VOL;
        break;
        
        case 3:
        clipboard.FIRST_CELL = CELL_FX;
        break;
    }
    
    for (int i = 0; i < row_count; ++i)
    {
        std::vector<NOTE_DATA> row;
        
        for (int j = 0; j < track_count; ++j)
        {
            NOTE_DATA cur_track_row_data;
            
            cur_track_row_data.NAME = module[clipboard.START_ROW + offset + i][clipboard.START_COL + j].NAME;
            cur_track_row_data.FREQUENCY = module[clipboard.START_ROW + offset +  i][clipboard.START_COL + j].FREQUENCY;
            cur_track_row_data.INSTRUMENT = module[clipboard.START_ROW + offset +  i][clipboard.START_COL + j].INSTRUMENT;
            cur_track_row_data.VOLUME = module[clipboard.START_ROW + offset +  i][clipboard.START_COL + j].VOLUME;
            cur_track_row_data.FX = module[clipboard.START_ROW + offset +  i][clipboard.START_COL + j].FX;
            cur_track_row_data.FX_PARAM = module[clipboard.START_ROW + offset +  i][clipboard.START_COL + j].FX_PARAM;
            
            row.push_back(cur_track_row_data);
            
            if (cut)
            {
                module[clipboard.START_ROW + offset + i][clipboard.START_COL + j].NAME = "---";
                module[clipboard.START_ROW + offset +  i][clipboard.START_COL + j].FREQUENCY = 0.0f;
                module[clipboard.START_ROW + offset +  i][clipboard.START_COL + j].INSTRUMENT = 0;
                module[clipboard.START_ROW + offset +  i][clipboard.START_COL + j].VOLUME = 0;
                module[clipboard.START_ROW + offset +  i][clipboard.START_COL + j].FX = -1;
                module[clipboard.START_ROW + offset +  i][clipboard.START_COL + j].FX_PARAM = -1;
            }
        }
        
        clipboard.CLIPBOARD_DATA.push_back(row);
    }
}

void PasteFromClipboard()
{
    if (clipboard.CLIPBOARD_DATA.empty())
    {
        return;
    }
    int offset = patterns_list[active_pattern].OFFSET ;
    int act_cell_track = active_cell.COL / 4;
    int row_count = clipboard.END_ROW - clipboard.START_ROW + 1;
    int track_count = clipboard.END_COL - clipboard.START_COL + 1;
    
    int row_limit = patterns_list[active_pattern].OFFSET + patterns_list[active_pattern].ROWS - active_cell.ROW - patterns_list[active_pattern].OFFSET;
    int col_limit = tracks_list.size() - (active_cell.COL / 4);
    
    if (row_count > row_limit)
    {
        row_count = row_limit;
    }
    
    if (track_count > col_limit)
    {
        track_count = col_limit;
    }
    
    for (int i = 0; i < row_count; ++i)
    {
        for (int j = 0; j < track_count; ++j)
        {
            module[active_cell.ROW + offset + i][act_cell_track + j].NAME = clipboard.CLIPBOARD_DATA[i][j].NAME;
            module[active_cell.ROW + offset + i][act_cell_track + j].FREQUENCY = clipboard.CLIPBOARD_DATA[i][j].FREQUENCY;
            module[active_cell.ROW + offset + i][act_cell_track + j].INSTRUMENT = clipboard.CLIPBOARD_DATA[i][j].INSTRUMENT;
            module[active_cell.ROW + offset + i][act_cell_track + j].VOLUME = clipboard.CLIPBOARD_DATA[i][j].VOLUME;
            module[active_cell.ROW + offset + i][act_cell_track + j].FX = clipboard.CLIPBOARD_DATA[i][j].FX;
            module[active_cell.ROW + offset + i][act_cell_track + j].FX_PARAM = clipboard.CLIPBOARD_DATA[i][j].FX_PARAM;
        }
    }
}

void MovePattern(int direction)
{
    // -1 up, 1 down
    if (direction == -1)
    {
        
    }
    
}

bool GetModifiers(KEYBOARD_BINDING binding)
{
    
    ImGuiIO& io = ImGui::GetIO();
    bool result = (binding.MDFR_CTRL == io.KeyCtrl &&
                   binding.MDFR_ALT == io.KeyAlt &&
                   binding.MDFR_SHIFT == io.KeyShift);
    return result;
}

void CreateNewModule()
{
    // Reset containers
    module.clear();
    samples_list.clear();
    instruments_list.clear();
    patterns_list.clear();
    tracks_list.clear();
    
    
    // Release FMOD stuff
    for (int i = 0; i < tracks_list.size(); ++i)
    {
        tracks_list[i].CHANNELGROUP = 0;
        tracks_list[i].CHANNELGROUP->release();
    }
    
    for (int i = 0; i < samples_list.size(); ++i)
    {
        samples_list[i].SOUND = 0;
        samples_list[i].SOUND->release();
    }
    
    result = fsystem->createChannelGroup("mychannels", &channelgroup);
    ERRCHECK(result);
    
    fsystem->close();
    
    result = fsystem->init(1000, FMOD_INIT_NORMAL, 0);
    ERRCHECK(result);
    
    // create default tracks
    for (int i = 0; i < DEFAULT_TRACK_COUNT; ++i)
    {
        CreateTrack(tracks_list, tracks_list.size(), true);
    }
    
    CreatePattern(patterns_list, 64, module); // create default PATTERN_
    CreateInstrument(instruments_list); // create default instrument (serves as "no instrument" equivalent of MPT)
    
    // set defaults
    octave = 5;
    bpm= 125;
    rows_per_beat = 4;
    step = 0;
    master_volume = 64;
    
}