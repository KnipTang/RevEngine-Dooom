#include "CompInput.h"
#include "Rev_CoreSystems.h"
#include <utility>

using namespace Rev;

CompInput::CompInput(GameObject* gameObj) :
	BaseComponent(gameObj)
{
	Rev::Rev_CoreSystems::pInputManager->SubscribeInputComp(this);
}

void CompInput::BindKeyAction(SDL_Scancode key, std::function<void()> action)
{
	m_KeyActions.emplace(key, action);
}

void CompInput::ExecuteKey(SDL_Scancode key)
{
	std::pair actionRange = m_KeyActions.equal_range(key);
	for (auto iterator = actionRange.first; iterator != actionRange.second; iterator++) {
		iterator->second();
	}
}

void CompInput::BindMouseAction(Uint8 mouse, std::function<void()> action)
{
	m_MouseActions.emplace(mouse, action);
}
void CompInput::ExecuteMouse(Uint8 mouse)
{
	std::pair actionRange = m_MouseActions.equal_range(mouse);
	for (auto iterator = actionRange.first; iterator != actionRange.second; iterator++) {
		iterator->second();
	}
}