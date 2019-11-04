#include "NodeViewer.h"

#include "imgui.h"

#define CHILD_W	220
#define CHILD_H	330

NodeViewer::NodeViewer() : neigbours(), tx(), filters(), id(), windowName()
{
}

void NodeViewer::update(void* n)
{
	Node* node = (Node*)n;
	id = node->getID();
	neigbours = *(node->getNeighbours());
	filters = *(node->getFilters());

	windowName = id + "##node";

}

void NodeViewer::cycle()
{
	ImGui::Begin(windowName.c_str());
	ImGui::SameLine();
	ImGui::BeginChild("VIEW", ImVec2(CHILD_W, CHILD_H));

	showNeighbours();
	showTx();
	showFilters();

	ImGui::EndChild();
	ImGui::End();
}

void NodeViewer::showNeighbours()
{
	if (ImGui::CollapsingHeader("Neighbours")) {
		if (neigbours.size() > 0) {
			for (int i = 0; i < neigbours.size(); i++) {
				ImGui::Text("Port: %s - IP: %s", neigbours[i].port.c_str(), neigbours[i].ip.c_str());
			}
		}
		else {
			ImGui::Text("No neighbours available");
		}
	}
}

void NodeViewer::showFilters()
{
	if (ImGui::CollapsingHeader("Filters")) {
		if (filters.size() > 0) {
			for (int i = 0; i < filters.size(); i++) {
				ImGui::Text("%s",filters[i].c_str());
			}
		}
		else {
			ImGui::Text("No filters available");
		}
	}
}

void NodeViewer::showTx() {
	if (ImGui::CollapsingHeader("Transactions")) {
		if (tx.size() > 0) {
			for (int i = 0; i < tx.size(); i++) {
				string subWindowName = tx[i].txId + "##" + to_string(i);
				if (ImGui::CollapsingHeader(subWindowName.c_str())) {
					ImGui::Text(tx[i].txId.c_str());
				}
			}
		}
		else {
			ImGui::Text("No transactions available");
		}
	}
}
