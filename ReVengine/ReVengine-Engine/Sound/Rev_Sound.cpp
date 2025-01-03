#include "Rev_Sound.h"
#include "Rev_SoundImpl.h"

using namespace Rev;

Rev_Sound::Rev_Sound() :
	pSoundImpl{ std::make_unique<RevDev::Rev_SoundImpl>() }
{

}

Rev_Sound::~Rev_Sound()
{

}

void Rev_Sound::LoadSound(const std::string& name, const std::string& source)
{
	pSoundImpl->LoadSound(name, source);
}

void Rev_Sound::PlayRevSound(const std::string name)
{
	pSoundImpl->PlaySound(name);
}

