void DrawListButtons(std::vector<PATTERN_> &patterns_list, std::vector<INSTRUMENT> &instruments_list, std::vector<std::vector<NOTE_DATA>> &module)
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

	if (ImGui::BeginPopupModal("Pattern Options", &p_opened, ImGuiWindowFlags_NoResize))
	{
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

	ImGui::SetNextWindowSize(ImVec2(UI.INSTRUMENT_OPTIONS_MODAL_WIDTH, UI.INSTRUMENT_OPTIONS_MODAL_HEIGHT));

	if (ImGui::BeginPopupModal("Instrument options", &p_opened, ImGuiWindowFlags_NoResize))
	{
		ImGui::EndPopup();
	}

	ImGui::End();
}