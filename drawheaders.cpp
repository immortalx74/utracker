// draw track headers
ImGui::SetNextWindowContentSize(ImVec2((tracks * UI.TRACK_WIDTH) + UI.MAIN_PADDING, UI.CELL_HEIGHT));
ImGui::SetCursorPosX(UI.TRACK_HEADERS_START);
ImGui::BeginChild("##trackheaders", ImVec2(0, UI.TRACK_HEADERS_HEIGHT), false);
ImGui::SetScrollX(grid_scroll_x);
ImGui::Columns(tracks);

for (int t = 0; t < tracks; ++t)
{
	std::string track_text = "    Track:";
	track_text += std::to_string(t + 1);
	ImGui::PushStyleColor(ImGuiCol_Text, col_title_text);
	ImGui::Text(track_text.c_str());
	ImGui::PopStyleColor();
	ImGui::PushItemWidth(UI.TRACK_SLIDERS_WIDTH);
	ImGui::PushID(std::to_string(t).c_str());
	ImGui::SliderInt("##trackvolume", &tracks_list[t].VOLUME, 0, 64, "v:%02d");
	if (ImGui::IsItemClicked(1))
	{
		tracks_list[t].VOLUME = 64;
	}
	ImGui::PopID();
	ImGui::PushID(std::to_string(t).c_str());
	ImGui::SliderFloat("##trackpan", &tracks_list[t].PAN, -1, 1, "p:%+.2f");
	if (ImGui::IsItemClicked(1))
	{
		tracks_list[t].PAN = 0;
	}
	ImGui::PopID();
	ImGui::PopItemWidth();
	ImGui::SetColumnWidth(-1, UI.TRACK_WIDTH);
	ImGui::NextColumn();
}

ImGui::EndChild();
ImGui::Columns(1);

// draw row headers
ImGui::SetNextWindowContentSize(ImVec2(UI.CELL_WIDTH, (pattern_rows * UI.CELL_HEIGHT) + UI.CELL_WIDTH));
ImGui::BeginChild("##rowheaders", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);
ImGui::SetScrollY(grid_scroll_y);
ImGui::SetCursorPosY(8);

for (int p = 0; p < pattern_rows; ++p)
{
	std::string row_text = std::to_string(p);
	std::string row_text_zeros = std::string(3 - row_text.length(), '0') + row_text;
	ImGui::Text(row_text_zeros.c_str());
}
ImGui::EndChild();

ImGui::SetCursorPosY(UI.MAIN_X + UI.TRACK_HEADERS_HEIGHT + UI.MARGIN);
ImGui::SetCursorPosX(UI.CELL_WIDTH + 8);
ImGui::SetNextWindowContentSize(ImVec2(tracks * UI.TRACK_WIDTH, pattern_rows * UI.CELL_HEIGHT));
ImGui::BeginChild("##scrollinggrid", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

grid_scroll_x = ImGui::GetScrollX(); // update scroll_x to sync with track headers scrolling
grid_scroll_y = ImGui::GetScrollY(); // update scroll_y to sync with row headers scrolling