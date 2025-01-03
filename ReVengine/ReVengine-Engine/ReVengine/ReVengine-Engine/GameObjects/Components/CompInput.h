#pragma once

#include <functional>
#include <unordered_map>
#include <SDL_scancode.h>
#include "GameObjects/BaseComponent.h"

namespace Rev
{
	class CompInput : public BaseComponent
	{
	public:
		CompInput(GameObject* gameObj);
		~CompInput() = default;

		void BindKeyAction(SDL_Scancode key, std::function<void()> action);
		void ExecuteKey(SDL_Scancode key);
		
		void BindMouseAction(Uint8 mouse, std::function<void()> action);
		void ExecuteMouse(Uint8 mouse);

	private:
		using KeyActionMap = std::unordered_multimap<SDL_Scancode, std::function<void()>>;
		KeyActionMap m_KeyActions;
		using MouseActionMap = std::unordered_multimap<Uint8, std::function<void()>>;
		MouseActionMap m_MouseActions;

	};
}