#include "EDANet.h"
#include "NetController.h"

int main(void) {
	//Con la logica del programa
	EDANet model;
	NetController controller(model);
	//el controller maneja ImGui. Crea el viewer para el model

	model.attach(controller);

	while (controller.isRunning()) {
		controller.cycle();
	}

	return 0;
}