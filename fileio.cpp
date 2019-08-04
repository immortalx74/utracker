// HEADER
// char[14] - format identifier (14 Bytes)
// int version (4 Bytes)

// MODULE SETTINGS
// int - BPM (4 Bytes)
// int - rows/beat (4 Bytes)
// int - master volume (4 Bytes)

// TRACK DATA
// int - tracks_list size (4 Bytes)
// TRACK - track data (size X 32 Bytes)

// PATTERN DATA
// int - patterns_list size (4 Bytes)
// PATTERN - pattern data (size X 16 Bytes)

// INSTRUMENT DATA NOTE: *CHANNELGROUP pointers discarded
// int - instruments_list size length (4 Bytes)
// int - length of NAME (4 bytes)
// char[length of NAME] - NAME (variable size)
// std::array[120] - SAMPLE_MAP (480 Bytes) NOTE:needs sample-map compression
// std::array[120] - TEMP_MAP (480 Bytes) NOTE:needs sample-map compression

// SAMPLE DATA NOTE: *SOUND pointers discarded 
// int - samples_list size (4 Bytes)
// int - loop type (4 bytes)
// int - length of NAME (4 bytes)
// char[length of NAME] - NAME (variable size)
// int - length of FILENAME (4 bytes)
// char[length of FILENAME] - FILENAME (variable size)

// NOTE CONTENT
// int - records (4 Bytes) 
// int - note row (4 Bytes)
// int - note track (4 Bytes)
// NOTE_DATA - note data (records X 56 Bytes)

void WriteModule(const char* fname)
{
    using namespace std;
    
    ofstream outfile;
    outfile.open (fname, ios::out | ios::binary);
    
    // HEADER
    char *formatid = "uTrackerModule";
    outfile.write (formatid, 14);
    int version = 1;
    outfile.write((char *)&version, sizeof(version));
    
    // MODULE SETTINGS
    outfile.write((char *)&bpm, sizeof(bpm));
    outfile.write((char *)&rows_per_beat, sizeof(rows_per_beat));
    outfile.write((char *)&master_volume, sizeof(master_volume));
    
    // TRACK DATA
    int track_count = tracks_list.size();
    outfile.write((char *)&track_count, sizeof(track_count));
    
    for (int i = 0; i < tracks_list.size(); ++i)
    {
        outfile.write((char *)&tracks_list[i], sizeof(tracks_list[i]));
    }
    
    // PATTERN DATA
    int pattern_count = patterns_list.size();
    outfile.write((char *)&pattern_count, sizeof(pattern_count));
    
    for (int i = 0; i < patterns_list.size(); ++i)
    {
        outfile.write((char *)&patterns_list[i], sizeof(patterns_list[i]));
    }
    
    // INSTRUMENT DATA
    int instrument_count = instruments_list.size();
    outfile.write((char *)&instrument_count, sizeof(instrument_count));
    
    for (int i = 0; i < instruments_list.size(); ++i)
    {
        const char *n = instruments_list[i].NAME.c_str();
        int n_length = strlen(n);
        
        outfile.write((char *)&n_length, sizeof(int)); // 4 bytes
        outfile.write(n, n_length);
        
        std::array<int,120> s_map = instruments_list[i].SAMPLE_MAP;
        std::array<int,120> t_map = instruments_list[i].TEMP_MAP;
        
        outfile.write((char *)&s_map, sizeof(s_map));
        outfile.write((char *)&t_map, sizeof(t_map));
    }
    
    // SAMPLE DATA
    int sample_count = samples_list.size();
    outfile.write((char *)&sample_count, sizeof(sample_count));
    
    for (int i = 0; i < samples_list.size(); ++i)
    {
        int loop_type = samples_list[i].LOOP_TYPE;
        
        const char *n = samples_list[i].NAME.c_str();
        const char *fn = samples_list[i].FILENAME.c_str();
        
        outfile.write((char *)&loop_type, sizeof(loop_type));
        
        int n_length = strlen(n);
        int fn_length = strlen(fn);
        
        outfile.write((char *)&n_length, sizeof(int)); // 4 bytes
        outfile.write(n, n_length);
        
        outfile.write((char *)&fn_length, sizeof(int)); // 4 bytes
        outfile.write(fn, fn_length);
    }
    
    // NOTE CONTENT
    NOTE_DATA track_row_data;
    int pos = outfile.tellp();
    int records = 0;
    outfile.write((char *)&records, sizeof(records));
    
    for (int i = 0; i < module.size(); ++i)
    {
        for (int j = 0; j < tracks_list.size(); ++j)
        {
            track_row_data.NAME = module[i][j].NAME;
            track_row_data.FREQUENCY = module[i][j].FREQUENCY;
            track_row_data.INSTRUMENT = module[i][j].INSTRUMENT;
            track_row_data.VOLUME = module[i][j].VOLUME;
            track_row_data.FX = module[i][j].FX;
            track_row_data.FX_PARAM = module[i][j].FX_PARAM;
            
            if (module[i][j].NAME != "---" && module[i][j].VOLUME > 0)
            {
                outfile.write((char *)&i, sizeof(i));
                outfile.write((char *)&j, sizeof(j));
                outfile.write((char *)&track_row_data, sizeof(track_row_data));
                
                records++;
            }
        }
    }
    
    outfile.seekp(pos); // jump back and write record count
    outfile.write((char *)&records, sizeof(records));
    
    outfile.close();
}

