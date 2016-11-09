#pragma once

#include <memory>
#include <map>

#include "IBot.h"

// NEAT includes
#include "time.h"
#include "NEAT/neat.h"
#include "NEAT/population.h"
#include "NEAT/genome.h"
#include "NEATOutput.h"

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
	bool IsIdleTooLong();

	// NEAT experiment control
	std::unique_ptr<NEAT::Population> population;
	std::unique_ptr<NEAT::Genome> genome;
	NEAT::Organism * organism; //TODO(Martin): remove this raw pointer :(
	int currentGeneration;
	int currentOrganism;

	// NEAT sensorial information
	static const int boxRadius{2};
	static const int inputBoxSize{(boxRadius*2+1)*(boxRadius*2+1)};
	static const int inputSize{inputBoxSize+1}; // +1 -> bias input node
	double input[inputSize];

	// obstacle detection
	enum Direction { Left = -1, Right = 1 };
	Direction facingDirection{Right};
	int inputDir{0};

	
	struct Position
	{
		int x; int y;
		bool operator<(const Position & other) const
		{
			if(this->x == other.x) return this->y < other.y;
			return this->x < other.x;
		}
	};

	// NEAT fitness
	float getFitness();
	bool isFirstFrame{true};
	Position startPos{0,0};

	// NEAT idle control
	Position lastPos{0,0};
	double lastTimeMoved{0};
	double maxIdleTime{3};
	std::map<Position, int> states;
	int stateMaxVisit{10};
};

