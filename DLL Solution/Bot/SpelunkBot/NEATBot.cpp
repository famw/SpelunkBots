#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <string>

#include "NEATBot.h"

NEATBot::NEATBot()
{
	currentGeneration = 0;
	currentOrganism = 0;
	InitializeNeat();
}

NEATBot::~NEATBot()
{
}

void NEATBot::Update()
{
	// get entrance location
	if(isFirstFrame)
	{
		startPos= {(int)_playerPositionXNode, (int)_playerPositionYNode};
		lastPos = {0,0};
		lastTimeMoved = 0;
		states.clear();
		facingDirection = Direction::Right;
		inputDir = 0;

		isFirstFrame = false;
	}
	
	// abort if the bot is idle
	if(IsIdleTooLong()) _shouldSuicide = true;
		
	// neat core
	ConfigureInputs();
	organism->net->load_sensors(input);
	organism->net->activate();
	ConfigureOutputs();

	if(_goLeft) facingDirection = Direction::Left;
	else if(_goRight) facingDirection = Direction::Right;

	// win condition
	if(hasWon())
	{
		organism->winner = true;
		_shouldSuicide = true;
	}
}

void NEATBot::Reset()
{
	_goRight = false;
	_goLeft = false;
	_jump = false;
	_lookUp = false;
	_attack = false;
	_shouldSuicide = false;
	_run = false;
}

void NEATBot::NewLevel()
{
	ResetExperiment();
}

void NEATBot::InitializeNeat()
{
	std::cerr << "===============" << std::endl;
	std::cerr << "BEGIN NEAT SETUP..." << std::endl;

	// Seed the random-number generator
	//srand((unsigned)time(NULL));
	srand(12345);

	std::cerr << "Reading parameters file..." << std::endl;
	NEAT::load_neat_params("neat/neat_parameters.ne", false); // true = print params

	std::cerr << "Loading starter genome file..." << std::endl;
	genome = std::make_unique<NEAT::Genome>("neat/neat_startgenes");

	std::cerr << "Creating population..." << std::endl;
	population = std::make_unique<NEAT::Population>(genome.get(), NEAT::pop_size);
	//population = std::make_unique<NEAT::Population>("neat/gen_16.pop");
	// ^ example on how to read a population file with multiple genomes
	population->verify();

	std::cerr << "NEAT SETUP IS COMPLETE." << std::endl;
	std::cerr << "===============" << std::endl << std::endl;

	organism = population->organisms.at(0);

	std::cerr << "Beginning evaluation..." << std::endl;
	std::cerr << "GENERATION: " << currentGeneration << std::endl;
	std::cerr << "ORGANISM: " << currentOrganism << std::endl << std::endl;
}

void NEATBot::ResetExperiment()
{
	// Assign organism fitness
	organism->fitness = getFitness();

	std::cerr << "Ended current evaluation..." << std::endl;
	std::cerr << "FITNESS : " << organism->fitness << std::endl;
	std::cerr << "---------------" << std::endl;

	// If we have reached the end of the population
	if(currentOrganism == population->organisms.size() - 1)
	{
		std::cerr << "Reached end of population" << std::endl;
		std::cerr << "HIGHEST FITNESS: " << population->highest_fitness << std::endl;

		// Log generation's population
		std::string name = "neat/genomes/gen_" + std::to_string(currentGeneration) + ".pop";
		char *fileName = &name[0u]; // NEAT API requires char* ...
		population->print_to_file_by_species(fileName);
		delete fileName; // Make sure we deallocate this...

		// Activate population epoch
		std::cerr << "Calculating epoch..." << std::endl;
		population->epoch(currentGeneration);

		// Reset
		std::cerr << "Reseting population..." << std::endl;
		std::cerr << "Moving to new generation..." << std::endl << std::endl;
		currentOrganism = 0;
		currentGeneration++;

	}
	else
	{
		// Advance to next individual on population
		currentOrganism++;
	}

	// Get reference to current organism
	organism = population->organisms.at(currentOrganism);

	// Reset fitness score
	isFirstFrame = true;

	std::cerr << "GENERATION: " << currentGeneration << std::endl;
	std::cerr << "ORGANISM: " << currentOrganism << std::endl;
}

