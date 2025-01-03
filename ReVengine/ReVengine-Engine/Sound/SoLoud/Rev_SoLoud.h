#pragma once
#include <unordered_map>
#include <memory>
#include <string>

namespace SoLoud {
	class Soloud;
	class Wav;
	class AudioSource;
}

namespace RevDev
{
	class Rev_SoLoud
	{
	public:
		Rev_SoLoud();
		~Rev_SoLoud();

		void PlaySound(const std::string name);
		void LoadSound(const std::string& name, const std::string& source);

	private:
		std::unique_ptr<SoLoud::Soloud> pRevDev_Soloud;
		std::unique_ptr<SoLoud::Wav> pRevDev_Wave;

		std::unordered_map<std::string, SoLoud::AudioSource*> m_LoadedAudio; //Normal pointer because SoLoud manages for you
	};
}