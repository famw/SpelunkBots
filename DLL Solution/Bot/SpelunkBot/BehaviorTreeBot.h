#pragma once

#include <memory>

#include "IBot.h"

#include "time.h"
#include "../lib/BehaviorTree/src/BehaviorTree.h"
#include "../lib/BehaviorTree/src/Selector.h"
#include "../lib/BehaviorTree/src/Sequence.h"

using namespace bt;

class BehaviorTreeBot : public IBot
{
public:
	BehaviorTreeBot();
	~BehaviorTreeBot();

	// SpelunkBots
	void Update()   override;
	void Reset()    override;
	void NewLevel() override;

    // BehaviorTree experiment
    BehaviorTree* bt;
};