void NEATBot::ConfigureInputs()
{
	// sensorial input
	int currentInput = 0;
	for(int dy=-boxRadius; dy<=boxRadius; dy++)
	{
		for(int dx=-boxRadius; dx<=boxRadius; dx++)
		{
			int x = _playerPositionXNode + dx;
			int y = _playerPositionYNode + dy;

			int tile = 1; // start at solid block
			//bool enemy = 0;
			if(x >= 0 && x<=42 && y >=0 && y<=34) // level boundaries
			{
				tile = GetNodeState(x,y,0);
				//enemy = IsEnemyInNode(x,y,0);
			}

			// map out tiles to our desired sensorial inputs
			switch(tile)
			{
				case 0: tile=0; break;		// empty
				case 1: tile=1; break;		// solid
				case 2: tile=0; break;		// ladder
				case 3: tile=2; break;		// exit
				case 4: tile=0; break;		// entrance
				case 10: tile=-1; break;	// spikes
				default: tile=1; break;		// everything else - solid
			}

			input[currentInput++] = tile;
		}
	}


	// obstacle input
	/*int x = _playerPositionXNode + facingDirection;
	int y = _playerPositionYNode;
	int tile1 = GetNodeState(x, y, 0);
	int tile2 = GetNodeState(x, y-1, 0);
	int tile3 = GetNodeState(x, y-2, 0);
	if(tile1 == 1 && tile2 == 1 && tile3 == 1)
	{
		if(facingDirection == Direction::Left) inputDir = 1;
		else inputDir = -1;
	}
	input[currentInput++] = inputDir;*/


	// bias input
	input[inputSize-1] = 1.0;
}

void NEATBot::ConfigureOutputs()
{
	double activation = 0.0;
	int current = 0;
	auto outputs = organism->net->outputs;

	for(auto it = outputs.begin(); it != outputs.end(); it++)
	{
		activation = (*it)->activation;
		//std::cerr << curr << ": " << activation << std::endl;

		// set buttons accordingly
		switch(current)
		{
			case Movement:
				if(activation >= Activation::MoveLeftMin &&
					activation <= Activation::MoveLeftMax)
						_goLeft = true;
				else if(activation >= Activation::MoveRightMin &&
						activation <= Activation::MoveRightMax)
						_goRight = true;
				break;

			case Jump:
				if(activation >= Activation::JumpMin &&
					activation <= Activation::JumpMax)
						_jump = true;
				break;

			case Up:
				if(activation >= Activation::RunMin &&
					activation <= Activation::RunMax)
				{
					std::cout << "UP" << std::endl;
					_lookUp = true;
				}

			default: break; // shouldnt happen
		}
		current++;
	}
}

