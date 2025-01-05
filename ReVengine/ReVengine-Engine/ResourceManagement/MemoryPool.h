#pragma once
#include <vector>

namespace Rev
{
	class GameObject;
}

namespace Rev
{
	class MemoryPool final
	{
	public:
		MemoryPool(GameObject* gameObj, int fixedSize);
		~MemoryPool();

		GameObject* Activate();
		std::vector<GameObject*> Activate(int size);

		std::vector<GameObject*> GetAllObjects() { return m_Objects; };

	private:
		int m_PoolSize;
		std::vector<GameObject*> m_Objects;
	};
}