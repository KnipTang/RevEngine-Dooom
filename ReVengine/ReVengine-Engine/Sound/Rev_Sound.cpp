#include "Rev_Sound.h"
#include "Rev_SoundImpl.h"

using namespace Rev;

Rev_Sound::Rev_Sound() :
	m_SoundImpl{ std::make_unique<RevDev::Rev_SoundImpl>() }
{

}

Rev_Sound::~Rev_Sound()
{

}

void Rev_Sound::LoadSound(const std::string& name, const std::string& source)
{
	m_SoundImpl->LoadSound(name, source);
}

void Rev_Sound::PlayRevSound(const std::string name)
{
	m_SoundImpl->PlaySound(name);
}

