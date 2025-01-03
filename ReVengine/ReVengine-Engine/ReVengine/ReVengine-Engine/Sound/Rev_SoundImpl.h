#pragma once

#include <memory>
#include <string>

namespace RevDev
{
	class Rev_SoLoud;
}

namespace RevDev
{
	class Rev_SoundImpl
	{
	public:
		Rev_SoundImpl();
		~Rev_SoundImpl(); //Can't delete destructor when using unique ptr (never delete destructor maybe lol)

		void PlaySound(const std::string name);
		void LoadSound(const std::string& name, const std::string& source);
	private:
		bool FileValid(const std::string source);

		std::unique_ptr<RevDev::Rev_SoLoud> pSoLoud;

	};
}
