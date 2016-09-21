#pragma once

#include <memory>

#include "IBot.h"

// NEAT includes
#include "time.h"
#include "NEAT/neat.h"
#include "NEAT/population.h"
#include "NEAT/genome.h"

enum Output
{
	MOVE_LEFT,
	MOVE_RIGHT,
	JUMP,
	
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
	void ConfigureInputs();
	void ConfigureOutputs();
	void ExecuteOutputs();
	void UpdateFitness();
	bool IsIdleTooLong();

	// NEAT experiment
	std::unique_ptr<NEAT::Population> population;
	std::unique_ptr<NEAT::Genome> genome;
	NEAT::Organism * organism; //TODO(Martin): remove this raw pointer :(
	int currentGeneration;
	int currentOrganism;
	float currentFitness;

	// NEAT controls
	double input[2];
	bool output[OUTPUT_MAX];

	// NEAT fitness values
	int scoreExit{1000};

	// NEAT idle control
	double lastX{0}, lastY{0};
	double lastTimeMoved{0};
	double maxIdleTime{2.0};
};

