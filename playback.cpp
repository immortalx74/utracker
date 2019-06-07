bool RowTick(int ms)
{
	LARGE_INTEGER start, stop, freq;
	QueryPerformanceCounter(&freq);
	QueryPerformanceCounter(&start);
	bool looping = true;
	do
	{
		QueryPerformanceCounter(&stop);

		if (10000000 * (stop.QuadPart - start.QuadPart) / freq.QuadPart >= ms)
		{
			looping = false;
			return 0;
			break;
		}
	}
	while (looping);
	return 0;
}

bool PlayRow(
	std::vector<std::vector<NOTE_DATA>> module,
	FMOD::System *system,
	FMOD::Channel *channel,
	FMOD::Sound *sound,
	int row,
	int track_count)
{
	FMOD_RESULT result;
	float f = 44100;
	for (int i = 0; i < track_count; ++i)
	{
		if (module[row][i].NAME != "---")
		{
			channel->stop();
			result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
			ERRCHECK(result);
		}
	}
	return true;
}

bool PlayPattern(
	std::vector<std::vector<NOTE_DATA>> module,
	FMOD::System *system,
	FMOD::Channel *channel,
	FMOD::Sound *sound,
	int start,
	int end,
	int track_count)
{
	for (int i = start; i < end; ++i)
	{
		if (module[i][0].NAME != "---")
		{
			PlayRow(module, system, channel, sound, 0, track_count);
		}

		globalfut2 = std::async(std::launch::async, RowTick, 10);
		playrow++;

		if (application_state == EDITOR)
		{
			return false;
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
		// std::future<int> fut = std::async(std::launch::async, RowTick, 10);
		globalfut2 = std::async(std::launch::async, RowTick, 10);
		playrow++;

		if (application_state == EDITOR)
		{
			return false;
		}
	}
	
	playrow = 0;
	return false;
}