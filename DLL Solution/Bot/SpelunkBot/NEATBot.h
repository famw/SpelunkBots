#pragma once

#include <memory>

#include "IBot.h"

// NEAT includes
#include "time.h"
#include "../lib/NEAT/neat.h"
#include "../lib/NEAT/population.h"
#include "../lib/NEAT/genome.h"

enum OUTPUT
{
	MOVE_LEFT,
	MOVE_RIGHT,
	LOOK_UP,
	
	OUTPUT_MAX
};

class NEATBot : public IBot
{
public:
	NEATBot();
	~NEATBot();

	// SpelunkBots
	void Update() override;
	void Reset() override;
	void NewLevel() override;

	// NEAT configurations
	void InitializeNeat();
	void ResetExperiment();

	// NEAT experiment
	std::unique_ptr<NEAT::Population> population;
	std::unique_ptr<NEAT::Genome> genome;
	NEAT::Organism * organism; //TODO(Martin): remove this raw pointer :(
	int currentGeneration;
	int currentOrganism;
	float currentFitness;

	// Controls
	double input[2];
	bool output[OUTPUT_MAX];
};

