#pragma once

#include <memory>

#include "IBot.h"

#include "bt.h"

class BehaviorTreeBot : public IBot
{
public:
	BehaviorTreeBot();
	~BehaviorTreeBot();

	// SpelunkBots
	void Update()   override;
	void Reset()    override;
	void NewLevel() override;

	// Behavior Tree
	bt::BehaviorTree tree;
};
