
#include "gui.h"

#include <imgui_internal.h>

void errorCallback(int error, char const* description) {
	ERR("GLFW error {}: {}", error, description);
}

void DrawStats(duration const& frame_time) {
	static char text_buffer[128];

	sprintf(text_buffer,
		"hello"
	);

	ImGui::SetNextWindowBgAlpha(0.0f);
	//ImGui::SetNextWindowPos(ImVec2(510.0f, 5.0f));
	ImGui::Begin("TextOverlayFG", nullptr, ImGuiWindowFlags_None);
	ImGui::TextColored(ImColor(0.7f, 0.7f, 0.7f, 1.0f), "%s", text_buffer);
	ImGui::End();
}

void beginFrameImGui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiWindowFlags wflags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
		| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus
		| ImGuiWindowFlags_NoBackground;
	bool p_open = false;
	const auto* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::Begin("root_dock", nullptr, wflags);
	ImGuiDockNodeFlags dflags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGui::DockSpace(ImGui::GetID("dockspace"), ImVec2(0.0, 0.0), dflags);
	ImGui::End();
}

void endFrameImGui() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void initImGui(GLFWwindow* window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//(void)io;

	// enable docking
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.IniFilename = nullptr;

	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\FiraCode-Regular.ttf", 20.0f);
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();

	ImGui::GetStyle().WindowBorderSize = 0.0f;
}

GLFWwindow* initGLFW(char const* title, int width, int height) {
	GLFWwindow* window = nullptr;
	glfwSetErrorCallback(errorCallback);
	if (!glfwInit())
		return nullptr;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window)
		return nullptr;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	return window;
}

bool initGL() {
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		ERR("GLEW init failed: {}", glewGetErrorString(err));
		return false;
	}
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	return true;
}

GLFWwindow* initGui(char const* title, int width, int height) {
	GLFWwindow* window = initGLFW(title, width, height);
	if (!window || !initGL())
		return nullptr;
	initImGui(window);
	return window;
}

void cleanUpGui(GLFWwindow* window) {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

GuiState::GuiState() {
	
	// dummy draw calls to setup initial state
	//beginFrameImGui();
	////dock_main_id = ImGui::GetID("dockspace");
	//display();
	////dock_id_bottom = ImGui::DockSpace(dock_main_id, ImGuiDir_Down, 0.20f, nullptr, &dock_main_id);
	//dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, nullptr, &dock_main_id);
	//dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, nullptr, &dock_main_id);
	//dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, nullptr, &dock_main_id);

	//ImGui::DockBuilderFinish(dock_main_id);
	//endFrameImGui();
}

void GuiState::display()
{
}
