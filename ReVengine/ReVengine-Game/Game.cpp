#include "ReVengine.h"
#include "Rev_CoreSystems.h"
#include "Sound/Rev_Sound.H"
#include "GameObjects/GameObject.h"
#include "GameObjects/BaseComponent.h"
#include "GameObjects/Components/CompTransform.h"
#include "GameObjects/Components/CompRender.h"
#include "GameObjects/Components/CompInput.h"
#include "GameObjects/Components/CompCamera.h"
#include "GameObjects/Components/CompCollision.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "Rendering/Texture.h"
#include "GameSettings.h"
#include <glm/vec3.hpp>
#include <SDL_scancode.h>
#include <iostream>
#include "Objects/Weapons/BulletComp.h"
#include "Objects/Weapons/GunComp.h"
#include "Objects/Enemies/LookAtPlayerComp.h"
#include "Objects/Enemies/EnemyComp.h"
#include "Objects/ObjectSpawnerComp.h"
#include "Rendering/Shaders/TextureShader.h"
#include "Rendering/Shaders/TextureShader2D.h"
#include "Objects/HealthComp.h"
#include "Physics/Physics.h"
#include "Rendering/HUD/UI.h"

const std::string resourceFolder = "../game_resources";
const std::string doomSprites = "/doomSprites";
const std::string doomEnemies = "/Enemies";
const std::string doomBullets = "/Bullets";
const std::string doomWeapons = "/Weapons";
const std::string doomMap = "/Map";

std::unique_ptr<Rev::Scene> GameOverScene();

std::unique_ptr<Rev::Scene> StartScene()
{
	std::unique_ptr<Rev::Scene> scene = std::make_unique<Rev::Scene>("Menu");
	scene->SetActive(true);
	Rev::Rev_CoreSystems::pUI->ToggleUI(false);
	const std::string menu = "/menu";

	const std::string eToStartPath = resourceFolder + menu + "/eToStart.png";
	const std::string BackGroundPath = resourceFolder + doomSprites + doomMap + "/floor0_5.png";

	Rev::Texture* eToStartTexture = Rev::Rev_CoreSystems::pResourceManager->LoadResource("eToStart", eToStartPath);
	Rev::TextureShader2D* textureShader = new Rev::TextureShader2D{};



	Rev::GameObject* textEToStart = new Rev::GameObject{};
	textEToStart->transform->SetPosition(-1, -1, 0);
	Rev::CompRender* textRenderComp = textEToStart->AddComponent<Rev::CompRender>(textEToStart, 
		new Rev::CompCamera{ textEToStart, textEToStart->transform }, 
		textureShader, eToStartTexture,
		4,4, glm::vec3{0,0,0.1f}, false);
	textRenderComp->m_Is2D = true;
	Rev::CompInput* inputComp = textEToStart->AddComponent<Rev::CompInput>(textEToStart);
	inputComp->BindKeyAction(SDL_SCANCODE_E, []() { 
		Rev::Rev_CoreSystems::pSceneManager->GetSceneByTag("Menu")->SetActive(false); 
		Rev::Rev_CoreSystems::pSceneManager->GetSceneByTag("GameScene")->SetActive(true);	
		//Rev::Rev_CoreSystems::pSceneManager->addScene(std::move(GameOverScene()));

		Rev::Rev_CoreSystems::pUI->ToggleUI(true);  
		});

	scene->AddGameObject(textEToStart);
	return scene;
}

