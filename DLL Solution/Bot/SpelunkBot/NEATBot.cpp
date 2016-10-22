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
		startX = _playerPositionXNode;
		startY = _playerPositionYNode;
		isFirstFrame = false;
	}
	
	// abort if the bot is idle
	if(IsIdleTooLong()) _shouldSuicide = true;
		
	// neat core
	ConfigureInputs();
	organism->net->load_sensors(input);
	organism->net->activate();
	ConfigureOutputs();
	ExecuteOutputs();

	// win condition
	if(GetNodeState(_playerPositionXNode, _playerPositionYNode, NODE_COORDS) == spExit)
	{
		std::cout << "Found the exit" << std::endl;
		organism->winner = true;
		_shouldSuicide = true;
	}

}

void NEATBot::Reset()
{
	_goRight = false;
	_goLeft = false;
	_jump = false;
	_attack = false;

	_shouldSuicide = false;
	_run = false;
}

void NEATBot::NewLevel()
{
	std::cout << "Resetting level..." << std::endl;
	ResetExperiment();
}

void NEATBot::InitializeNeat()
{
	std::cout << "===============" << std::endl;
	std::cout << "BEGIN NEAT SETUP..." << std::endl;

	// Seed the random-number generator
	//srand((unsigned)time(NULL));
	srand(12345);

	std::cout << "Reading parameters file..." << std::endl;
	NEAT::load_neat_params("neat/neat_parameters.ne", false); // true = print params

	std::cout << "Loading starter genome file..." << std::endl;
	genome = std::make_unique<NEAT::Genome>("neat/neat_startgenes");

	std::cout << "Creating population..." << std::endl;
	population = std::make_unique<NEAT::Population>(genome.get(), NEAT::pop_size);
	//population = std::make_unique<NEAT::Population>("neat/gen_16.pop");
	// ^ example on how to read a population file with multiple genomes
	population->verify();

	std::cout << "NEAT SETUP IS COMPLETE." << std::endl;
	std::cout << "===============" << std::endl << std::endl;

	organism = population->organisms.at(0);

	std::cout << "Beginning evaluation..." << std::endl;
	std::cout << "GENERATION: " << currentGeneration << std::endl;
	std::cout << "ORGANISM: " << currentOrganism << std::endl;
}

void NEATBot::ResetExperiment()
{
	// Assign organism fitness
	organism->fitness = getFitness();

	std::cout << "Ended current evaluation..." << std::endl;
	std::cout << "FITNESS : " << organism->fitness << std::endl;
	std::cout << "---------------" << std::endl;

	// If we have reached the end of the population
	if(currentOrganism == population->organisms.size() - 1)
	{
		std::cout << "Reached end of population" << std::endl;
		std::cout << "HIGHEST FITNESS: " << population->highest_fitness << std::endl;

		// Log generation's population
		std::string name = "neat/genomes/gen_" + std::to_string(currentGeneration) + ".pop";
		char *fileName = &name[0u]; // NEAT API requires char* ...
		population->print_to_file_by_species(fileName);
		delete fileName; // Make sure we deallocate this...

		// Activate population epoch
		std::cout << "Calculating epoch..." << std::endl;
		population->epoch(currentGeneration);

		// Reset
		std::cout << "Reseting population..." << std::endl;
		std::cout << "Moving to new generation..." << std::endl << std::endl;
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

	std::cout << "Beginning evaluation..." << std::endl;
	std::cout << "GENERATION: " << currentGeneration << std::endl;
	std::cout << "ORGANISM: " << currentOrganism << std::endl;
}

void NEATBot::ConfigureInputs()
{
	int currentInput = 0;
	for(int dy=-inputRadius; dy<=inputRadius; dy++)
	{
		for(int dx=-inputRadius; dx<=inputRadius; dx++)
		{
			int x = _playerPositionXNode + dx;
			int y = _playerPositionYNode + dy;

			int tile = 0;
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
				case 1: tile=1; break;		// terrain
				case 3: tile=2; break;		// exit
				case 10: tile=-1; break;	// spikes
				default: tile=0; break;		// everything else
			}

			input[currentInput++] = tile;
		}
	}

	input[inputSize-1] = 1.0; // bias
}

