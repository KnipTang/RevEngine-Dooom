#include "Rev_SoLoud.h"

#include <soloud.h>
#include <soloud_wav.h>

#include <SDL_messagebox.h>

using namespace RevDev;

Rev_SoLoud::Rev_SoLoud()
{
	pRevDev_Soloud = std::make_unique<SoLoud::Soloud>();

	pRevDev_Soloud->init();
}

Rev_SoLoud::~Rev_SoLoud()
{
	pRevDev_Soloud->deinit();
}

void Rev_SoLoud::LoadSound(const std::string& name, const std::string& source)
{
	auto it = m_LoadedAudio.find(name);
	if (it != m_LoadedAudio.end())
	{
		std::string message = "Sound \"" + name + "\" is already loaded!";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Sound Manager", message.c_str(), nullptr);
	}
	else
	{
		auto wave = std::make_unique<SoLoud::Wav>();

		SoLoud::result result = wave->load(source.c_str());
		if (result == SoLoud::SO_NO_ERROR)
			m_LoadedAudio.emplace(name, std::move(wave));
		else
		{
			std::string message = "Loaded sound \"" + name + "\" not found!";
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Sound Manager", message.c_str(), nullptr);
		}
	}
}

void Rev_SoLoud::PlaySound(const std::string name)
{
	auto&& it = m_LoadedAudio.find(name);
	if (it != m_LoadedAudio.end())
	{
		pRevDev_Soloud->play(*it->second);
	}
	else
	{
		std::string message = "Played sound \"" + name + "\" not found!";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Sound Manager", message.c_str(), nullptr);
	}
}