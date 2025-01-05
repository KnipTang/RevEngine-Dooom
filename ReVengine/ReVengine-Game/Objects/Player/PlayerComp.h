#pragma once

#include "GameObjects/BaseComponent.h"

class PlayerComp final: public Rev::BaseComponent
{
public:
	PlayerComp(Rev::GameObject* gameObj); 
	~PlayerComp() = default;
private:
};