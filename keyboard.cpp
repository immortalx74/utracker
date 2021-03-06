// get keyboard
int key;
bool mdfr;

// TEMP! DSP dom freq

//channelgroup->addDSP(0, dsp);
//if (ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_A]))
//{
//float val;
//dsp->setParameterInt(FMOD_DSP_FFT_WINDOWTYPE, FMOD_DSP_FFT_WINDOW_TRIANGLE);
//dsp->setParameterInt(FMOD_DSP_FFT_WINDOWSIZE, 4096);
//dsp->getParameterFloat(FMOD_DSP_FFT_DOMINANT_FREQ, &val, 0, 0);
//print(val);
//}


// delete
key = key_binding[Delete].KEY;
mdfr = GetModifiers(key_binding[Delete]);
if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr)
{
    NOTE_DATA nd = {"---", 0, 0, 0, -1, -1};
    if (selection_exists)
    {
        // delete data at selection
        int srow, scol, erow, ecol;
        
        if (selection.START_ROW > selection.END_ROW)
        {
            srow = selection.END_ROW;
            erow = selection.START_ROW;
        }
        else
        {
            srow = selection.START_ROW;
            erow = selection.END_ROW;
        }
        
        if (selection.START_COL > selection.END_COL)
        {
            scol = selection.END_COL;
            ecol = selection.START_COL;
        }
        else
        {
            scol = selection.START_COL;
            ecol = selection.END_COL;
        }
        
        for (int i = srow + pattern_start; i < erow + pattern_start + 1; ++i)
        {
            for (int j = scol; j <= ecol; ++j)
            {
                CellSet(i, j, nd, module);
            }
        }
    }
    else
    {
        // delete data at cursor
        CellSet(active_cell.ROW + pattern_start, active_cell.COL, nd, module);
    }
}

// select next/previous instrument
key = key_binding[NextInstrument].KEY;
mdfr = GetModifiers(key_binding[NextInstrument]);
if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr)
{
    if (active_instrument < instruments_list.size() - 1)
    {
        active_instrument++;
    }
}

key = key_binding[PreviousInstrument].KEY;
mdfr = GetModifiers(key_binding[PreviousInstrument]);
if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr)
{
    if (active_instrument > 0)
    {
        active_instrument--;
    }
}

// select next/previous pattern
key = key_binding[NextPattern].KEY;
mdfr = GetModifiers(key_binding[NextPattern]);
if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr)
{
    if (active_pattern < patterns_list.size() - 1)
    {
        active_pattern++;
    }
}

key = key_binding[PreviousPattern].KEY;
mdfr = GetModifiers(key_binding[PreviousPattern]);
if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr)
{
    if (active_pattern > 0)
    {
        active_pattern--;
    }
}

// set next/previous middle octave
key = key_binding[NextOctave].KEY;
mdfr = GetModifiers(key_binding[NextOctave]);
if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr)
{
    if (octave < 8)
    {
        octave++;
    }
}

key = key_binding[PreviousOctave].KEY;
mdfr = GetModifiers(key_binding[PreviousOctave]);
if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr)
{
    if (octave > 1)
    {
        octave--;
    }
}

// increase/decrease step
key = key_binding[IncreaseStep].KEY;
mdfr = GetModifiers(key_binding[IncreaseStep]);
if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr)
{
    if (step < 32)
    {
        step++;
    }
}

key = key_binding[DecreaseStep].KEY;
mdfr = GetModifiers(key_binding[DecreaseStep]);
if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr)
{
    if (step > 0)
    {
        step--;
    }
}


// clear selection
key = key_binding[ClearSelection].KEY;
mdfr = GetModifiers(key_binding[ClearSelection]);
if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr)
{
    if (selection_exists)
    {
        selection_exists = false;
    }
}