void NEATBot::ConfigureOutputs()
{
	double activation = 0.0;

	int curr = 0;
	auto outputs = organism->net->outputs;
	for(auto it = outputs.begin(); it != outputs.end(); it++)
	{
		activation = (*it)->activation;
		//std::cout << curr << ": " << activation << std::endl;
		if(activation >= 0.5) output[curr] = true; // Sigmoid?
		else output[curr] = false;
		curr++;
	}
}

void NEATBot::ExecuteOutputs()
{
	// Debug
	if(false)
	{
		std::cout << "MOVE LEFT : " << output[MOVE_LEFT] << std::endl;
		std::cout << "MOVE RIGHT: " << output[MOVE_RIGHT] << std::endl;
		std::cout << "JUMP      : " << output[JUMP] << std::endl;
		std::cout << "-----" << std::endl;
	}

	if(output[MOVE_LEFT]) _goLeft = true;
	if(output[MOVE_RIGHT]) _goRight = true;
	if(output[JUMP]) _jump = true;
}

float NEATBot::getFitness()
{
	// distance travelled (based on manhattan distance)
	float distX = std::fabs(startX - _playerPositionXNode);
	float distY = std::fabs(startY - _playerPositionYNode);
	float distance = distX + distY;
	float distance2 = distX + distY*2;
	float distance3 = distX + distY*distY;
	float maxDistance = 39 + 31; // (x=39,y=31): (1,1) -> (40,32)
	float normalizedDistance = distance / maxDistance;
	if(GetNodeState(_playerPositionXNode, _playerPositionYNode, NODE_COORDS) == spExit)
	{
		normalizedDistance = 1.0f;
	}
	// sanity check
	if(distance <= 0.0f) distance = 0.0001f;
	if(distance2 <= 0.0f) distance = 0.0001f;
	if(distance3 <= 0.0f) distance = 0.0001f;
	std::cout << "DISTANCE TRAVELLED: " << distance << std::endl;
	std::cout << "DISTANCE TRAVELLED (Y*2): " << distance2 << std::endl;
	std::cout << "DISTANCE TRAVELLED (Y^2): " << distance3 << std::endl;
	std::cout << "NORMALIZED DISTANCE: " << normalizedDistance << std::endl;
	std::cout << "NORMALIZED DISTANCE (Y*2): " << distance2/maxDistance << std::endl;
	std::cout << "NORMALIZED DISTANCE (Y^2):" << distance3/maxDistance << std::endl;

	// time taken (penalize if idle)
	float normalizedTime = 0.0001f; // sanity check
	if(!IsIdleTooLong() && GetTimeElapsed() < GetTestSeconds()) 
	{
		normalizedTime = (GetTestSeconds() - GetTimeElapsed())
			/ GetTestSeconds();
	}

	std::cout << "NORMALIZED TIME: " << normalizedTime << std::endl;

	// calculate fitness
	float fitnessAM = (normalizedDistance + normalizedTime) / 2.0f;
	float fitnessWAM = 0.6f*normalizedDistance + 0.4f*normalizedTime;
	float fitnessDT = normalizedDistance / normalizedTime;
	float fitnessHM = 2.0f/((1.0f/normalizedDistance)+(1.0f/normalizedTime));
	// sanity check
	if(fitnessAM <= 0.0f) fitnessAM == 0.0001f;
	if(fitnessWAM <= 0.0f) fitnessWAM == 0.0001f;
	if(fitnessDT <= 0.0f) fitnessDT == 0.0001f;
	if(fitnessHM <= 0.0f) fitnessHM == 0.0001f;

	std::cerr << "FITNESS (AVG): " << fitnessAM << std::endl;
	std::cerr << "FITNESS (WEIGHTED AVG): " << fitnessWAM << std::endl;
	std::cerr << "FITNESS (D/T): " << fitnessDT << std::endl;
	std::cerr << "FITNESS (HM): " << fitnessHM << std::endl;

	return fitnessHM;
}

bool NEATBot::IsIdleTooLong()
{
	bool isIdle = false;

	// If we changed positions (node)
	if(lastX != _playerPositionXNode || lastY != _playerPositionYNode)
	{
		// Update our last position
		lastX = _playerPositionXNode;
		lastY = _playerPositionYNode;
		// Update out last time moved
		lastTimeMoved = GetTimeElapsed();
	}
	// We have not moved
	else
	{
		// Check if it's been too long
		if(GetTimeElapsed() - lastTimeMoved >= maxIdleTime)
		{
			isIdle = true;
			std::cout << "Player is idle" <<std::endl;
		}
	}
	
	return isIdle;
}
