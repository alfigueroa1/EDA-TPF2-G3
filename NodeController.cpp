#include "NodeController.h"
#include "imgui.h"
#include <cstring>

#define CHILD_W	220
#define CHILD_H	330
#define BUTTON_S	100
#define BUTTONS_PER_ROW 2


NodeController::NodeController(Node& m) :
	model(&m), mID(), mNeighbours(), cstate(OUT), windowName(), availableNeighbours(false),
	currNeighbour(0), comboNeighbour()
{
	mID = model->getID();
	windowName = mID + "##node";
}

void NodeController::update(void*)
{
	mNeighbours = *(model->getNeighbours());

	if (mNeighbours.size() == 0)
		availableNeighbours = false;
	else {
		availableNeighbours = true;
		currNeighbour = 0;
	}
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

void NodeController::neighbourSelect()
{
	if (availableNeighbours == false) {
		int curr = 0;
		ImGui::Combo("Neighbour nodes -empty-", &curr, "\0");
	}
	else {
		ImGui::Combo("Neighbour nodes", &currNeighbour, &comboGetter, this, mNeighbours.size());
	}
}



bool NodeController::comboGetter(void* data, int idx, const char** out_str)
{
	NodeController& node = *(NodeController*)data;
	string rta = node.mNeighbours[idx].ip + " - " + node.mNeighbours[idx].port;
	strncpy(node.comboNeighbour, rta.c_str(), COMBO_SIZE);
	node.comboNeighbour[COMBO_SIZE] = '\0';
	*out_str = node.comboNeighbour;
	return true;

}
