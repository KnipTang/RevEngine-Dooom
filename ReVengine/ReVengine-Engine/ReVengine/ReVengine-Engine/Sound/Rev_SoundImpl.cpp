#include "Rev_SoundImpl.H"
#include "SoLoud/Rev_SoLoud.h"
#include <iostream>

using namespace RevDev;

Rev_SoundImpl::Rev_SoundImpl()
{
	pSoLoud = std::make_unique<RevDev::Rev_SoLoud>();
}

Rev_SoundImpl::~Rev_SoundImpl()
{

}

void Rev_SoundImpl::LoadSound(const std::string& name, const std::string& source)
{
	if (!FileValid(source))
	{
		std::cerr << "source file not found";
	}
	else
		pSoLoud->LoadSound(name, source);
}

void Rev_SoundImpl::PlaySound(const std::string name)
{
	return pSoLoud->PlaySound(name);
}

bool Rev_SoundImpl::FileValid(const std::string source)
{
	struct stat buffer;
	return (stat(source.c_str(), &buffer) == 0);
}