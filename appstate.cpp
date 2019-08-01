if (application_state == PLAY_PATTERN)
{
	active_cell.ROW = 0;
	ImGui::SetScrollY(0);
	application_state = PLAYING;
    int track_count = tracks_list.size();
	future_play = std::async(std::launch::async, PlayPattern, pattern_start, pattern_end, track_count);
    if (application_state == END_PATTERN)
    {
        application_state = EDITOR;
    }
}

if (application_state == PLAY_MODULE)
{
	active_pattern = 0;
    active_cell.ROW = 0;
	ImGui::SetScrollY(0);
	application_state = PLAYING;
	int end = patterns_list.size();
    int track_count = tracks_list.size();
	future_play = std::async(std::launch::async, PlayModule, 0, end, track_count);
}

if (application_state == EDITOR)
{
	FMOD_RESULT result;
	result = channel->stop();
}

if (application_state == PLAYING)
{
	active_cell.LAST_CURSOR_ACTION = DOWN;
	active_cell.Y = 132 + (active_cell.ROW * UI.CELL_HEIGHT);
	if (active_cell.ROW > patterns_list[active_pattern].ROWS - 1)
	{
		ImGui::SetScrollY(0);
    }
    if (active_cell.ROW == 0)
    {
        ImGui::SetScrollY(0);
    }
}

if (application_state == END_PATTERN)
{
    for (int j = 0; j < tracks_list.size(); ++j)
    {
        tracks_list[j].CHANNELGROUP->stop();
    }
    
    if (future_play.get())
    {
        application_state = EDITOR;
    }
}
