typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<double> fsec;

bool RowTick(double mils)
{
    bool looping = true;
    
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = start;
    while(looping)
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
    return true;
}


bool RowHasContent(std::vector<std::vector<NOTE_DATA>> &module, int row, int track_count)
{
	for (int i = 0; i < track_count; ++i)
	{
		if (module[row][i].NAME == "= =")
        {
            return true;
        }
        
        if (module[row][i].NAME != "---" &&
            module[row][i].INSTRUMENT != 0 &&
            module[row][i].INSTRUMENT < instruments_list.size()
            && samples_list.size() > 0)
		{
            return true;
		}
	}
    
	return false;
}

bool PlayNote(FMOD::System *fsystem, FMOD::Channel *channel, FMOD::Sound *sound,
              FMOD::ChannelGroup *channelgroup,float frequency)
{
	FMOD_RESULT result;
    
	result = fsystem->playSound(sound, 0, false, &channel);
	ERRCHECK(result);
    
	channel->setChannelGroup(channelgroup);
	
	result = channel->setFrequency(frequency);
	ERRCHECK(result);
    
	return true;
}

bool PlayRow(FMOD::System *fsystem, int row, int track_count)
{
	FMOD_RESULT result;
    FMOD::Sound *s;
    FMOD::Channel *ch;
    FMOD::ChannelGroup *chgroup;
	float freq;
    float vol_note, vol_track, vol_master, vol_final, pan_track;
    
    int sample_index;
    
	for (int i = 0; i < track_count; ++i)
	{
		ch = tracks_list[i].CHANNEL;
        chgroup = tracks_list[i].CHANNELGROUP;
        ch->setChannelGroup(chgroup);
        
        if (module[row][i].NAME != "---" && module[row][i].NAME != "= =" && !tracks_list[i].MUTE)
		{
            chgroup->stop();
            
            sample_index = NoteToSample(module[row][i].NAME, module[row][i].INSTRUMENT);
            s = samples_list[sample_index].SOUND;
            
            result= fsystem->playSound(s, 0, true, &ch);
			ERRCHECK(result);
            
            ch->setChannelGroup(chgroup);
            
			freq = module[row][i].FREQUENCY;
            pan_track = tracks_list[i].PAN;
            vol_note = ((float)module[row][i].VOLUME / 64.0f);
            vol_track = ((float)tracks_list[i].VOLUME / 64.0f);
            vol_master = ((float)master_volume / 64.0f);
            vol_final = vol_note * vol_track * vol_master;
            
            
            ch->setVolume(vol_final);
            ch->setPan(pan_track);
            result = ch->setFrequency(freq);
			ERRCHECK(result);
            
            ch->setPaused(false);
		}
        
        else if (module[row][i].NAME == "= =")
        {
            chgroup->stop();
        }
	}
    
	return true;
}

bool PlayPattern(FMOD::System *fsystem, int start, int end, int track_count)
{
	FMOD_RESULT result;
    
	for (int i = start; i < end; ++i)
	{
        PlayRow(fsystem, i, track_count);
        
        double row_tick_delay = (60000.0f/(double)bpm/(double)rows_per_beat);
		
        future_tick = std::async(std::launch::async, RowTick, row_tick_delay);
		
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


bool PlayModule(FMOD::System *fsystem, int start, int end, int track_count)
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
        
		PlayPattern(fsystem, pat_start, pat_end, track_count);
        
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