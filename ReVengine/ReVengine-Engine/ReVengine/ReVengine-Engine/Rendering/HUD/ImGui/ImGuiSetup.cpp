#include "ImGuiSetup.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_sdl2.h"

using namespace RevDev;

ImGuiSetup::ImGuiSetup() :
	m_ImguiWindowWidth{300},
	m_ImguiWindowHeight{100}
{
}

ImGuiSetup::~ImGuiSetup()
{
}

void ImGuiSetup::InitWindow(int /*windowWidth*/, int windowHeight)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	
	m_WindowHeight = float(windowHeight);

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.1f, 0.02f, 1.0f);
}

void ImGuiSetup::InitForSDL2(SDL_Window& window)
{
	ImGui_ImplSDL2_InitForD3D(&window);
}

void ImGuiSetup::InitFor3D11(ID3D11Device& device, ID3D11DeviceContext& deviceContext)
{
	ImGui_ImplDX11_Init(&device, &deviceContext);
}

void ImGuiSetup::Update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0, m_WindowHeight - m_ImguiWindowHeight), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(m_ImguiWindowWidth, m_ImguiWindowHeight), ImGuiCond_Always);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin("Invisible Window", nullptr,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove /*| ImGuiWindowFlags_NoBackground*/);

	ImGui::SetNextWindowPos(ImVec2(0, m_WindowHeight - m_ImguiWindowHeight), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(m_ImguiWindowWidth, m_ImguiWindowHeight), ImGuiCond_Always);

	for (const auto& element : m_SubscriptedElements)
	{
		ImGui::Text("%s: %.2f", element.first.c_str(), *element.second);
	}

	ImGui::End();

	ImGui::PopStyleVar(2);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiSetup::SubscribeElement(std::string text, const float* value)
{
	m_SubscriptedElements.emplace(std::move(text), value);
}