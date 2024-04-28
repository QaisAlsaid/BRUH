#include "pch.h"
#include "Karen/Core/Layers/GuiLayer.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "Karen/Core/App.h"
//TODO: remove it
#include <GLFW/glfw3.h>

namespace Karen 
{
  GuiLayer::GuiLayer(const std::string& name) : Layer(name)
  {
    init();
  }

  GuiLayer::~GuiLayer()
  {

  }

  void GuiLayer::onAttach()
  {

  }

  void GuiLayer::onDetach()
  {

  }
  void GuiLayer::onGuiUpdate()
  {
  }
  
  void GuiLayer::begin()
  {
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
  }

  void GuiLayer::end()
  {
    App* app = App::get();
    auto& window = app->getWindow();
    ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(window.getWidth(), window.getHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
  }
  
  void GuiLayer::init()
  {
    const App* app = App::get();
    auto* window = (GLFWwindow*)app->getWindow().getNativeWindow();

    IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		
		ImGui::StyleColorsDark();
		
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
  }
}
