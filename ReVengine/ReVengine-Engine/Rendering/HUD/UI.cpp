#include "UI.h"
#include "Rev_CoreSystems.h"
#include "ImGui/ImGuiSetup.h"

using namespace RevDev;

UI::UI() : 
	m_ImGui{ Rev::Rev_CoreSystems::pRevRender->GetImGuiHandle() }
{

}

UI::~UI()
{

}

void UI::SubscribeElement(std::string text, const float* value)
{
	if(m_ImGui != nullptr)
		m_ImGui->SubscribeElement(text, value);
	else
	{
		m_ImGui = Rev::Rev_CoreSystems::pRevRender->GetImGuiHandle();
		SubscribeElement(text, value);
	}
}

void UI::ToggleUI(bool value)
{
	if (m_ImGui != nullptr)
	m_ImGui->ToggleUI(value);
	else
	{
		m_ImGui = Rev::Rev_CoreSystems::pRevRender->GetImGuiHandle();
		ToggleUI(value);
	}
}