bool ReadModule(const char* fname)
{
    using namespace std;
    
    ifstream infile;
    infile.open(fname, ios::in | ios::binary);
    
    //HEADER
    char formatid[14];
    infile.read(formatid, 14);
    
    std::string fid = formatid;
    if (fid != "uTrackerModule")
    {
        print ("ERROR!");
        return false;
    }
    
    int version = 0;
    infile.read((char *)&version, sizeof(version));
    if (version != 1)
    {
        print ("ERROR!");
        return false;
    }
    
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
    
    // MODULE SETTINGS
    infile.read((char *)&bpm, sizeof(bpm));
    infile.read((char *)&rows_per_beat, sizeof(rows_per_beat));
    infile.read((char *)&master_volume, sizeof(master_volume));
    
    // TRACK DATA
    int track_count = 0;
    infile.read((char *)&track_count, sizeof(track_count));
    
    for (int i = 0; i < track_count; ++i)
    {
        CreateTrack(tracks_list, tracks_list.size(), true);
        infile.read((char *)&tracks_list[i], sizeof(tracks_list[i]));
        
        FMOD::ChannelGroup *new_channel_group;
        fsystem->createChannelGroup(0, &new_channel_group);
        
        tracks_list[i].CHANNELGROUP = new_channel_group;
    }
    
    // PATTERN DATA
    int pattern_count = 0;
    infile.read((char *)&pattern_count, sizeof(pattern_count));
    
    for (int i = 0; i < pattern_count; ++i)
    {
        CreatePattern(patterns_list, 64, module);
        infile.read((char *)&patterns_list[i], sizeof(patterns_list[i]));
    }
    
    // INSTRUMENT DATA
    int instrument_count = 0;
    infile.read((char *)&instrument_count, sizeof(instrument_count));
    
    for (int i = 0; i < instrument_count; ++i)
    {
        //infile.read((char *)&instruments_list[i], sizeof(instruments_list[i]));
        int n_size;
        char *n;
        
        INSTRUMENT new_instrument;
        instruments_list.push_back(new_instrument);
        
        infile.read((char *)&n_size, sizeof(n_size));
        n = new char[n_size + 1];
        infile.read(n, n_size);
        n[n_size] = 0;
        free(n);
        
        std::array<int,120> s_map;
        std::array<int,120> t_map;
        
        infile.read((char *)&s_map, sizeof(s_map));
        infile.read((char *)&t_map, sizeof(t_map));
        
        instruments_list[i].NAME = n;
        instruments_list[i].SAMPLE_MAP = s_map;
        instruments_list[i].TEMP_MAP = t_map;
    }
    
    // SAMPLE DATA
    int sample_count = 0;
    infile.read((char *)&sample_count, sizeof(sample_count));
    
    for (int i = 0; i < sample_count; ++i)
    {
        int loop_type;
        int n_size;
        char *n;
        int fn_size;
        char *fn;
        
        SAMPLE new_sample;
        samples_list.push_back(new_sample);
        
        infile.read((char *)&loop_type, sizeof(loop_type));
        
        infile.read((char *)&n_size, sizeof(n_size));
        n = new char[n_size + 1];
        infile.read(n, n_size);
        n[n_size] = 0;
        
        infile.read((char *)&fn_size, sizeof(fn_size));
        fn = new char[fn_size + 1];
        infile.read(fn, fn_size);
        fn[fn_size] = 0;
        
        FMOD::Sound *snd;
        FMOD_RESULT result;
        result = fsystem->createSound(fn, FMOD_CREATESAMPLE || FMOD_LOOP_NORMAL, 0, &snd);
        
        samples_list[i].LOOP_TYPE = loop_type;
        samples_list[i].NAME = n;
        samples_list[i].FILENAME = fn;
        samples_list[i].SOUND = snd;
        
        free(n);
        free(fn);
    }
    
    
    // NOTE CONTENT
    NOTE_DATA track_row_data;
    int row = 0;
    int track = 0;
    int records = 0;
    infile.read((char *)&records, sizeof(records));
    
    for (int i = 0; i < records; ++i)
    {
        infile.read((char *)&row, sizeof(row));
        infile.read((char *)&track, sizeof(track));
        infile.read((char *)&track_row_data, sizeof(track_row_data));
        
        module[row][track].NAME = track_row_data.NAME;
        module[row][track].FREQUENCY = track_row_data.FREQUENCY;
        module[row][track].INSTRUMENT = track_row_data.INSTRUMENT;
        module[row][track].VOLUME = track_row_data.VOLUME;
        module[row][track].FX = track_row_data.FX;
        module[row][track].FX_PARAM = track_row_data.FX_PARAM;
    }
    
    infile.close();
    
    return true;
}