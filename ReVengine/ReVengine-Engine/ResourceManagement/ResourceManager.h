#pragma once

#include <unordered_map>
#include <string>
#include <memory>

namespace Rev
{
	class Texture;
}

namespace RevDev
{
	class ResourceManager final
	{
	public:
		ResourceManager();
		~ResourceManager();

		Rev::Texture* LoadResource(const std::string& name, const std::string& path);

		Rev::Texture* GetResource(const std::string& name);

	private:
		std::unordered_map<std::string, std::unique_ptr<Rev::Texture>> m_LoadedResources;
	};
}