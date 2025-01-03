#include "ResourceManager.h"
#include "Rendering/Texture.h"
#include <SDL_messagebox.h>

using namespace RevDev;

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

Rev::Texture* ResourceManager::LoadResource(ID3D11Device* pDevice, const std::string& name, const std::string& path)
{
	auto it = m_LoadedResources.find(name);
	if (it != m_LoadedResources.end()) //Check if inside of loadedResources
	{
		return it->second.get();
	}
	else
	{
		Rev::Texture* newTexture = new Rev::Texture{ pDevice, path };
		auto newResource = m_LoadedResources.emplace(name, newTexture);
		return newResource.first->second.get();
	}
}

Rev::Texture* ResourceManager::GetResource(const std::string& name)
{
	auto it = m_LoadedResources.find(name);
	if (it != m_LoadedResources.end()) //Check if inside of loadedResources
		return it->second.get();
	else
	{
		std::string message = "Texture \"" + name + "\" not found!";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Resource Manager", message.c_str(), nullptr);
		return nullptr;
	}
}