
int app_window_height = io.DisplaySize.y;
ImGui::SetNextWindowSize(ImVec2(UI.LEFT_PANE_WIDTH, app_window_height));
ImGui::SetNextWindowPos(ImVec2(UI.LEFT_PANE_X + UI.MARGIN, UI.LEFT_PANE_Y));
ImGui::Begin("LeftPane", false,ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

// patterns list
ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
ImGui::Text("Patterns");
ImGui::PopStyleColor();

UI.PATTERNS_LIST_X = ImGui::GetCursorPosX();
UI.PATTERNS_LIST_Y = ImGui::GetCursorPosY();

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

// Instruments list 
ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
ImGui::Text("Instruments");
ImGui::PopStyleColor();

UI.INSTRUMENTS_LIST_X = ImGui::GetCursorPosX();
UI.INSTRUMENTS_LIST_Y = ImGui::GetCursorPosY();

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

// Samples list 
ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
ImGui::Text("Samples");
ImGui::PopStyleColor();

UI.SAMPLES_LIST_X = ImGui::GetCursorPosX();
UI.SAMPLES_LIST_Y = ImGui::GetCursorPosY();

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



// Draw sliders
ImGui::PushItemWidth(UI.LEFT_SLIDERS_WIDTH);
ImGui::SetCursorPosY(ImGui::GetCursorPosY() + UI.MARGIN);
ImGui::SliderInt("Middle octave", &octave, 1, 8);
ImGui::SliderInt("BPM", &bpm, 32, 512);
ImGui::SliderInt("Ticks/Row", &ticks_per_row, 1, 32);
ImGui::SliderInt("Step", &step, 0, 32);
ImGui::SliderInt("Master volume", &master_volume, 0, 64);

ImGui::PopItemWidth();

// show debug info
ImGui::Spacing();
ImGui::Text("mouse_x:");ImGui::SameLine();ImGui::Text(std::to_string(ImGui::GetMousePos().x).c_str());
ImGui::Text("mouse_y:");ImGui::SameLine();ImGui::Text(std::to_string(ImGui::GetMousePos().y).c_str());

ImGui::Text("cell_x:");ImGui::SameLine();ImGui::Text(std::to_string(active_cell.X).c_str());
ImGui::Text("cell_y:");ImGui::SameLine();ImGui::Text(std::to_string(active_cell.Y).c_str());
ImGui::Text("cell_row:");ImGui::SameLine();ImGui::Text(std::to_string(active_cell.ROW).c_str());
ImGui::Text("cell_col:");ImGui::SameLine();ImGui::Text(std::to_string(active_cell.COL).c_str());

// show app state
ImGui::Spacing();
std::string as;
if (application_state == 0) as ="PLAY_MODULE";
if (application_state == 1) as ="PLAY_PATTERN";
if (application_state == 2) as ="END_PATTERN";
if (application_state == 3) as ="PLAYING";
if (application_state == 4) as ="EDITOR";

ImGui::Text("state:");ImGui::SameLine();ImGui::Text(as.c_str());

//=======================
// Draw PATTERN_ buttons
ImGui::SetCursorPos(ImVec2(UI.PATTERNS_LIST_X + UI.PATTERNS_LIST_WIDTH + UI.MARGIN, UI.PATTERNS_LIST_Y));
if (ImGui::Button("+##patternadd"))
{
    CreatePattern(patterns_list, 64, module);
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Add Pattern");
}


ImGui::SetCursorPosX(UI.PATTERNS_LIST_X + UI.PATTERNS_LIST_WIDTH + UI.MARGIN);
if (ImGui::Button("-##patterndel"))
{
    DeletePattern(patterns_list, module);
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Delete Pattern");
}

ImGui::SetCursorPosX(UI.PATTERNS_LIST_X + UI.PATTERNS_LIST_WIDTH + UI.MARGIN);
if (ImGui::Button("^##patternup"))
{
    //
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Move Pattern up");
}

ImGui::SetCursorPosX(UI.PATTERNS_LIST_X + UI.PATTERNS_LIST_WIDTH + UI.MARGIN);
if (ImGui::Button("v##patterndown"))
{
    //
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Move Pattern down");
}

ImGui::SetCursorPosX(UI.PATTERNS_LIST_X + UI.PATTERNS_LIST_WIDTH + UI.MARGIN);
if (ImGui::Button("o##patternedit"))
{
    ImGui::OpenPopup("Pattern Options");
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Open Pattern Options");
}

ImGui::SetNextWindowSize(ImVec2(UI.PATTERN_OPTIONS_MODAL_WIDTH, UI.PATTERN_OPTIONS_MODAL_HEIGHT));

bool p_opened = true;


if (ImGui::BeginPopupModal("Pattern Options", &p_opened, ImGuiWindowFlags_NoResize))
{
    static char current_pattern_name[24] = "";
    static int r;
    static bool enable_write = false;
    
    if (!enable_write)
    {
        r = patterns_list[active_pattern].ROWS;
        strcpy(current_pattern_name, patterns_list[active_pattern].NAME.c_str());
        enable_write = true;
    }
    
    ImGui::PushItemWidth(200);
    ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
    ImGui::Text("Number of rows");
    ImGui::SliderInt("##numofrows", &r, 1, 64);
    ImGui::PopStyleColor();
    
    ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
    ImGui::Text("Pattern name");
    ImGui::PopStyleColor();
    
    if (ImGui::InputText("##patternname", current_pattern_name, 24, ImGuiInputTextFlags_CharsNoBlank))
    {
        
    }
    
    ImGui::PopItemWidth();
    
    float modal_width = ImGui::GetWindowSize().x;
    float modal_height = ImGui::GetWindowSize().y;
    ImGui::SetCursorPos(ImVec2(modal_width- 180, modal_height - 30));
    
    if (ImGui::Button("OK", ImVec2(80,0)))
    {
        enable_write = false;
        
        if (strlen(current_pattern_name) != 0)
        {
            patterns_list[active_pattern].NAME = current_pattern_name;
        }
        
        patterns_list[active_pattern].ROWS = r;
        ImGui::CloseCurrentPopup();
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(80,0)))
    {
        enable_write = false;
        ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
}


// Draw Instrument buttons
ImGui::SetCursorPos(ImVec2(UI.INSTRUMENTS_LIST_X + UI.INSTRUMENTS_LIST_WIDTH + UI.MARGIN, UI.INSTRUMENTS_LIST_Y));
if (ImGui::Button("+##instrument_add"))
{
    CreateInstrument(instruments_list);
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Add Instrument");
}

ImGui::SetCursorPosX(UI.INSTRUMENTS_LIST_X + UI.INSTRUMENTS_LIST_WIDTH + UI.MARGIN);
if (ImGui::Button("-##instrument_del"))
{
    //
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Delete Instrument");
}

ImGui::SetCursorPosX(UI.INSTRUMENTS_LIST_X + UI.INSTRUMENTS_LIST_WIDTH + UI.MARGIN);
if (ImGui::Button("^##instrument_up"))
{
    //
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Move instrument up");
}

ImGui::SetCursorPosX(UI.INSTRUMENTS_LIST_X + UI.INSTRUMENTS_LIST_WIDTH + UI.MARGIN);
if (ImGui::Button("v##instrument_down"))
{
    //
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Move instrument down");
}

ImGui::SetCursorPosX(UI.INSTRUMENTS_LIST_X + UI.INSTRUMENTS_LIST_WIDTH + UI.MARGIN);
if (ImGui::Button("o##instrument_edit"))
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

ImGui::SetNextWindowSize(ImVec2(UI.INSTRUMENT_OPTIONS_MODAL_WIDTH+200, UI.INSTRUMENT_OPTIONS_MODAL_HEIGHT+200));

if (ImGui::BeginPopupModal("Instrument options", &p_opened, ImGuiWindowFlags_NoResize))
{
    static char current_instrument_name[24] = "";
    static bool enable_write = false;
    static std::array<int,120> temp_map = instruments_list[active_instrument].SAMPLE_MAP;;
    
    if (!enable_write)
    {
        strcpy(current_instrument_name, instruments_list[active_instrument].NAME.c_str());
        enable_write = true;
    }
    
    ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
    ImGui::Text("Instrument name");
    ImGui::PopStyleColor();
    
    ImGui::PushItemWidth(200);
    
    ImGui::InputText("##instrumentnname", current_instrument_name, 24, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank);
    ImGui::PopItemWidth();
    
    ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
    ImGui::Spacing();
    ImGui::Spacing();
    //======================================================
    ImGui::Text("Sample map");
    
    ImGui::PushItemWidth(UI.LEFT_SLIDERS_WIDTH);
    ImGui::SliderInt("Octave", &samplemap_octave, 0, 9);ImGui::SameLine();
    ImGui::PopItemWidth();
    
    ImGui::SetCursorPosX(188);
    
    std::string oct_str = std::to_string(samplemap_octave);
    std::string octave_notes = 
        "C-" + oct_str +
        " C#" + oct_str +
        " D-" + oct_str +
        " D#" + oct_str +
        " E-" + oct_str +
        " F-" + oct_str +
        " F#" + oct_str +
        " G-" + oct_str +
        " G#" + oct_str +
        " A-" + oct_str +
        " A#" + oct_str +
        " B-" + oct_str;
    
    ImGui::Text(octave_notes.c_str());
    ImGui::PopStyleColor();
    
    int relative_cell_x;
    int relative_cell_y;
    
    ImGui::BeginChild("##samplemap", ImVec2(540,200), true);
    
    ImVec2 csp = ImGui::GetCursorScreenPos();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    float cpx = csp.x;
    float cpy = csp.y;
    int list_size = samples_list.size();
    int xoff = 168;
    float so = ImGui::GetScrollY();
    
    int mrelative_x = ImGui::GetMousePos().x - 176 - ImGui::GetWindowPos().x;
    int mrelative_y = ImGui::GetMousePos().y - 8 - ImGui::GetWindowPos().y;
    
    dl->AddLine(ImVec2(cpx,cpy+so), ImVec2(cpx,cpy+(list_size * 17)+so),col_active_cell_border);
    
    // vertical separators
    for (int j = 0; j < 13; ++j)
    {
        dl->AddLine(ImVec2(cpx+xoff,cpy+so), ImVec2(cpx+xoff,cpy+(list_size * 17)+so),col_active_cell_border);
        xoff += 28;
    }
    
    // horizontal separators & sample names
    static int selected_sample = 0;
    
    for (int i = 0; i < samples_list.size(); ++i)
    {
        dl->AddLine(ImVec2(ImGui::GetWindowPos().x+8,ImGui::GetWindowPos().y+8-so), ImVec2(ImGui::GetWindowPos().x+512,ImGui::GetWindowPos().y+8-so), col_active_cell_border);
        
        std::string full_name = samples_list[i].NAME;
        std::string trunc_name = full_name.substr(0, 20);
        if (i <= 9)
        {
            trunc_name = "0" + std::to_string(i) + ":" + trunc_name;
        }
        else
        {
            trunc_name = std::to_string(i) + ":" + trunc_name;
        }
        
        int col;
        if (selected_sample == i)
        {
            col = col_title_text;
        }
        else
        {
            col = col_row_headers;
        }
        
        ImGui::PushStyleColor(ImGuiCol_Text, col);
        ImGui::Text(trunc_name.c_str());
        ImGui::PopStyleColor();
        
        csp = ImGui::GetCursorPos();
        cpx = csp.x;
        cpy = csp.y;
        dl->AddLine(ImVec2(ImGui::GetWindowPos().x+cpx,ImGui::GetWindowPos().y+cpy-so), ImVec2(ImGui::GetWindowPos().x+cpx+504,ImGui::GetWindowPos().y+cpy-so), col_active_cell_border);
        
        int xxoff = 0;
        for (int k = 0; k < 12; ++k)
        {
            //if (instruments_list[active_instrument].SAMPLE_MAP[(12 * samplemap_octave) + k] == i)
            //{
            //dl->AddRectFilled(ImVec2(ImGui::GetWindowPos().x+177 + xxoff,ImGui::GetWindowPos().y+cpy-16-so),
            //ImVec2(ImGui::GetWindowPos().x+177 + xxoff + 26,ImGui::GetWindowPos().y+cpy-so), col_active_cell);
            //}
            //
            //xxoff += 28;
            if (temp_map[(12 * samplemap_octave) + k] == i)
            {
                dl->AddRectFilled(ImVec2(ImGui::GetWindowPos().x+177 + xxoff,ImGui::GetWindowPos().y+cpy-16-so),
                                  ImVec2(ImGui::GetWindowPos().x+177 + xxoff + 26,ImGui::GetWindowPos().y+cpy-so), col_active_cell);
            }
            
            xxoff += 28;
        }
    }
    // get cell
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
    {
        // xoff=176, xmax=335, yoff=8
        relative_cell_x = mrelative_x / 28;
        relative_cell_y = (mrelative_y + ImGui::GetScrollY()) / 17;
        
        if (ImGui::IsMouseClicked(0))
        {
            if (relative_cell_x >=0 && relative_cell_x <= 11 && relative_cell_y >= 0 && relative_cell_y <= samples_list.size() - 1 && mrelative_x >= 0 && mrelative_y >= 0)
            {
                temp_map[(12 * samplemap_octave) + relative_cell_x] = relative_cell_y;
                //instruments_list[active_instrument].SAMPLE_MAP[(12 * samplemap_octave) + relative_cell_x] = relative_cell_y;
            }
            else if (relative_cell_x < 0 && relative_cell_y >= 0 && relative_cell_y <= samples_list.size() - 1 && mrelative_x < 0 && mrelative_y >= 0)
            {
                selected_sample = relative_cell_y;
            }
        }
    }
    
    ImGui::EndChild();
    //=====================================================
    
    // Assign sample to octave(s)
    if (selected_sample >= 0 && selected_sample < samples_list.size())
    {
        ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
        ImGui::Text("Assign selected sample to:");
        ImGui::PopStyleColor();
        
        ImGui::PushStyleColor(ImGuiCol_Button, col_button);
        
        if (ImGui::Button("Current octave"))
        {
            for (int i = 0; i < 12; ++i)
            {
                temp_map[(12 * samplemap_octave) + i]= selected_sample;
                //instruments_list[active_instrument].SAMPLE_MAP[(12 * samplemap_octave) + i]= selected_sample;
            }
        }
        
        ImGui::SameLine();
        
        if (ImGui::Button("All octaves"))
        {
            for (int i = 0; i < 120; ++i)
            {
                temp_map[i]= selected_sample;
                //instruments_list[active_instrument].SAMPLE_MAP[i]= selected_sample;
            }
        }
        
        ImGui::PopStyleColor();
    }
    
    float modal_width = ImGui::GetWindowSize().x;
    float modal_height = ImGui::GetWindowSize().y;
    ImGui::SetCursorPos(ImVec2(modal_width- 180, modal_height - 30));
    
    if (ImGui::Button("OK", ImVec2(80,0)))
    {
        enable_write = false;
        
        if (strlen(current_instrument_name) != 0)
        {
            instruments_list[active_instrument].NAME = current_instrument_name;
        }
        
        instruments_list[active_instrument].SAMPLE_MAP = temp_map;
        ImGui::CloseCurrentPopup();
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Cancel", ImVec2(80,0)))
    {
        enable_write = false;
        ImGui::CloseCurrentPopup();
    }
    
    ImGui::EndPopup();
}

// Draw Sample buttons
ImGui::SetCursorPos(ImVec2(UI.SAMPLES_LIST_X + UI.SAMPLES_LIST_WIDTH + UI.MARGIN, UI.SAMPLES_LIST_Y));

if (ImGui::Button("+##sample_add"))
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

ImGui::SetCursorPosX(UI.SAMPLES_LIST_X + UI.SAMPLES_LIST_WIDTH + UI.MARGIN);
if (ImGui::Button("-##sample_del"))
{
    //
}

if (ImGui::IsItemHovered())
{
    ImGui::SetTooltip("Delete Sample");
}

ImGui::End();