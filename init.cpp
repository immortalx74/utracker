FMOD::System     *system;
FMOD::Sound      *sound;
FMOD::Channel    *channel = 0;
FMOD::ChannelGroup *channelgroup;
FMOD_RESULT       result;

result = FMOD::System_Create(&system);
ERRCHECK(result);

// system->setOutput(FMOD_OUTPUTTYPE_ASIO);
system->setOutput(FMOD_OUTPUTTYPE_WASAPI);

result = system->init(500, FMOD_INIT_NORMAL, 0);
ERRCHECK(result);

result = system->createSound("res/pianoc5.wav", FMOD_SOFTWARE, 0, &sound);
ERRCHECK(result);

result = sound->setMode(FMOD_LOOP_OFF);
ERRCHECK(result);

result = system->createChannelGroup("mychannels", &channelgroup);
ERRCHECK(result);

// create default tracks
for (int i = 0; i < tracks; ++i)
{
    CreateTrack(tracks_list);
}

CreatePattern(patterns_list, 64, module); // create default PATTERN_
CreateInstrument(instruments_list); // create default instrument (serves as "no instrument" equivalent of MPT)

active_cell.X = 306;
active_cell.Y = 132;
active_cell.ROW = 0;
active_cell.COL = 0;

SELECTION selection;
selection.START_X = 0;
selection.START_Y = 0;
selection.END_X = 0;
selection.END_Y = 0;

float grid_scroll_x = 0.0f;
float grid_scroll_y = 0.0f;

float mousex = 0;
float mousey = 0;
float winx = 0;
float winy = 0;
bool key_pressed = false;