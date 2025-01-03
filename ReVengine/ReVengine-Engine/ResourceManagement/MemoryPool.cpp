#include "MemoryPool.h"
#include "GameObjects/GameObject.h"

using namespace Rev;

MemoryPool::MemoryPool(GameObject* gameObj, int fixedSize) :
	m_PoolSize{ fixedSize }
{
	gameObj->SetActive(false);

	for (int i = 0; i < m_PoolSize; i++)
	{
		m_Objects.emplace_back(gameObj); 
	}
}

MemoryPool::~MemoryPool()
{
}

GameObject* MemoryPool::Activate()
{
	for (int i = 0; i < m_PoolSize; i++)
	{
		if (!m_Objects.at(i)->IsActive())
		{
			m_Objects.at(i)->SetActive(true);
			return m_Objects.at(i);
		}
	}

	return nullptr;
}

std::vector<GameObject*> MemoryPool::Activate(int size)
{
	std::vector<GameObject*> activatedObjects;

	for (int i = 0; i < size; i++)
	{
		activatedObjects.emplace_back(Activate());
	}

	return activatedObjects;
}