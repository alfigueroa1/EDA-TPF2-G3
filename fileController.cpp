#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "imgui.h"
#include "imgui_impl_allegro5.h"
#include "model.h"
#include "fileController.h"

//FILE CONTROLLER
fileController::fileController(Model& model) {
	m = &model;
}

void fileController::update(void* model) {
	m = (Model*)model;
}


string fileController::askPath() {
	string aux = "";

	ImGui::Begin("File Selector");
	ImGui::Text("Por favor especifique la carpeta con los archivos que desea");
	ImGui::Text("Recuerde que en Windows las carpetas se separan con \\");
	ImGui::NewLine();

	ImGui::InputText("Path", path, _MAX_PATH);
	ImGui::SameLine();
	if (ImGui::Button("Submit")) {
		filenames.clear();
		vector<string>* p = m->getBlockChainNames(path);		//busca los .json en el path
		if (p != NULL) {
			for (int i = 0; i < p->size(); i++)
				filenames.push_back((*p)[i]);
			if (filenames.size() == 1)
				ImGui::Text("Se encontro 1 archivo:");
			else if (filenames.size() == 0)
				ImGui::Text("No se encontraron archivos");
			else
				ImGui::Text("Se encontraron %d archivos:", filenames.size());
		}
	}

	ImGui::NewLine();
	for (int i = 0; i < filenames.size(); i++) {
		if (ImGui::Selectable(filenames[i].c_str()))
			aux = filenames[i];
	}

	ImGui::End();

	return aux;
}









