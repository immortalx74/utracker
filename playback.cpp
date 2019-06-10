typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

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

void PlayNote(
	FMOD::System *system,
	FMOD::Channel *channel,
	FMOD::Sound *sound,
	float frequency)
{
	FMOD_RESULT result;

	channel->stop();
	result = system->playSound(FMOD_CHANNEL_REUSE, sound, false, &channel);
	ERRCHECK(result);

	result = channel->setFrequency(frequency);
	ERRCHECK(result);
}

bool PlayRow(
	std::vector<std::vector<NOTE_DATA>> &module,
	FMOD::System *system,
	FMOD::Channel **channel,
	FMOD::Sound *sound,
	int row,
	int track_count)
{
	FMOD_RESULT result;
	float freq;

	for (int i = 0; i < track_count; ++i)
	{
		if (module[row][i].NAME != "---")
		{
			result = system->playSound(FMOD_CHANNEL_FREE, sound, false, channel);
			ERRCHECK(result);

			freq = module[row][i].FREQUENCY;
			result = (*channel)->setFrequency(freq);
			ERRCHECK(result);
		}
	}
	return true;
}

bool PlayPattern(
	std::vector<std::vector<NOTE_DATA>> &module,
	FMOD::System *system,
	FMOD::Channel *channel,
	FMOD::Sound *sound,
	int start,
	int end,
	int track_count)
{
	FMOD_RESULT result;
	float freq;
	int chans_playing = 0;

	for (int i = start; i < end; ++i)
	{
		if (module[i][0].NAME != "---")
		{
			PlayRow(module, system, &channel, sound, i, track_count);
		}

		future_tick = std::async(std::launch::async, RowTick, 100);
		playrow++;

		if (application_state == EDITOR)
		{
			channel->stop();
			return true;

		}
	}

	return true;
}


bool PlayModule(std::vector<std::vector<NOTE_DATA>> module, FMOD::System *system, FMOD::Channel *channel, FMOD::Sound *sound)
{
	FMOD_RESULT result;

	for (int i = 0; i < 60; ++i)
	{
		if (module[i][0].NAME != "---")
		{
			channel->stop();
			result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
			ERRCHECK(result);
		}

		future_tick = std::async(std::launch::async, RowTick, 10);
		playrow++;

		if (application_state == EDITOR)
		{
			return false;
		}
	}
	
	playrow = 0;
	return false;
}