std::unique_ptr<Rev::Scene> GameScene()
{
	std::unique_ptr<Rev::Scene> scene = std::make_unique<Rev::Scene>("GameScene");
	scene->SetActive(false);
	Rev::Physics* physicsHandle = Rev::Rev_CoreSystems::pSceneManager->GetPhysicsHandle();


	//Sound
	{
		const std::string SoundPew = "/sound/pew_pew.wav";
		const std::string SoundEnemyGrawling = "/sound/EnemyGrawling.mp3";

		Rev::Rev_CoreSystems::pRevSound->LoadSound("pew", resourceFolder + SoundPew);
		Rev::Rev_CoreSystems::pRevSound->LoadSound("EnemyGrawl", resourceFolder + SoundEnemyGrawling);
		Rev::Rev_CoreSystems::pRevSound->LoadSound("Hurt", resourceFolder + "/sound/Hurt.mp3");
		Rev::Rev_CoreSystems::pRevSound->PlayRevSound("pew");
	}

	//Textures
	const std::string mainBulletPath = resourceFolder + doomSprites + doomBullets + "/misla5.png";
	const std::string secondBulletPath = resourceFolder + doomSprites + doomBullets + "/misla1.png";
	const std::string weaponBulletPath = resourceFolder + doomSprites + doomWeapons + "/pisga0.png";
	const std::string enemyDoomFile = resourceFolder + doomSprites + doomEnemies + "/bossb1.png";
	const std::string doomFloor = resourceFolder + doomSprites + doomMap + "/floor0_5.png";
	Rev::Texture* enemyTexture = Rev::Rev_CoreSystems::pResourceManager->LoadResource("Enemy", enemyDoomFile);
	Rev::Texture* bulletTexture = Rev::Rev_CoreSystems::pResourceManager->LoadResource("bulletTexture", mainBulletPath);
	Rev::Texture* bullet2Texture = Rev::Rev_CoreSystems::pResourceManager->LoadResource("bullet2Texture", secondBulletPath);
	Rev::Texture* weaponTexture = Rev::Rev_CoreSystems::pResourceManager->LoadResource("weaponTexture", weaponBulletPath);
	Rev::Texture* floorTexture = Rev::Rev_CoreSystems::pResourceManager->LoadResource("floor", doomFloor);

	Rev::TextureShader* textureShader = new Rev::TextureShader{};
	Rev::TextureShader2D* textureShader2D = new Rev::TextureShader2D{ };

	//Player
	std::unique_ptr<Rev::GameObject> player = std::make_unique<Rev::GameObject>("Player");
	player->transform->SetPosition(0, 0.5f, 0);
	HealthComp* healthPlayerComp = player->AddComponent<HealthComp>(player.get());
	healthPlayerComp->SetDeathFunctionality([]() {
		Rev::Rev_CoreSystems::pSceneManager->GetSceneByTag("GameScene")->SetActive(false);
		Rev::Rev_CoreSystems::pSceneManager->GetSceneByTag("GameOver")->SetActive(true);
		Rev::Rev_CoreSystems::pUI->ToggleUI(false);
		});
	Rev::Rev_CoreSystems::pUI->SubscribeElement("Health: ", &healthPlayerComp->GetHealth());
	Rev::CompCamera* cameraComp = player->AddComponent<Rev::CompCamera>(player.get(), player->transform);
	Rev::CompInput* inputComp = player->AddComponent<Rev::CompInput>(player.get());
	player->AddComponent<Rev::CompCollision>(player.get(), physicsHandle, false, false, glm::vec3{0.1f, 1, 0.1f});
	cameraComp->LockAxes(RotationAxes::x, true);

	//Map
	std::vector<std::unique_ptr<Rev::GameObject>> floors;
	for (float i = -5; i < 5; i+=0.5f)
	{
		for (float j = -5; j < 5; j += 0.5f)
		{
			floors.emplace_back(std::make_unique<Rev::GameObject>("Floor"));
			auto&& currentFloor = floors.back();

			currentFloor->transform->SetRotationDegree(90, 0, 0);
			currentFloor->transform->SetPosition(i, 0, j);
			currentFloor->AddComponent<Rev::CompRender>(currentFloor.get(), cameraComp, textureShader, floorTexture, 1.f, 1.f);

		}
	}

	//Gun
	std::unique_ptr<Rev::GameObject> gun = std::make_unique<Rev::GameObject>("Gun");
	gun->transform->SetPosition(0, -1.f, 0);
	Rev::CompRender* gunRender = gun->AddComponent<Rev::CompRender>(gun.get(), cameraComp, textureShader2D, weaponTexture, 0.7f, 0.7f, glm::vec3{},  true);
	gunRender->m_Is2D = true;
	GunComp* gunComp = gun->AddComponent<GunComp>(gun.get(), player->transform, 0.25f,
		[cameraComp, textureShader, bulletTexture, physicsHandle]() {
			Rev::GameObject* bullet = new Rev::GameObject{"Bullet"};
			bullet->AddComponent<Rev::CompRender>(bullet, cameraComp, textureShader, bulletTexture, 0.1f, 0.1f, glm::vec3{0,0,0}, true);
			BulletComp& bulletComp = *bullet->AddComponent<BulletComp>(bullet, 50.f);
			bulletComp.SetMaxTravelDistance(100);
			Rev::CompCollision& bulletColl = *bullet->AddComponent<Rev::CompCollision>(bullet, physicsHandle, false, false, glm::vec3{ 0.1f, 0.1f, 0.1f });
			bulletColl.SetOnContactFunction(
				[](Rev::CompCollision* other) {
				Rev::GameObject& obj = *other->GetGameObject();
				if (obj.m_Tag != "Bullet" && obj.m_Tag != "Player") obj.Destroy();
				});
			return bullet;
		});
	gunComp->SetFireSoundEffect("pew");
	gunComp->SetAmmo(30);
	Rev::Rev_CoreSystems::pUI->SubscribeElement("Ammo: ", &gunComp->GetAmmo());

	Rev::CompTransform* playerTransform = player->transform;
	float walkingSpeed = 0.05f;
	//Input Config
	{
	inputComp->BindKeyAction(SDL_SCANCODE_I, [playerTransform]() { playerTransform->AddPitchInput(10); });
	inputComp->BindKeyAction(SDL_SCANCODE_K, [playerTransform]() { playerTransform->AddPitchInput(-10); });
	inputComp->BindKeyAction(SDL_SCANCODE_L, [playerTransform]() { playerTransform->AddYawInput(10); });
	inputComp->BindKeyAction(SDL_SCANCODE_J, [playerTransform]() { playerTransform->AddYawInput(-10); });
	inputComp->BindKeyAction(SDL_SCANCODE_W, [playerTransform, walkingSpeed]() { playerTransform->MoveForward(1, walkingSpeed); });
	inputComp->BindKeyAction(SDL_SCANCODE_S, [playerTransform, walkingSpeed]() { playerTransform->MoveForward(-1, walkingSpeed); });
	inputComp->BindKeyAction(SDL_SCANCODE_D, [playerTransform, walkingSpeed]() { playerTransform->MoveRight(1, walkingSpeed); });
	inputComp->BindKeyAction(SDL_SCANCODE_A, [playerTransform, walkingSpeed]() { playerTransform->MoveRight(-1, walkingSpeed); });

	inputComp->BindKeyAction(SDL_SCANCODE_R, [playerTransform]() { playerTransform->SetRotationRad(0,0,0); });

	inputComp->BindMouseAction(SDL_BUTTON_LEFT, [gunComp]() { gunComp->Fire(); });

	inputComp->BindKeyAction(SDL_SCANCODE_G, [gunComp]() { gunComp->Fire(); });
	}

	auto lambdaCollEnemy = 
		[](Rev::CompCollision* other) {
			Rev::GameObject& obj = *other->GetGameObject();
			if (obj.m_Tag == "Bullet")
			{
				obj.Destroy();
				Rev::Rev_CoreSystems::pRevSound->PlayRevSound("EnemyGrawl");
			}
			else if (obj.m_Tag == "Player")
			{
				HealthComp* healthComp = obj.GetComponent<HealthComp>();
				if (healthComp != nullptr)
				{
					healthComp->AddHealth(-5);
					Rev::Rev_CoreSystems::pRevSound->PlayRevSound("Hurt");

					obj.transform->MoveForward(-1, 2);
				}

			}
		};

	std::function<Rev::GameObject*()> lambdaEnemyObj =
		[playerTransform, cameraComp, textureShader, enemyTexture, physicsHandle, lambdaCollEnemy]() {
		Rev::GameObject* enemy = new Rev::GameObject("Enemy");
		enemy->AddComponent<LookAtPlayerComp>(enemy, playerTransform);
		enemy->AddComponent<EnemyComp>(enemy);
		enemy->transform->SetPosition(0, 0, 5);
		enemy->AddComponent<Rev::CompRender>(enemy, cameraComp, textureShader, enemyTexture, 1.5f, 1.5f, glm::vec3{ -0.75 / 2 ,0,0 }, true);
		Rev::CompCollision& enemyColl = *enemy->AddComponent<Rev::CompCollision>(enemy, physicsHandle, false, false, glm::vec3{ 0.3f, 1, 0.5f });
		enemyColl.SetOnContactFunction(lambdaCollEnemy);
		return enemy;
		};

	//Enemies
	std::unique_ptr<Rev::GameObject> enemy1 = std::make_unique<Rev::GameObject>("Enemy");
	enemy1->AddComponent<LookAtPlayerComp>(enemy1.get(), playerTransform);
	enemy1->AddComponent<EnemyComp>(enemy1.get());
	enemy1->transform->SetPosition(0, 0, 5);
	enemy1->AddComponent<Rev::CompRender>(enemy1.get(), cameraComp, textureShader, enemyTexture, 1.5f,1.5f, glm::vec3{ -0.75/2 ,0,0}, true);
	Rev::CompCollision& enemy1Coll = *enemy1->AddComponent<Rev::CompCollision>(enemy1.get(), physicsHandle, false, false, glm::vec3{0.3f, 1, 0.5f});
	enemy1Coll.SetOnContactFunction(lambdaCollEnemy);

	std::unique_ptr<Rev::GameObject> enemy2 = std::make_unique<Rev::GameObject>("Enemy");
	enemy2->AddComponent<LookAtPlayerComp>(enemy2.get(), playerTransform);
	enemy2->AddComponent<EnemyComp>(enemy2.get());
	enemy2->transform->SetPosition(5, 0, 5);
	enemy2->AddComponent<Rev::CompRender>(enemy2.get(), cameraComp, textureShader, enemyTexture, 1.5f, 1.5f, glm::vec3{ -0.75 / 2,0,0 }, true);
	Rev::CompCollision& enemy2Coll = *enemy2->AddComponent<Rev::CompCollision>(enemy2.get(), physicsHandle, false, false, glm::vec3{ 0.3f, 1, 0.5f });
	enemy2Coll.SetOnContactFunction(lambdaCollEnemy);

	//Spawner
	std::unique_ptr<Rev::GameObject> spawner = std::make_unique<Rev::GameObject>("Spawner");
	ObjectSpawnerComp* spawnerComp = spawner->AddComponent<ObjectSpawnerComp>(spawner.get());
	spawnerComp->SetSpawnCondition([]() 
		{ 
			if (rand() % 300 == 0)
				return true;
			return false;
		});
	spawnerComp->SetObjectToSpawn(lambdaEnemyObj);
	
	//Scene add gameobects & return
	{
		scene->AddGameObject(std::move(enemy2));
		scene->AddGameObject(std::move(enemy1));
		scene->AddGameObject(std::move(gun));
		scene->AddGameObject(std::move(player));
		for (auto&& floor : floors)
		{
			scene->AddGameObject(std::move(floor));
		}
		scene->AddGameObject(std::move(spawner));
		scene->DisplaySceneHierarchy();
		return std::move(scene);
	}
}

