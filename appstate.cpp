if (application_state == PLAY_PATTERN)
{
	active_cell.ROW = 0;
	ImGui::SetScrollY(0);
	application_state = PLAYING;
	future_play = std::async(std::launch::async, PlayPattern, module, system, channel,sound, channelgroup, pattern_start, pattern_end, tracks);
}

if (application_state == PLAY_MODULE)
{
	active_cell.ROW = 0;
	ImGui::SetScrollY(0);
	application_state = PLAYING;
	int end = patterns_list.size();
	future_play = std::async(std::launch::async, PlayModule, module, system, channel,sound, channelgroup, 0, end, tracks);
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
	if (active_cell.ROW >= patterns_list[active_pattern].ROWS - 1)
	{
		ImGui::SetScrollY(0);
		active_cell.ROW = 0;
		active_cell.Y = 132;
	}
}