#include "ResourceManager.h"
#include "Rendering/Texture.h"
#include <SDL_messagebox.h>
#include "Rev_CoreSystems.h"

using namespace RevDev;

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

Rev::Texture* ResourceManager::LoadResource(const std::string& name, const std::string& path)
{
	auto it = m_LoadedResources.find(name);
	if (it != m_LoadedResources.end()) //Check if inside of loadedResources
	{
		return it->second.get();
	}
	else
	{
		Rev::Texture* newTexture = new Rev::Texture{ Rev::Rev_CoreSystems::pRevRender.get()->GetDevice(), path};
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