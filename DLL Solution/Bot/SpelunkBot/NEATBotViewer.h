#pragma once

#include <memory>

#include "IBot.h"

// NEAT includes
#include "time.h"
#include "NEAT/neat.h"
#include "NEAT/population.h"
#include "NEAT/genome.h"
#include "NEATOutput.h"

class NEATBotViewer : public IBot
{
public:
	NEATBotViewer();
	~NEATBotViewer();

	// SpelunkBots
	void Update() override;
	void Reset() override;
	void NewLevel() override;

	// NEAT configurations
	void InitializeNeat();
	void ResetExperiment();
	void ConfigureInputs();
	void ConfigureOutputs();
	bool IsIdleTooLong();

	// NEAT experiment
	std::unique_ptr<NEAT::Population> population;
	std::unique_ptr<NEAT::Genome> genome;
	NEAT::Organism * organism; //TODO(Martin): remove this raw pointer :(
	int currentOrganism;

	// NEAT sensorial information
	static const int boxRadius{2};
	static const int inputBoxSize{(boxRadius*2+1)*(boxRadius*2+1)};
	static const int inputSize{inputBoxSize+1}; // +1 -> bias input node
	double input[inputSize];

	// NEAT fitness
	float getFitness();
	bool isFirstFrame{true};
	int startX{0}, startY{0};

	// NEAT idle control
	double lastX{0}, lastY{0};
	double lastTimeMoved{0};
	double maxIdleTime{5};
};

