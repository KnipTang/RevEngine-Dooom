#include "GameObject.h"
#include "BaseComponent.h"
#include "Components/CompTransform.h"
#include "Rev_CoreSystems.h"
#include "Scenes/Scene.h"

using namespace Rev;

int GameObject::s_ObjIDCounter = 0;

GameObject::GameObject(std::string tag) :
	m_Tag{ tag },
	m_Components{},
	m_ChildrenCount{0},
	m_Parent{nullptr},
	m_Active{true},
	m_ToDestroy{false},
	m_ObjID{s_ObjIDCounter++}
{
	transform = AddComponent<Rev::CompTransform>(this);
}

GameObject::~GameObject()
{
}

GameObject::GameObject(const GameObject& other) :
	transform{ other.transform },
	m_Components{},
	m_ChildrenCount{ other.m_ChildrenCount },
	m_Parent{ other.m_Parent },
	m_Active{ other.m_Active },
	m_ObjID{ s_ObjIDCounter++ }
{
	m_Components.reserve(other.m_Components.size());
	for (const auto& comp : other.m_Components) {
		m_Components.emplace_back(std::make_unique<BaseComponent>(*comp));
	}

	m_Children.reserve(other.m_Children.size());
	for (const auto& child : other.m_Children) {
		m_Children.emplace_back(std::make_unique<GameObject>(*child));
	}
}

void GameObject::Update(float deltaTime)
{
	for (auto&& comp : m_Components)
	{
		comp->Update(deltaTime);
	}
	for (auto&& obj : m_Children)
	{
		obj->Update(deltaTime);
	}
}

void GameObject::LateUpdate(float deltaTime)
{
	for (auto&& comp : m_Components)
	{
		comp->LateUpdate(deltaTime);
	}
	for (auto&& obj : m_Children)
	{
		obj->LateUpdate(deltaTime);
	}
}

void GameObject::FixedUpdate(float fixeDeltaTime)
{
	for (auto&& comp : m_Components)
	{
		comp->FixedUpdate(fixeDeltaTime);
	}
	for (auto&& obj : m_Children)
	{
		obj->FixedUpdate(fixeDeltaTime);
	}
}

const void GameObject::Render()
{
	for (auto&& comp : m_Components)
	{
		comp->Render();
	}
	for (auto&& obj : m_Children)
	{
		obj->Render();
	}
}

GameObject* GameObject::AddChild(GameObject* childObj)
{
	childObj->SetParent(this);

	childObj->transform->SetPosition(childObj->transform->GetWorldPosition() - transform->GetWorldPosition());
	childObj->transform->SetRotationRad(childObj->transform->GetWorldRotation() - transform->GetWorldRotation());

	m_Children.emplace_back(childObj);

	m_ChildrenCount++;

	return m_Children.back().get();
}

void GameObject::SetActive(bool active)
{
	m_Active = active;
	Rev::Rev_CoreSystems::pSceneManager->GetActiveScenes();
	if (active)
		Rev::Rev_CoreSystems::pSceneManager->GetActiveScenes().at(0)->AddActiveGameObject(this);
	else
		Rev::Rev_CoreSystems::pSceneManager->GetActiveScenes().at(0)->RemoveActiveGameObject(this);
}