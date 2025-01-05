#pragma once

#include <memory>
#include "Rendering/RenderWindow.h"
#include "Sound/Rev_Sound.h"
#include "Input/InputManager.h"
#include "ResourceManagement/ResourceManager.h"
#include "Scenes/SceneManager.h"
#include "Rendering/HUD/UI.h"

namespace Rev
{
	class Rev_CoreSystems final
	{
	public:
		Rev_CoreSystems() = delete;
		~Rev_CoreSystems() = delete;

		//Core systems //Static vars can't depend on each other because static party problem
		//Needs inline to be able to declare static variable in header file
		static inline std::unique_ptr<Rev::SceneManager> pSceneManager = std::make_unique<Rev::SceneManager>();
		static inline std::unique_ptr<Rev::Rev_Sound> pRevSound = std::make_unique<Rev::Rev_Sound>();
		static inline std::unique_ptr<RevDev::RenderWindow> pRevRender = std::make_unique<RevDev::RenderWindow>();
		static inline std::unique_ptr<RevDev::InputManager> pInputManager = std::make_unique<RevDev::InputManager>();
		static inline std::unique_ptr<RevDev::ResourceManager> pResourceManager = std::make_unique<RevDev::ResourceManager>();
		static inline std::unique_ptr<RevDev::UI> pUI = std::make_unique<RevDev::UI>();
	private:
	};
}