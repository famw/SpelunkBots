#pragma once

#include <memory>

#include "IBot.h"

// NEAT includes
#include "time.h"
#include "../lib/NEAT/neat.h"
#include "../lib/NEAT/population.h"
#include "../lib/NEAT/genome.h"

class NEATBot : public IBot
{
public:
	NEATBot();
	~NEATBot();

	void Update() override;
	void Reset() override;
	void NewLevel() override;

	void LoadNeat();

	std::unique_ptr<NEAT::Population> pop;
	std::unique_ptr<NEAT::Genome> genome;
};