std::unique_ptr<Rev::Scene> GameOverScene()
{
	std::unique_ptr<Rev::Scene> scene = std::make_unique<Rev::Scene>("GameOver");
	scene->SetActive(true);
	Rev::Rev_CoreSystems::pUI->ToggleUI(false);
	const std::string menu = "/menu";

	const std::string gameOverPath = resourceFolder + menu + "/GameOver.png";

	Rev::Texture* gameOverTexture = Rev::Rev_CoreSystems::pResourceManager->LoadResource("GameOver", gameOverPath);
	Rev::TextureShader2D* textureShader = new Rev::TextureShader2D{};

	Rev::GameObject* textEToStart = new Rev::GameObject{};
	textEToStart->transform->SetPosition(-1, -1, 0);
	Rev::CompRender* textRenderComp = textEToStart->AddComponent<Rev::CompRender>(textEToStart,
		new Rev::CompCamera{ textEToStart, textEToStart->transform },
		textureShader, gameOverTexture,
		4, 4, glm::vec3{ 0,0,0.1f }, false);
	textRenderComp->m_Is2D = true;
	Rev::CompInput* inputComp = textEToStart->AddComponent<Rev::CompInput>(textEToStart);
	inputComp->BindKeyAction(SDL_SCANCODE_E, []() { 
		Rev::Rev_CoreSystems::pSceneManager->GetSceneByTag("GameOver")->SetActive(false);
		Rev::Rev_CoreSystems::pSceneManager->GetSceneByTag("GameScene")->SetActive(true);
		Rev::Rev_CoreSystems::pUI->ToggleUI(true);
		});

	scene->AddGameObject(textEToStart);
	return scene;
}

Rev::SceneManager* Load()
{
	Rev::Rev_CoreSystems::pSceneManager->AddScene(std::move(StartScene()));
	Rev::Rev_CoreSystems::pSceneManager->AddScene(std::move(GameScene()));
	Rev::Rev_CoreSystems::pSceneManager->AddScene(std::move(GameOverScene()));
	return Rev::Rev_CoreSystems::pSceneManager.get();
}

int main(int argc, char* argv[])
{
	std::unique_ptr<Rev::ReVengine> pReVengine;
	pReVengine = std::make_unique<Rev::ReVengine>(GameSettings::windowWidth, GameSettings::windowHeight);

	pReVengine->Run(Load);

	return 0;
}