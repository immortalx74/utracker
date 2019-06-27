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
		if (module[row][i].NAME != "---")
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
    
	result = fsystem->playSound(FMOD_CHANNEL_REUSE, sound, false, &channel);
	ERRCHECK(result);
    
	channel->setChannelGroup(channelgroup);
	
	result = channel->setFrequency(frequency);
	ERRCHECK(result);
	return true;
}

bool PlayRow(
std::vector<std::vector<NOTE_DATA>> &module,
FMOD::System *fsystem,
FMOD::Channel **channel,
FMOD::Sound *sound,
FMOD::ChannelGroup *channelgroup,
int row,
int track_count)
{
	FMOD_RESULT result;
	float freq;
	(*channel)->setChannelGroup(channelgroup);
	channelgroup->stop();
    
	for (int i = 0; i < track_count; ++i)
	{
		if (module[row][i].NAME != "---")
		{
			result = fsystem->playSound(FMOD_CHANNEL_FREE, sound, false, channel);
			ERRCHECK(result);
            
			(*channel)->setChannelGroup(channelgroup);
            
			freq = module[row][i].FREQUENCY;
			result = (*channel)->setFrequency(freq);
			ERRCHECK(result);
		}
	}
	return true;
}

bool PlayPattern(
std::vector<std::vector<NOTE_DATA>> &module,
FMOD::System *fsystem,
FMOD::Channel *channel,
FMOD::Sound *sound,
FMOD::ChannelGroup *channelgroup,
int start,
int end,
int track_count)
{
	FMOD_RESULT result;
	float freq;
	int chans_playing = 0;
    
	for (int i = start; i < end; ++i)
	{
		if (RowHasContent(module, i, track_count))
		{
			PlayRow(module, fsystem, &channel, sound, channelgroup, i, track_count);
		}
        
		future_tick = std::async(std::launch::async, RowTick, 60000/bpm/ticks_per_row);
		if (future_tick.get() && i < end - 1)
		{
			active_cell.ROW++;
		}
        
		if (application_state == EDITOR)
		{
			channelgroup->stop();
			return true;
		}
	}
    
	return true;
}


bool PlayModule(
std::vector<std::vector<NOTE_DATA>> &module,
FMOD::System *fsystem,
FMOD::Channel *channel,
FMOD::Sound *sound,
FMOD::ChannelGroup *channelgroup,
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
        
		PlayPattern(module, fsystem, channel, sound, channelgroup, pat_start, pat_end, track_count);
        
		if (active_pattern < end - 1)
		{
			active_pattern++;
		}
        
		if (application_state == EDITOR)
		{
			channelgroup->stop();
			return true;
		}
	}
	application_state = EDITOR;
	return true;
}