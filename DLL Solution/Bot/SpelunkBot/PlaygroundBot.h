#pragma once

#include "IBot.h"

class PlaygroundBot : public IBot
{
public:
	PlaygroundBot() { }
	virtual ~PlaygroundBot() { }

	void Update() override;
};
