
int app_window_height = io.DisplaySize.y;
ImGui::SetNextWindowSize(ImVec2(UI.LEFT_PANE_WIDTH, app_window_height));
ImGui::SetNextWindowPos(ImVec2(UI.LEFT_PANE_X + UI.MARGIN, UI.LEFT_PANE_Y));
ImGui::PushStyleColor(ImGuiCol_WindowBg, color_schemes[active_color_scheme].DATA[PanelBackground].COLOR_VALUE);
ImGui::Begin("LeftPane", false,ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

// patterns list
ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[HeadingText].COLOR_VALUE);

ImGui::Text("Patterns");
ImGui::PopStyleColor();

UI.PATTERNS_LIST_X = ImGui::GetCursorPosX();
UI.PATTERNS_LIST_Y = ImGui::GetCursorPosY();

ImGui::PushStyleColor(ImGuiCol_FrameBg, color_schemes[active_color_scheme].DATA[FrameBackground].COLOR_VALUE);
ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);

if (ImGui::ListBoxHeader("##patlist", ImVec2(UI.PATTERNS_LIST_WIDTH, UI.PATTERNS_LIST_HEIGHT)))
{   
	std::string name;
    
    for (int i = 0; i < patterns_list.size(); ++i)
	{
		if (i <= 9) 
        {
            name = "0" + std::to_string(i) + ":" + patterns_list[i].NAME;
        }
        else
        {
            name = std::to_string(i) + ":" + patterns_list[i].NAME;
        }
        
        if (ImGui::Selectable(name.c_str(), active_pattern == i))
		{
			active_pattern = i;
        }
	}
}
ImGui::ListBoxFooter();
ImGui::PopStyleColor();
ImGui::PopStyleColor();

// Instruments list 
ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[HeadingText].COLOR_VALUE);
ImGui::Text("Instruments");
ImGui::PopStyleColor();

UI.INSTRUMENTS_LIST_X = ImGui::GetCursorPosX();
UI.INSTRUMENTS_LIST_Y = ImGui::GetCursorPosY();


ImGui::PushStyleColor(ImGuiCol_FrameBg, color_schemes[active_color_scheme].DATA[FrameBackground].COLOR_VALUE);
ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);
if (ImGui::ListBoxHeader("##inslist", ImVec2(UI.INSTRUMENTS_LIST_WIDTH, UI.INSTRUMENTS_LIST_HEIGHT)))
{   
	std::string name;
    
    for (int i = 0; i < instruments_list.size(); ++i)
	{
		if (i <= 9) 
        {
            name = "0" + std::to_string(i) + ":" + instruments_list[i].NAME;
        }
        else
        {
            name = std::to_string(i) + ":" + instruments_list[i].NAME;
        }
        
        if (ImGui::Selectable(name.c_str(), active_instrument == i))
		{
			active_instrument = i;
		}
	}
}

ImGui::ListBoxFooter();
ImGui::PopStyleColor();
ImGui::PopStyleColor();

// Samples list 
ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[HeadingText].COLOR_VALUE);
ImGui::Text("Samples");
ImGui::PopStyleColor();

UI.SAMPLES_LIST_X = ImGui::GetCursorPosX();
UI.SAMPLES_LIST_Y = ImGui::GetCursorPosY();

ImGui::PushStyleColor(ImGuiCol_FrameBg, color_schemes[active_color_scheme].DATA[FrameBackground].COLOR_VALUE);
ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);
if (ImGui::ListBoxHeader("##samlist", ImVec2(UI.SAMPLES_LIST_WIDTH, UI.SAMPLES_LIST_HEIGHT)))
{   
	std::string name;
    
    for (int i = 0; i < samples_list.size(); ++i)
	{
		if (i <= 9) 
        {
            name = "0" + std::to_string(i) + ":" + samples_list[i].NAME;
        }
        else
        {
            name = std::to_string(i) + ":" + samples_list[i].NAME;
        }
        
        if (ImGui::Selectable(name.c_str(), active_sample == i))
        {
			active_sample = i;
		}
        
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(samples_list[i].FILENAME.c_str());
		}
	}
}

ImGui::ListBoxFooter();
ImGui::PopStyleColor();
ImGui::PopStyleColor();


// Draw sliders
ImGui::PushItemWidth(UI.LEFT_SLIDERS_WIDTH);
ImGui::SetCursorPosY(ImGui::GetCursorPosY() + UI.MARGIN);

