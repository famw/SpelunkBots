#pragma once

#include <memory>

#include "IBot.h"

#include "time.h"
#include "../lib/BehaviorTree/src/BehaviorTree.h"

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
    void SetGoRight(bool value)
    {
        std::cout << "GOING RIGHT" << std::endl;
        _goRight = value;
    };

    BehaviorTree* bt;
};
