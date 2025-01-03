#pragma once
#include <memory>
#include <vector>
#include <SDL_scancode.h>
#include <SDL_events.h>
#include "Utils/MousePos.h"

namespace Rev
{
	class CompInput;
}

namespace RevDev
{
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void SubscribeInputComp(Rev::CompInput* inputComp);

		void HandleKeyDown(SDL_Scancode key);
		void HandleMouseKeyDown(Uint8 mouse);
		void HandleMouseRelativeMotion(int x, int y);

		MouseRelativeMotion GetMouseRelativeMotion() { return *m_MouseRelativeMotion.get(); }
	private:
		std::vector<Rev::CompInput*> m_SubscriptedInputComps;

		std::unique_ptr<MouseRelativeMotion> m_MouseRelativeMotion;
	};
}