ImGui::PushStyleColor(ImGuiCol_FrameBg, color_schemes[active_color_scheme].DATA[FrameBackground].COLOR_VALUE);
ImGui::PushStyleColor(ImGuiCol_Text, color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE);
ImGui::SliderInt("Middle octave", &octave, 1, 8);
ImGui::SliderInt("BPM", &bpm, 32, 512);
ImGui::SliderInt("Rows/Beat", &rows_per_beat, 1, 32);
ImGui::SliderInt("Step", &step, 0, 32);
ImGui::SliderInt("Master volume", &master_volume, 0, 64);
ImGui::PopStyleColor();
ImGui::PopStyleColor();

ImGui::PopItemWidth();

// show debug info
ImGui::Spacing();
//ImGui::Text("mouse_x:");ImGui::SameLine();ImGui::Text(std::to_string(ImGui::GetMousePos().x).c_str());
//ImGui::Text("mouse_y:");ImGui::SameLine();ImGui::Text(std::to_string(ImGui::GetMousePos().y).c_str());

//ImGui::Text("cell_x:");ImGui::SameLine();ImGui::Text(std::to_string(active_cell.X).c_str());
//ImGui::Text("cell_y:");ImGui::SameLine();ImGui::Text(std::to_string(active_cell.Y).c_str());
ImGui::Text("cell_row:");ImGui::SameLine();ImGui::Text(std::to_string(active_cell.ROW).c_str());
//ImGui::Text("cell_col:");ImGui::SameLine();ImGui::Text(std::to_string(active_cell.COL).c_str());
//
//ImGui::Text("start row:");ImGui::SameLine();ImGui::Text(std::to_string(selection.START_ROW).c_str());
//ImGui::Text("start col:");ImGui::SameLine();ImGui::Text(std::to_string(selection.START_COL).c_str());
//ImGui::Text("end row:");ImGui::SameLine();ImGui::Text(std::to_string(selection.END_ROW).c_str());
//ImGui::Text("end col:");ImGui::SameLine();ImGui::Text(std::to_string(selection.END_COL).c_str());

// show app state
ImGui::Spacing();
std::string as;
if (application_state == 0) as ="PLAY_MODULE";
if (application_state == 1) as ="PLAY_PATTERN";
if (application_state == 2) as ="END_PATTERN";
if (application_state == 3) as ="PLAYING";
if (application_state == 4) as ="EDITOR";

ImGui::Text("state:");ImGui::SameLine();ImGui::Text(as.c_str());

ImGui::Text(std::to_string(active_pattern).c_str());

//=======================
// Draw PATTERN_ buttons

ImGui::SetCursorPos(ImVec2(UI.PATTERNS_LIST_X + UI.PATTERNS_LIST_WIDTH + (2 * UI.MARGIN), UI.PATTERNS_LIST_Y));

ImGui::PushID("pattern_add");
if (ImGui::ImageButton(buttons[1], ImVec2(16, 16), 0, sf::Color::Transparent,
                       color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE))
{
    CreatePattern(patterns_list, 64, module);
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Add Pattern");
}
ImGui::PopID();

ImGui::SetCursorPosX(UI.PATTERNS_LIST_X + UI.PATTERNS_LIST_WIDTH + (2 * UI.MARGIN));

ImGui::PushID("pattern_del");
if (ImGui::ImageButton(buttons[3],ImVec2(16, 16), 0,sf::Color::Transparent,
                       color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE))
{
    DeletePattern(patterns_list, module);
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Delete Pattern");
}
ImGui::PopID();


ImGui::SetCursorPosX(UI.PATTERNS_LIST_X + UI.PATTERNS_LIST_WIDTH + (2 * UI.MARGIN));

ImGui::PushID("pattern_up");
if (ImGui::ImageButton(buttons[4], ImVec2(16, 16), 0, sf::Color::Transparent,
                       color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE))
{
    //
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Move Pattern up");
}
ImGui::PopID();


ImGui::SetCursorPosX(UI.PATTERNS_LIST_X + UI.PATTERNS_LIST_WIDTH + (2 * UI.MARGIN));

ImGui::PushID("pattern_down");
if (ImGui::ImageButton(buttons[0], ImVec2(16, 16), 0, sf::Color::Transparent,
                       color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE))
{
    //
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Move Pattern down");
}
ImGui::PopID();


ImGui::SetCursorPosX(UI.PATTERNS_LIST_X + UI.PATTERNS_LIST_WIDTH + (2 * UI.MARGIN));

ImGui::PushID("pattern_options");
if (ImGui::ImageButton(buttons[5], ImVec2(16, 16), 0, sf::Color::Transparent,
                       color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE))
{
    ImGui::OpenPopup("Pattern Options");
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Open Pattern Options");
}

bool p_opened = true;

#include "pattern_options.cpp"

ImGui::PopID();

// Draw Instrument buttons
ImGui::SetCursorPos(ImVec2(UI.INSTRUMENTS_LIST_X + UI.INSTRUMENTS_LIST_WIDTH + (2 * UI.MARGIN), UI.INSTRUMENTS_LIST_Y));

