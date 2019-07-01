typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

// bool RowTick(int ms)
// {
// 	LARGE_INTEGER start, stop, freq;
// 	QueryPerformanceCounter(&freq);
// 	QueryPerformanceCounter(&start);
// 	bool looping = true;
// 	std::cout << freq.QuadPart << std::endl;

// 	do
// 	{
// 		QueryPerformanceCounter(&stop);

// 		if (1000000 * (stop.QuadPart - start.QuadPart) / freq.QuadPart >= ms)
// 		{
// 			looping = false;
// 			return true;
// 		}
// 	}

// 	while (looping);
// 	return true;
// }

bool RowTick(int mils)
{
	bool looping = true;
    
	auto start = std::chrono::high_resolution_clock::now();
	auto stop = start;
	do
	{
		stop = std::chrono::high_resolution_clock::now();
		fsec fs = stop - start;
		ms d = std::chrono::duration_cast<ms>(fs);
        
		if (d.count() >= mils)
		{
			looping = false;
			return true;
		}
	}
	while(looping);
	return true;
}


bool RowHasContent(std::vector<std::vector<NOTE_DATA>> &module, int row, int track_count)
{
	for (int i = 0; i < track_count; ++i)
	{
		if (module[row][i].NAME != "---" && module[row][i].INSTRUMENT != 0 && module[row][i].INSTRUMENT < instruments_list.size())
		{
			return true;
		}
	}
    
	return false;
}

bool PlayNote(
FMOD::System *fsystem,
FMOD::Channel *channel,
FMOD::Sound *sound,
FMOD::ChannelGroup *channelgroup,
float frequency)
{
	FMOD_RESULT result;
    
	result = fsystem->playSound(sound, 0, false, &channel);
	ERRCHECK(result);
    
	channel->setChannelGroup(channelgroup);
	
	result = channel->setFrequency(frequency);
	ERRCHECK(result);
	return true;
}

bool PlayRow(
std::vector<std::vector<NOTE_DATA>> &module,
FMOD::System *fsystem,
int row,
int track_count)
{
	FMOD_RESULT result;
    FMOD::Sound *s;
    FMOD::Channel *ch;
    FMOD::ChannelGroup *chgroup;
	float freq;
    
    int sample_index;
    
    
	for (int i = 0; i < track_count; ++i)
	{
		ch = tracks_list[i].CHANNEL;
        chgroup = tracks_list[i].CHANNELGROUP;
        ch->setChannelGroup(chgroup);
        
        if (module[row][i].NAME != "---")
		{
            chgroup->stop();
            //std::cerr << FMOD_ErrorString(result);
            
            sample_index = NoteToSample(module[row][i].NAME, module[row][i].INSTRUMENT);
            s = samples_list[sample_index].SOUND;
            
            result= fsystem->playSound(s, 0, false, &ch);
			ERRCHECK(result);
            
            ch->setChannelGroup(chgroup);
            
			freq = module[row][i].FREQUENCY;
            result = ch->setFrequency(freq);
			ERRCHECK(result);
		}
	}
    
	return true;
}

bool PlayPattern(
std::vector<std::vector<NOTE_DATA>> &module,
FMOD::System *fsystem,
int start,
int end,
int track_count)
{
	FMOD_RESULT result;
    
	for (int i = start; i < end; ++i)
	{
		if (RowHasContent(module, i, track_count))
		{
            
            PlayRow(module, fsystem, i, track_count);
		}
        
		future_tick = std::async(std::launch::async, RowTick, 60000/bpm/ticks_per_row);
		if (future_tick.get() && i < end - 1)
		{
			active_cell.ROW++;
		}
        
		if (application_state == EDITOR)
		{
			for (int j = 0; j < tracks_list.size(); ++j)
            {
                tracks_list[j].CHANNELGROUP->stop();
            }
			return true;
		}
	}
    application_state = END_PATTERN;
	return true;
}


bool PlayModule(
std::vector<std::vector<NOTE_DATA>> &module,
FMOD::System *fsystem,
int start,
int end,
int track_count)
{
	FMOD_RESULT result;
	float freq;
	int pat_start;
	int pat_rows;
	int pat_end;
    
	for (int i = start; i < end; ++i)
	{
        pat_start = patterns_list[active_pattern].OFFSET;
        pat_rows = patterns_list[active_pattern].ROWS;
        pat_end = pat_start + pat_rows;
        
		PlayPattern(module, fsystem, pat_start, pat_end, track_count);
        
		if (active_pattern < end - 1)
		{
			active_pattern++;
		}
        
        if (application_state == END_PATTERN)
        {
            application_state = PLAYING;
        }
        
		if (application_state == EDITOR)
		{
			for (int j = 0; j < tracks_list.size(); ++j)
            {
                tracks_list[j].CHANNELGROUP->stop();
            }
			return true;
		}
	}
	application_state = EDITOR;
	return true;
}