// relevant only when grid has focus
if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows) && application_state == EDITOR)
{
    // cut/copy/paste
    key = key_binding[Cut].KEY;
    mdfr = GetModifiers(key_binding[Cut]);
    if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr)
    {
        CopyToClipboard(selection.START_ROW, selection.START_COL, selection.END_ROW,
                        selection.END_COL, true);
    }
    
    key = key_binding[Copy].KEY;
    mdfr = GetModifiers(key_binding[Copy]);
    if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr)
    {
        CopyToClipboard(selection.START_ROW, selection.START_COL, selection.END_ROW,
                        selection.END_COL, false);
    }
    
    key = key_binding[Paste].KEY;
    mdfr = GetModifiers(key_binding[Paste]);
    if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr)
    {
        PasteFromClipboard();
    }
    
    
    // select all/select track
    key = key_binding[SelectAll].KEY;
    mdfr = GetModifiers(key_binding[SelectAll]);
    if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr)
    {
        selection_exists = true;
        
        selection.START_ROW = 0;
        selection.START_COL = 0;
        selection.END_ROW = patterns_list[active_pattern].ROWS - 1;
        selection.END_COL = (tracks_list.size() * 4) - 1;
    }
    
    key = key_binding[SelectTrack].KEY;
    mdfr = GetModifiers(key_binding[SelectTrack]);
    if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr)
    {
        selection_exists = true;
        
        selection.START_ROW = 0;
        selection.START_COL = (active_cell.COL / 4) * 4;
        selection.END_ROW = patterns_list[active_pattern].ROWS - 1;
        selection.END_COL = selection.START_COL + 3;
    }
    
    // arrows navigation
    key = key_binding[CursorDown].KEY;
    mdfr = GetModifiers(key_binding[CursorDown]);
    if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr && active_cell.ROW < pattern_rows - 1)
    {
        active_cell.LAST_CURSOR_ACTION = DOWN;
        active_cell.Y += UI.CELL_HEIGHT;
        active_cell.ROW++;
        
        if (selection_exists)
        {
            selection_exists = false;
        }
    }
    
    key = key_binding[CursorUp].KEY;
    mdfr = GetModifiers(key_binding[CursorUp]);
    if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr && active_cell.ROW > 0)
    {
        active_cell.LAST_CURSOR_ACTION = UP;
        active_cell.Y -= UI.CELL_HEIGHT;
        active_cell.ROW--;
        
        if (selection_exists)
        {
            selection_exists = false;
        }
    }
    
    key = key_binding[CursorRight].KEY;
    mdfr = GetModifiers(key_binding[CursorRight]);
    if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr && active_cell.COL < (tracks_list.size() * 4) - 1)
    {
        active_cell.LAST_CURSOR_ACTION = RIGHT;
        active_cell.X += UI.CELL_WIDTH;
        active_cell.COL++;
        
        if (selection_exists)
        {
            selection_exists = false;
        }
    }
    
    key = key_binding[CursorLeft].KEY;
    mdfr = GetModifiers(key_binding[CursorLeft]);
    if (ImGui::IsKeyPressed(io.KeyMap[key]) && mdfr && active_cell.COL > 0)
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
            is_playing = false;
        }
        
        if (ImGui::IsKeyPressed(i)&& !key_pressed && !is_playing)
        {
            key_pressed = true;
            
            if (active_cell.COL % 4 == 0) // note cell
            {
                std::string keychar = KeyToNote(i, octave);
                NOTE_DATA nd;
                nd.NAME = keychar;
                
                if (keychar == "= =")
                {
                    nd.NAME = "= =";
                    nd.FREQUENCY = 0;
                    nd.INSTRUMENT = 0;
                    nd.VOLUME = 0;
                    nd.FX = -1;
                    nd.FX_PARAM = -1;
                    
                    CellSet(active_cell.ROW + pattern_start, active_cell.COL, nd, module);
                }
                else if (keychar != "invalid" && keychar !="= =")
                {
                    float freq = NoteToFrequency(keychar);
                    nd.FREQUENCY = freq;
                    
                    if (active_instrument >0 && samples_list.size() > 0 && key_pressed)
                    {
                        int sample_index = NoteToSample(keychar, active_instrument);
                        FMOD::Sound *s = samples_list[sample_index].SOUND;
                        
                        if (channel->isPlaying(&is_playing))
                        {
                            //key_pressed = false;
                            //is_playing = true;
                        }
                        
                        PlayNote(channel, s, channelgroup, freq);
                    }
                    
                    int cur_vol = module[active_cell.ROW + pattern_start][active_cell.COL/4].VOLUME;
                    
                    nd.INSTRUMENT = active_instrument;
                    
                    if (cur_vol == 0)
                    {
                        nd.VOLUME = 64;
                    }
                    else
                    {
                        nd.VOLUME = cur_vol;
                    }
                    CellSet(active_cell.ROW + pattern_start, active_cell.COL, nd, module);
                }
                
                // apply step to cursor
                if (keychar != "invalid")
                {
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