ImGui::PushID("instrument_add");
if (ImGui::ImageButton(buttons[1], ImVec2(16, 16), 0, sf::Color::Transparent,
                       color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE))
{
    CreateInstrument(instruments_list);
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Add Instrument");
}
ImGui::PopID();


ImGui::SetCursorPosX(UI.INSTRUMENTS_LIST_X + UI.INSTRUMENTS_LIST_WIDTH + (2 * UI.MARGIN));

ImGui::PushID("instrument_del");
if (ImGui::ImageButton(buttons[3], ImVec2(16, 16), 0, sf::Color::Transparent,
                       color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE))
{
    DeleteInstrument();
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Delete Instrument");
}
ImGui::PopID();


ImGui::SetCursorPosX(UI.INSTRUMENTS_LIST_X + UI.INSTRUMENTS_LIST_WIDTH + (2 * UI.MARGIN));

ImGui::PushID("instrument_up");
if (ImGui::ImageButton(buttons[4], ImVec2(16, 16), 0, sf::Color::Transparent,
                       color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE))
{
    //
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Move instrument up");
}
ImGui::PopID();


ImGui::SetCursorPosX(UI.INSTRUMENTS_LIST_X + UI.INSTRUMENTS_LIST_WIDTH + (2 * UI.MARGIN));

ImGui::PushID("instrument_down");
if (ImGui::ImageButton(buttons[0], ImVec2(16, 16), 0, sf::Color::Transparent,
                       color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE))
{
    //
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Move instrument down");
}
ImGui::PopID();


ImGui::SetCursorPosX(UI.INSTRUMENTS_LIST_X + UI.INSTRUMENTS_LIST_WIDTH + (2 * UI.MARGIN));

ImGui::PushID("instrument_options");
if (ImGui::ImageButton(buttons[5], ImVec2(16, 16), 0, sf::Color::Transparent,
                       color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE))
{
    ImGui::OpenPopup("Instrument options");
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Open instrument options");
}

// prevent instrument options from opening when "No Instrument" is selected
if (active_instrument <= 0)
{
    p_opened = false;
}

#include "instrument_options.cpp"
ImGui::PopID();


// Draw Sample buttons
ImGui::SetCursorPos(ImVec2(UI.SAMPLES_LIST_X + UI.SAMPLES_LIST_WIDTH + (2 * UI.MARGIN), UI.SAMPLES_LIST_Y));

ImGui::PushID("sample_add");
if (ImGui::ImageButton(buttons[1], ImVec2(16, 16), 0, sf::Color::Transparent,
                       color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE))
{
    auto f = pfd::open_file("Load sample(s)", "/tmp/",{ "Wave Files (.wav)", "*.wav"},true);
    
    int filecount = 0;
    for (auto const &fname : f.result())
    {
        filecount++;
        
        if (samples_list.size() + filecount <= MAX_SAMPLES_PER_MODULE)
        {
            LoadSample(samples_list, fname, fsystem);
        }
    }
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Add Sample");
}
ImGui::PopID();


ImGui::SetCursorPosX(UI.SAMPLES_LIST_X + UI.SAMPLES_LIST_WIDTH + (2 * UI.MARGIN));

ImGui::PushID("sample_instrument_add");
if (ImGui::ImageButton(buttons[2], ImVec2(16, 16), 0, sf::Color::Transparent,
                       color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE))
{
    auto f = pfd::open_file("Load sample(s)", "/tmp/",{ "Wave Files (.wav)", "*.wav"},true);
    
    int filecount = 0;
    for (auto const &fname : f.result())
    {
        filecount++;
        
        if (samples_list.size() + filecount <= MAX_SAMPLES_PER_MODULE)
        {
            LoadSample(samples_list, fname, fsystem);
            CreateInstrument(instruments_list);
            
            for (int i = 0; i < 120; ++i)
            {
                instruments_list[active_instrument].SAMPLE_MAP[i] = active_sample;
                instruments_list[active_instrument].NAME = samples_list[active_sample].NAME;
            }
        }
    }
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Add Sample/Instrument pair");
}
ImGui::PopID();


ImGui::SetCursorPosX(UI.SAMPLES_LIST_X + UI.SAMPLES_LIST_WIDTH + (2 * UI.MARGIN));

ImGui::PushID("sample_del");
if (ImGui::ImageButton(buttons[3], ImVec2(16, 16), 0, sf::Color::Transparent,
                       color_schemes[active_color_scheme].DATA[Text].COLOR_VALUE))
{
    DeleteSample();
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Delete Sample");
}
ImGui::PopID();


ImGui::End();
ImGui::PopStyleColor();