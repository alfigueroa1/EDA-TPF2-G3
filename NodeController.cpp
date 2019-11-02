#include "NodeController.h"
#include "imgui.h"
#include <cstring>

#define CHILD_W	220
#define CHILD_H	330
#define BUTTON_S	100
#define BUTTONS_PER_ROW 2


NodeController::NodeController(Node& m) :
	model(&m), mID(), mNeighbours(), cstate(OUT), windowName(), availableNeighbours(false),
	currNeighbour(0), comboNeighbour(), newId(), newPort()
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
	neighbourSelect();
}
void NodeController::drawGBHeader() {
	returnButton();
	neighbourSelect();
}
void NodeController::drawPTX() {
	returnButton();
	neighbourSelect();
}
void NodeController::drawPMBlock() {
	returnButton();
	neighbourSelect();
}
void NodeController::drawPFilter() {
	returnButton();
	neighbourSelect();
}
void NodeController::drawAddNode() {
	returnButton();
	ImGui::Text("\nADD A NEIGHBOUR\n\n");
	newPortSelect();
	newIpSelect();

	if (ImGui::Button("ADD NEIGHBOUR")) {
		string nIp = to_string(newId[0]) + '.' + to_string(newId[1]) + '.' + to_string(newId[2]) + '.' + to_string(newId[3]);
		string nPort = to_string(newPort);
		model->AddNeighbour(nIp,nPort);
		cstate = OUT;
	}
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
		ImGui::Text("Neighbour nodes -empty-");
		ImGui::Combo("##Neighbour nodes -empty-", &curr, "\0");
	}
	else {
		ImGui::Text("Neighbour nodes");
		ImGui::Combo("##Neighbour nodes", &currNeighbour, &comboGetter, this, mNeighbours.size());
	}
}



bool NodeController::comboGetter(void* data, int idx, const char** out_str)
{
	NodeController& node = *(NodeController*)data;
	string rta = node.mNeighbours[idx].port + " - " + node.mNeighbours[idx].ip ;
	strncpy(node.comboNeighbour, rta.c_str(), COMBO_SIZE);
	node.comboNeighbour[COMBO_SIZE] = '\0';
	*out_str = node.comboNeighbour;
	return true;

}

void NodeController::newIpSelect()
{
	ImGui::Text("Neighbour IP:");
	ImGui::PushItemWidth(25);
	ImGui::DragInt(".##1", newId, 0.5);
	ImGui::SameLine();
	ImGui::DragInt(".##2", newId + 1, 0.5);
	ImGui::SameLine();
	ImGui::DragInt(".##3", newId + 2, 0.5);
	ImGui::SameLine();
	ImGui::DragInt("##4", newId + 3, 0.5);
	ImGui::PopItemWidth();
}

void NodeController::newPortSelect()
{
	ImGui::Text("Neighbour Port:");
	ImGui::SetNextItemWidth(50);
	ImGui::DragInt("##Port", &newPort, 0.5);
}
