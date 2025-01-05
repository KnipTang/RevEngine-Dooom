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
	class Rev_SoLoud final
	{
	public:
		Rev_SoLoud();
		~Rev_SoLoud();

		void PlaySound(const std::string name);
		void LoadSound(const std::string& name, const std::string& source);

	private:
		std::unique_ptr<SoLoud::Soloud> pRevDev_Soloud;

		std::unordered_map<std::string, std::unique_ptr<SoLoud::Wav>> m_LoadedAudio; //Normal pointer because SoLoud manages for you
	};
}