#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <d3d11.h>

namespace Rev
{
	class Texture;
}

namespace RevDev
{
	class ResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();

		Rev::Texture* LoadResource(ID3D11Device* pDevice, const std::string& name, const std::string& path);

		Rev::Texture* GetResource(const std::string& name);

	private:
		std::unordered_map<std::string, std::unique_ptr<Rev::Texture>> m_LoadedResources;
	};
}