#include "NetViewer.h"

NetViewer::NetViewer() : nodeAmount(0)
{
}

void NetViewer::update(void* model)
{
	nodeAmount = ((EDANet*)model)->getNodeAmount();
}

void NetViewer::draw()
{
	if (ImGui::CollapsingHeader("Net Data")) {
		ImGui::Text("There are %u nodes in the Net", nodeAmount);
	}
}
