#pragma once

#include <string>
#include <vector>

namespace RevDev
{
	class ImGuiSetup;
}

namespace RevDev
{
	class UI final
	{
	public:
		UI();
		~UI();

		void SubscribeElement(std::string text, const float* value);
		void ToggleUI(bool value);
	private:
		RevDev::ImGuiSetup* m_ImGui;
	};
}