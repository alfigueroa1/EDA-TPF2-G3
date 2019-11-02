#include "NodeController.h"
#include "imgui.h"

#define CHILD_W	220
#define CHILD_H	330
#define BUTTON_S	100
#define BUTTONS_PER_ROW 2


NodeController::NodeController(Node& m) :
	model(&m), mID(), mNeighbours(), cstate(OUT), windowName()
{
	mID = model->getID();
	windowName = mID + "##node";
}

void NodeController::update(void*)
{
	mNeighbours = *(model->getNeighbours());
}

void NodeController::cycle()
{
	ImGui::Begin(windowName.c_str());
	ImGui::BeginChild("CONTROL",ImVec2(CHILD_W,CHILD_H));
	switch (cstate) {
	case OUT:
		drawOut();
		break;
	case PBLOCK:
		drawPBlock();
		break;
	case GBHEADER:
		drawGBHeader();
		break;
	case PTX:
		drawPTX();
		break;
	case PMBLOCK:
		drawPMBlock();
		break;
	case PFILTER:
		drawPFilter();
		break;
	case ADDN:
		drawAddNode();
		break;
	}
	ImGui::EndChild();
	ImGui::End();
}

void NodeController::drawOut()
{
	const char* BUTTON_TEXT[6] = {
		"POST\nBLOCK",
		"GET\nBLOCK\nHEADER",
		"POST\nTX",
		"POST\nMERKLE\nBLOCK",
		"POST\nFILTER",
		"ADD\nNEIGHBOUR"
	};

	for (int i = 0; i < 6; i++) {
		if ((i % BUTTONS_PER_ROW) != 0)
			ImGui::SameLine();
		ImGui::PushID(i);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 12.0f + 0.5f, 0.8f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 12.0f + 0.5f, 0.9f, 0.9f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 12.0f + 0.5f, 1.0f, 1.0f));
		if (ImGui::Button(BUTTON_TEXT[i], ImVec2(BUTTON_S, BUTTON_S)))
			cstate = controlState(PBLOCK + i);
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}

}

void NodeController::drawPBlock() {
	returnButton();
}
void NodeController::drawGBHeader() {
	returnButton();
}
void NodeController::drawPTX() {
	returnButton();
}
void NodeController::drawPMBlock() {
	returnButton();
}
void NodeController::drawPFilter() {
	returnButton();
}
void NodeController::drawAddNode() {
	returnButton();
}

void NodeController::returnButton()
{
	if (ImGui::Button("Return"))
		cstate = OUT;
}