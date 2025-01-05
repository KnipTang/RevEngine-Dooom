#pragma once

#include "d3d11.h"
#include <vector>
#include <string>
#include <unordered_map>

struct SDL_Window;

namespace RevDev
{
	class ImGuiSetup final
	{
	public:
		ImGuiSetup();
		~ImGuiSetup();

		void InitWindow(int windowWidth, int windowHeight);
		void InitForSDL2(SDL_Window& window);
		void InitFor3D11(ID3D11Device& device, ID3D11DeviceContext& deviceContext);

		void Update();

		void ToggleUI(bool value) { m_On = value; }

		void SubscribeElement(std::string text, const float* value);
	private:
		float m_ImguiWindowWidth;
		float m_ImguiWindowHeight;

		float m_WindowHeight;

		std::unordered_map<std::string, const float*> m_SubscriptedElements;

		bool m_On;
	};
}