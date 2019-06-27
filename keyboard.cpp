// get keyboard
if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
{
	if (ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_DownArrow]) && active_cell.ROW < pattern_rows - 1)
	{
		active_cell.LAST_CURSOR_ACTION = DOWN;
		active_cell.Y += UI.CELL_HEIGHT;
		active_cell.ROW++;
        
		if (selection_exists)
		{
			selection_exists = false;
		}
	}
	else if (ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_UpArrow]) && active_cell.ROW > 0)
	{
		active_cell.LAST_CURSOR_ACTION = UP;
		active_cell.Y -= UI.CELL_HEIGHT;
		active_cell.ROW--;
        
		if (selection_exists)
		{
			selection_exists = false;
		}
	}
	else if (ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_RightArrow]) && active_cell.COL < (tracks * 4) - 1)
	{
		active_cell.LAST_CURSOR_ACTION = RIGHT;
		active_cell.X += UI.CELL_WIDTH;
		active_cell.COL++;
        
		if (selection_exists)
		{
			selection_exists = false;
		}
	}
	else if (ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_LeftArrow]) && active_cell.COL > 0)
	{
		active_cell.LAST_CURSOR_ACTION = LEFT;
		active_cell.X -= UI.CELL_WIDTH;
		active_cell.COL--;
        
		if (selection_exists)
		{
			selection_exists = false;
		}
	}
    
	// note entry
	for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
	{
		if (ImGui::IsKeyReleased(i))
		{
			channelgroup->stop();
			key_pressed = false;
		}
        
        if (ImGui::IsKeyPressed(i) && !key_pressed)
		{
			key_pressed = true;
            
			if (active_cell.COL % 4 == 0) // note cell
			{
				std::string keychar = KeyToNote(i, octave);
                
				if (keychar != "invalid")
				{
					NOTE_DATA nd;
					nd.NAME = keychar;
                    
					float freq = NoteToFrequency(keychar);
					nd.FREQUENCY = freq;
                    
					PlayNote(fsystem, channel, sound, channelgroup, freq);
                    \
                        int cur_instr = module[active_cell.ROW + pattern_start][active_cell.COL/4].INSTRUMENT;
					int cur_vol = module[active_cell.ROW + pattern_start][active_cell.COL/4].VOLUME;
                    
					if (cur_instr == 0 && active_instrument > 0)
					{
						nd.INSTRUMENT = active_instrument;
					}
					else
					{
						nd.INSTRUMENT = cur_instr;
					}
					if (cur_vol == 0)
					{
						nd.VOLUME = 64;
					}
					else
					{
						nd.VOLUME = cur_vol;
					}
					CellSet(active_cell.ROW + pattern_start, active_cell.COL, nd, module);
                    
					// apply step to cursor
					if (pattern_start + active_cell.ROW + step < pattern_end)
					{
						active_cell.LAST_CURSOR_ACTION = DOWN;
						active_cell.ROW += step;
						active_cell.Y += step * UI.CELL_HEIGHT;
					}
				}
			}
			else if (active_cell.COL % 4 == 1) // instrument cell
			{
				int keychar = KeyToInstrument(i);
                
				if (keychar != -1)
				{
					int cur_value = module[active_cell.ROW + pattern_start][active_cell.COL/4].INSTRUMENT;
					int second_digit = (cur_value % 10);
					int new_value;
                    
					if (cur_value > 0)
					{
						new_value = (second_digit * 10) + keychar;
					}
					else if (cur_value == 0)
					{
						new_value = keychar;
					}
					
					NOTE_DATA nd;
					nd.INSTRUMENT = new_value;
					CellSet(active_cell.ROW + pattern_start, active_cell.COL, nd, module);
				}
			}
			else if (active_cell.COL % 4 == 2) // volume cell
			{
				int keychar = KeyToVolume(i);
                
				if (keychar != -1)
				{
					int cur_value = module[active_cell.ROW + pattern_start][active_cell.COL/4].VOLUME;
					int second_digit = (cur_value % 10);
					int new_value;
                    
					if (cur_value > 0)
					{
						new_value = (second_digit * 10) + keychar;
					}
					else if (cur_value == 0)
					{
						new_value = keychar;
					}
					
					if (new_value > 64) new_value = 64;
                    
					NOTE_DATA nd;
					nd.VOLUME = new_value;
					CellSet(active_cell.ROW + pattern_start, active_cell.COL, nd, module);
				}
			}
			else if (active_cell.COL % 4 == 1) // fx + param cell
			{
				//
			}
		}
	}
}
