#include "NetController.h"

#define SCREEN_WIDTH	1280
#define	SCREEN_HEIGHT	720

NetController::NetController(EDANet& m) :
	model(&m), running(true),
	display(nullptr), queue(nullptr),
	backColor(0.f,0.75f,0.75f,1.f),

	IDbuf(),
	IParr(),
	Port(),

	viewer(),
	controllerlist(),
	viewerlist()
{
	model->attach(viewer);
	init_interface();
}

NetController::~NetController()
{
	close_interface();
}

void NetController::update(void*)
{
	size_t index = controllerlist.size();//index representa el indice de un teorico nuevo elemento (controller y viewer), que solo se creara si el modelo tiene un nodo nuevo

	if (model->getNodeAmount() > index) {
		Node* newNode = model->getNode(index);

		controllerlist.emplace_back(*newNode);
		viewerlist.emplace_back();

		newNode->attach(controllerlist[index]);
		newNode->attach(viewerlist[index]);

		viewerlist[index].update(newNode); //seteo inicial de las variables mostradas por el viewer del nodo
	}
}

bool NetController::isRunning()
{
	return running;
}

void NetController::cycle()
{
	start_frame();
	
	controlWindow();

	for (int i = 0; i < controllerlist.size(); i++)
		controllerlist[i].cycle();

	for (int i = 0; i < viewerlist.size(); i++)
		viewerlist[i].cycle();

	end_frame();
}

void NetController::init_interface()
{
	al_init();
	al_install_keyboard();
	al_install_mouse();
	al_init_primitives_addon();
	al_set_new_display_flags(ALLEGRO_RESIZABLE);
	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	al_set_window_title(display, "TP FINAL PARTE 2");
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());

	//Inicializa ImGui
	IMGUI_CHECKVERSION(); //?
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();
	ImGui_ImplAllegro5_Init(display);
}

void NetController::close_interface()
{
	ImGui_ImplAllegro5_Shutdown();
	ImGui::DestroyContext();
	al_destroy_event_queue(queue);
	al_destroy_display(display);
}

void NetController::start_frame()
{
	ALLEGRO_EVENT ev;
	while (al_get_next_event(queue, &ev)) {
		ImGui_ImplAllegro5_ProcessEvent(&ev);
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			running = false;
		if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
			ImGui_ImplAllegro5_InvalidateDeviceObjects();
			al_acknowledge_resize(display);
			ImGui_ImplAllegro5_CreateDeviceObjects();
		}
	}

	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();
}

void NetController::end_frame()
{
	ImGui::Render();
	al_clear_to_color(al_map_rgba_f(backColor.x, backColor.y, backColor.z, backColor.w));
	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
	al_flip_display();
}

void NetController::controlWindow()
{
	ImGui::Begin("EDANet",&running);

	ImGui::Text("Bienvenidos a la fase 2 del TP Integrador de EDA.");
	ImGui::Text("El controller se encarga de la interfaz presente en Add Nodes (input del usuario):");

	if (ImGui::CollapsingHeader("Add nodes")) {

		ImGui::SetNextItemWidth(250);
		ImGui::InputTextWithHint("Node ID", "enter ID (\"name\" of the node) here", IDbuf, sizeof(IDbuf) - 1);

		ImGui::SetNextItemWidth(50);
		ImGui::DragInt("Port", &Port, 0.5);

		ImGui::PushItemWidth(25);
		ImGui::DragInt(".##1", IParr, 0.5);
		ImGui::SameLine();
		ImGui::DragInt(".##2", IParr+1, 0.5);
		ImGui::SameLine();
		ImGui::DragInt(".##3", IParr+2, 0.5);
		ImGui::SameLine();
		ImGui::DragInt("Node IP##4", IParr+3, 0.5);
		ImGui::PopItemWidth();

		if (ImGui::Button("Create Node")) {
			string sID = string(IDbuf);
			string sPort = to_string(Port);
			string sIP = to_string(IParr[0]) + '.' + to_string(IParr[1]) + '.' + to_string(IParr[2]) + '.' + to_string(IParr[3]);
			model->createNode(sIP, sPort, sID);
		}
	}

	
	ImGui::Text("El viewer se encarga de mostrar la informacion de la EDANet (cantidad de nodos actuales)");
	viewer.draw();
	ImGui::End();
}
