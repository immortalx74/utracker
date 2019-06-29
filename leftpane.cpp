
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
	for (int i = 0; i < patterns_list.size(); ++i)
	{
		if (ImGui::Selectable(patterns_list[i].NAME.c_str(), active_pattern == i))
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
	for (int i = 0; i < instruments_list.size(); ++i)
	{
		if (ImGui::Selectable(instruments_list[i].NAME.c_str(), active_instrument == i))
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
	for (int i = 0; i < samples_list.size(); ++i)
	{
		if (ImGui::Selectable(samples_list[i].NAME.c_str(), active_sample == i))
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

ImGui::PopItemWidth();

// show mouse coords
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
if (application_state == 2) as ="PLAYING";
if (application_state == 3) as ="EDITOR";

ImGui::Text("state:");ImGui::SameLine();ImGui::Text(as.c_str());

ImGui::Text("pat_start:"); ImGui::SameLine();
ImGui::Text(std::to_string(pattern_start).c_str());
ImGui::Text("pat_end:"); ImGui::SameLine();
ImGui::Text(std::to_string(pattern_end).c_str());
// Draw patterns and instruments list's buttons
//DrawListButtons(patterns_list, instruments_list, samples_list, module);


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
    //
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
static int r = 64;	

if (ImGui::BeginPopupModal("Pattern Options", &p_opened, ImGuiWindowFlags_NoResize))
{
    ImGui::PushItemWidth(200);
    ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
    ImGui::Text("Number of rows");
    ImGui::SliderInt("##numofrows", &r, 1, 64);
    ImGui::PopStyleColor();
    
    ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
    ImGui::Text("Pattern name");
    
    static char current_pattern_name[24] = "";
    // strcpy(current_pattern_name, patterns_list[active_pattern].NAME.c_str());
    
    ImGui::InputText("##patternname", current_pattern_name, 24, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank);
    
    ImGui::PopStyleColor();
    ImGui::PopItemWidth();
    
    float modal_width = ImGui::GetWindowSize().x;
    float modal_height = ImGui::GetWindowSize().y;
    ImGui::SetCursorPos(ImVec2(modal_width- 180, modal_height - 30));
    
    if (ImGui::Button("OK", ImVec2(80,0)))
    {
        if (strlen(current_pattern_name) == 0)
        {
            strcpy(current_pattern_name, patterns_list[active_pattern].NAME.c_str());
        }
        
        patterns_list[active_pattern].NAME = current_pattern_name;
        patterns_list[active_pattern].ROWS = r;
        ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(80,0)))
    {
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
    ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
    ImGui::Text("Instrument name");
    ImGui::PopStyleColor();
    
    ImGui::PushItemWidth(200);
    static char current_instrument_name[24] = "";
    
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
    for (int i = 0; i < samples_list.size(); ++i)
    {
        dl->AddLine(ImVec2(ImGui::GetWindowPos().x+8,ImGui::GetWindowPos().y+8-so), ImVec2(ImGui::GetWindowPos().x+512,ImGui::GetWindowPos().y+8-so), col_active_cell_border);
        
        std::string full_name = samples_list[i].NAME;
        std::string trunc_name = full_name.substr(0, 24);
        ImGui::Text(trunc_name.c_str());
        
        csp = ImGui::GetCursorPos();
        cpx = csp.x;
        cpy = csp.y;
        dl->AddLine(ImVec2(ImGui::GetWindowPos().x+cpx,ImGui::GetWindowPos().y+cpy-so), ImVec2(ImGui::GetWindowPos().x+cpx+504,ImGui::GetWindowPos().y+cpy-so), col_active_cell_border);
        
        int xxoff = 0;
        for (int k = 0; k < 12; ++k)
        {
            if (instruments_list[active_instrument].SAMPLE_MAP[(12 * samplemap_octave) + k] == i)
            {
                dl->AddRectFilled(ImVec2(ImGui::GetWindowPos().x+177 + xxoff,ImGui::GetWindowPos().y+cpy-16-so), ImVec2(ImGui::GetWindowPos().x+177 + xxoff + 26,ImGui::GetWindowPos().y+cpy-so), col_title_text);
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
            instruments_list[active_instrument].SAMPLE_MAP[(12 * samplemap_octave) + relative_cell_x] = relative_cell_y;
        }
    }
    
    ImGui::EndChild();
    //=====================================================
    
    float modal_width = ImGui::GetWindowSize().x;
    float modal_height = ImGui::GetWindowSize().y;
    ImGui::SetCursorPos(ImVec2(modal_width- 180, modal_height - 30));
    
    if (ImGui::Button("OK", ImVec2(80,0)))
    {
        if (strlen(current_instrument_name) == 0)
        {
            strcpy(current_instrument_name, instruments_list[active_instrument].NAME.c_str());
        }
        
        if (active_instrument <= 9)
        {
            instruments_list[active_instrument].NAME = "0" + std::to_string(active_instrument) + ":" + current_instrument_name;
        }
        else
        {
            instruments_list[active_instrument].NAME = std::to_string(active_instrument) + ":" + current_instrument_name;
        }
        
        
        ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(80,0)))
    {
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