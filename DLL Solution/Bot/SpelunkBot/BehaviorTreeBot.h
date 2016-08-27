#pragma once

#include <memory>

#include "IBot.h"

#include "time.h"
#include "../lib/btsk/BehaviorTreeEvent.cpp"

class BehaviorTreeBot : public IBot
{
public:
	BehaviorTreeBot();
	~BehaviorTreeBot();
};

