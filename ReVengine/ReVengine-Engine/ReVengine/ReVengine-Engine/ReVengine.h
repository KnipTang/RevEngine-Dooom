#pragma once

#include <functional>
#include <memory>

namespace RevDev 
{
	class RenderWindow;
}

namespace Rev
{
	class SceneManager;
}

namespace Rev
{
	class ReVengine
	{
	public:
		ReVengine(int windowWidth, int windowHeight, float nearZ = 0.5f, float farZ = 1000);
		~ReVengine();

		void Run(const std::function<SceneManager* ()>& GameRun);
	private:
		std::unique_ptr<RevDev::RenderWindow> pRenderWindow;

		int m_WindowWidth;
		int m_WindowHeight;
	};
}