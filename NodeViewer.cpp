#include "NodeViewer.h"

#include "imgui.h"

#define CHILD_W	300
#define CHILD_H	330

#define CCHILD_W (CHILD_W-40)
#define CCHILD_H 200

#define VCHILD_W (CCHILD_W-40)
#define VCHILD_H 100

NodeViewer::NodeViewer() : neigbours(), tx(), filters(), id(), windowName(),
	ip(), port()
{
}

void NodeViewer::update(void* n)
{
	Node* node = (Node*)n;
	id = node->getID();
	ip = node->getIP();
	port = node->getPort();
	neigbours = *(node->getNeighbours());
	filters = *(node->getFilters());
	tx = *(node->getTransactions());

	windowName = id + "##node";

}

void NodeViewer::cycle()
{
	ImGui::Begin(windowName.c_str());
	ImGui::SameLine();
	ImGui::BeginChild("VIEW", ImVec2(CHILD_W, CHILD_H));

	showData();
	showNeighbours();
	showTx();
	showFilters();

	ImGui::EndChild();
	ImGui::End();
}

void NodeViewer::showData()
{
	if (ImGui::CollapsingHeader("Data")) {
		ImGui::Text("ID: %s",id.c_str());
		ImGui::Text("Port: %s",port.c_str());
		ImGui::Text("IP: %s", ip.c_str());
	}
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
			ImGui::Text(" ");
			ImGui::SameLine();
			ImGui::BeginChild("Txs",ImVec2(CCHILD_W,CCHILD_H));
			for (int i = 0; i < tx.size(); i++) {
				string subWindowName = tx[i].txId + "##" + to_string(i);
				if (ImGui::CollapsingHeader(subWindowName.c_str())) {
					ImGui::Text("ID: %s",tx[i].txId.c_str());

					ImGui::Text("Number of incomes: %u",tx[i].nTxIn);
					ImGui::Text(" ");
					ImGui::SameLine();
					string vinwindow = "vins##" + to_string(i);
					ImGui::BeginChild(vinwindow.c_str(),ImVec2(VCHILD_W,VCHILD_H),false,ImGuiWindowFlags_HorizontalScrollbar|ImGuiWindowFlags_AlwaysVerticalScrollbar);
					if (ImGui::CollapsingHeader("vIn")) {
						for (int j = 0; j < tx[i].vIn.size(); j++) {
							ImGui::Text("Block ID: %s\tTx ID: %s",tx[i].vIn[j].blockId.c_str(), tx[i].vIn[j].txId.c_str());
						}
					}
					ImGui::EndChild();

					ImGui::Text("Number of outcomes: %u", tx[i].nTxOut);
					ImGui::Text(" ");
					ImGui::SameLine();
					string voutwindow = "vout##" + to_string(i);
					ImGui::BeginChild(voutwindow.c_str(), ImVec2(VCHILD_W, VCHILD_H),false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
					if (ImGui::CollapsingHeader("vOut")) {
						for (int j = 0; j < tx[i].vOut.size(); j++) {
							ImGui::Text("Public ID: %s\tTx Amount: %lu", tx[i].vOut[j].publicId.c_str(), tx[i].vOut[j].amount);
						}
					}
					ImGui::EndChild();
				}
			}
			ImGui::EndChild();
		}
		else {
			ImGui::Text("No transactions available");
		}
	}
}
