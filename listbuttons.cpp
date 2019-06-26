void DrawListButtons(std::vector<PATTERN_> &patterns_list,
                     std::vector<INSTRUMENT> &instruments_list,
                     std::vector<SAMPLE> &samples_list,
                     std::vector<std::vector<NOTE_DATA>> &module)
{
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
    
	ImGui::SetNextWindowSize(ImVec2(UI.INSTRUMENT_OPTIONS_MODAL_WIDTH, UI.INSTRUMENT_OPTIONS_MODAL_HEIGHT));
    
	if (ImGui::BeginPopupModal("Instrument options", &p_opened, ImGuiWindowFlags_NoResize))
	{
		ImGui::PushItemWidth(200);
		ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
		ImGui::Text("Instrument name");
        
		static char current_instrument_name[24] = "";
		// strcpy(current_pattern_name, patterns_list[active_pattern].NAME.c_str());
        
		ImGui::InputText("##instrumentnname", current_instrument_name, 24, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank);
        
		ImGui::PopStyleColor();
		ImGui::PopItemWidth();
        
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
				LoadSample(samples_list, fname);
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
}