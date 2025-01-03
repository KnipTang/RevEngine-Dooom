#pragma once

#include <memory>
#include <string>

namespace RevDev {
	class Rev_SoundImpl;
}

namespace Rev
{
	class Rev_Sound
	{
	public:
		Rev_Sound();
		~Rev_Sound();

		void PlayRevSound(const std::string name);
		void LoadSound(const std::string& name, const std::string& source);
	private:

		//Point of pImpl is so you dont have to include the SoLoud in the header file, in the cpp file doesnt matter if you include it there
		std::unique_ptr<RevDev::Rev_SoundImpl> pSoundImpl;
	};
}