float NEATBot::getFitness()
{
	// gather information
	float timeElapsed = GetTimeElapsed();
	RunStatus status = getRunStatus(timeElapsed);
		

	// distance travelled fitness (based on manhattan distance)
	float distX = std::fabs(startPos.x - _playerPositionXNode);
	float distY = std::fabs(startPos.y - _playerPositionYNode);

	float distance = distX + distY; // normal manhattan distance
	float distance2 = distX + distY*2; // weighted manhattan distance
	if(distance <= 0) distance = 1;
	if(distance2 <= 0) distance2 = 1;

	float maxDistance = 39 + 31; // (x=39,y=31): (1,1) -> (40,32)
	float maxDistance2 = 39 + 31*2;

	float normalizedDistance = distance / maxDistance;
	float normalizedDistance2 = distance2 / maxDistance2;
	if(hasWon()) normalizedDistance = normalizedDistance2 = 1.0f;

	std::cerr << "DISTANCE TRAVELLED: " << distance << std::endl;
	std::cerr << "DISTANCE TRAVELLED (Y*2): " << distance2 << std::endl;
	std::cerr << "NORMALIZED DISTANCE: " << normalizedDistance << std::endl;
	std::cerr << "NORMALIZED DISTANCE (Y*2): " << normalizedDistance2 << std::endl << std::endl;

    
    // states explored fitness
    float statesExplored = states.size();
    if(statesExplored == 0) statesExplored = 1;
    float exploration = statesExplored / (statesExplored + 50.0f);

	std::cerr << "STATES EXPLORED: " << statesExplored << std::endl;
	std::cerr << "NORMALIZED STATES EXPLORED: " << exploration << std::endl << std::endl;


	// time taken fitness
	float normalizedTime, explorationTime;
    int m = 2, n = 3;
	// apply penalties according to results
	switch(status)
	{
		case RunStatus::Won:
			std::cerr << "WINNER" << std::endl;
			// winner (Alive && Tt < Tmax)
			normalizedTime = (GetTestSeconds() - timeElapsed) / GetTestSeconds();
			explorationTime = timeElapsed;
		break;

		case RunStatus::Explored:
			std::cerr << "WAS EXPLORING" << std::endl;
			// was exploring (Alive && Tt < Tmax)
			normalizedTime = 0.01f;
			explorationTime = GetTestSeconds() * m;
			break;
		
		case RunStatus::Idle:
			std::cerr << "IDLE" << std::endl;
			// idle
			normalizedTime = 0.001f;
			explorationTime = GetTestSeconds() * n;
			break;

		case RunStatus::Repeated:
			// Repeated too many states
			std::cerr << "REPEATED STATES" << std::endl;
			normalizedTime = 0.001f;
			explorationTime = GetTestSeconds() * n;
			break;

		case RunStatus::Died:
			// died
			std::cerr << "DIED" << std::endl;
			normalizedTime = 0.001f;
			explorationTime = GetTestSeconds() * n;
			break;

	}

	std::cerr << "NORMALIZED TIME: " << normalizedTime << std::endl;
	std::cerr << "EXPLORATION TIME: " << explorationTime << std::endl << std::endl;


	// calculate fitness
	float fitnessAM = (normalizedDistance + normalizedTime) / 2.0f;
	float fitnessWAM = 0.6f*normalizedDistance + 0.4f*normalizedTime;
	float fitnessDT = normalizedDistance / normalizedTime;
	float fitnessDT2 = normalizedDistance * normalizedTime;
	float fitnessDT3 = normalizedDistance * explorationTime;
	float fitnessHM = 2.0f/((1.0f/normalizedDistance)+(1.0f/normalizedTime));
    float fitnessEX = (GetTestSeconds() * (0.5f*normalizedDistance + 0.5f*exploration)) / explorationTime;

	std::cerr << "FITNESS (AM): " << fitnessAM << std::endl;
	std::cerr << "FITNESS (WAM): " << fitnessWAM << std::endl;
	std::cerr << "FITNESS (DT1): " << fitnessDT << std::endl;
	std::cerr << "FITNESS (DT2): " << fitnessDT2 << std::endl;
	std::cerr << "FITNESS (DT3): " << fitnessDT3 << std::endl;
	std::cerr << "FITNESS (HM): " << fitnessHM << std::endl;
	std::cerr << "FITNESS (EX): " << fitnessEX << std::endl << std::endl;

	return fitnessEX;
}

bool NEATBot::hasWon()
{
	return GetNodeState(_playerPositionXNode, _playerPositionYNode, NODE_COORDS) == spExit; 
}

NEATBot::RunStatus NEATBot::getRunStatus(float timeElapsed)
{
	RunStatus status;
	if(timeElapsed < GetTestSeconds() && organism->winner) status = RunStatus::Won;
	else if(timeElapsed >= GetTestSeconds() && !IsIdleTooLong()) status = RunStatus::Explored;
	else if(IsIdleTooLong()) status = RunStatus::Idle;
	else if(hasRepeatedStates()) status = RunStatus::Repeated;
	else status = RunStatus::Died;
	
	return status;
}

bool NEATBot::IsIdleTooLong()
{
	bool isIdle = false;

	Position currentPos{(int)_playerPositionXNode, (int)_playerPositionYNode};

	// If we changed positions (node)
	if(lastPos.x != currentPos.x || lastPos.y != currentPos.y)
	{
		// Update our last position
		lastPos.x = currentPos.x;
		lastPos.y = currentPos.y;
		// Update out last time moved
		lastTimeMoved = GetTimeElapsed();

		// add entry to map
		states[currentPos] += 1;
		// if we visit it too much, abort
		if(states[currentPos] > stateMaxVisit) isIdle = true;
	}
	// We have not moved
	else
	{
		// Check if it's been too long
		if(GetTimeElapsed() - lastTimeMoved >= maxIdleTime)
		{
			isIdle = true;
		}
	}
	
	return isIdle;
}

bool NEATBot::hasRepeatedStates()
{
	std::map<Position, int>::iterator it;
	for(it = states.begin(); it != states.end(); it++)
	{
		if(it->second > stateMaxVisit) return true;
	}
	return false;
}
