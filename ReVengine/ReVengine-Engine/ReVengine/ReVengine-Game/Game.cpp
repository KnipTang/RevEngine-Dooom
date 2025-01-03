#include "ReVengine.h"
#include "Rev_CoreSystems.h"
#include "GameObjects/GameObject.h"
#include "Scenes/Scene.h"
#include "GameSettings.h"

std::unique_ptr<Rev::Scene> Scene1()
{
	std::unique_ptr<Rev::Scene> scene = std::make_unique<Rev::Scene>();
	std::unique_ptr<Rev::GameObject> exampleObject = std::make_unique<Rev::GameObject>();

	//Scene add gameobects & return
	{
		scene->addGameObject(std::move(exampleObject));
		scene->DisplaySceneHierarchy();
		return std::move(scene);
	}
}

Rev::SceneManager* Load()
{
	std::unique_ptr<Rev::Scene> scene(Scene1());

	Rev::Rev_CoreSystems::pSceneManager->addScene(std::move(scene));
	return Rev::Rev_CoreSystems::pSceneManager.get();
}

int main(int argc, char* argv[])
{
	std::unique_ptr<Rev::ReVengine> pReVengine;
	pReVengine = std::make_unique<Rev::ReVengine>(GameSettings::windowWidth, GameSettings::windowHeight);

	pReVengine->Run(Load);

	return 0;
}