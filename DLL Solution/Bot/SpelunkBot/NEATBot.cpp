#include <iostream>
#include <fstream>
#include <string>

#include "NEATBot.h"

NEATBot::NEATBot()
{
	currentGeneration = 0;
	currentOrganism = 0;
	currentFitness = 1.0f;
	InitializeNeat();
}

NEATBot::~NEATBot()
{
}

void NEATBot::Update()
{
	//GetTickCount();
	//std::cout << "Time elapsed: " << GetTimeElapsed() << std::endl;
	//std::cout << "Time left: " << GetSecondsLeft() << std::endl;
	
	if(IsIdleTooLong())
	{
		std::cout << "PLAYER IS IDLE!" << std::endl;
		_shouldSuicide = true;
	}
		
	// Set up inputs (sensory nodes)
	ConfigureInputs();
	// Load network with inputs
	organism->net->load_sensors(input);
	// Activate network
	organism->net->activate();

	// Set up outputs
	ConfigureOutputs();
	// Act based on outputs
	ExecuteOutputs();
	
	// Update organism's fitness
	UpdateFitness();
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

	// Load parameters file
	std::cout << "Reading parameters file..." << std::endl;
	NEAT::load_neat_params("neat/neat_parameters.ne", false); // true = print params

	// Load starter genome file
	std::cout << "Loading starter genome file..." << std::endl;
	char curword[20];
	int id;
	std::ifstream iFile("neat/neat_startgenes", std::ios::in);
	iFile >> curword;
	iFile >> id;
	genome = std::make_unique<NEAT::Genome>(id, iFile);
	iFile.close();

	// Spawn the first population from the starter gene
	std::cout << "Creating population..." << std::endl;
	population = std::make_unique<NEAT::Population>(genome.get(), NEAT::pop_size);
	population->verify();

	std::cout << "NEAT SETUP IS COMPLETE." << std::endl;
	std::cout << "===============" << std::endl << std::endl;

	// Get reference to current organism
	organism = population->organisms.at(0);

	std::cout << "Beginning evaluation..." << std::endl;
	std::cout << "GENERATION: " << currentGeneration << std::endl;
	std::cout << "ORGANISM: " << currentOrganism << std::endl;
}

void NEATBot::ResetExperiment()
{
	// Assign organism fitness
	organism->fitness = currentFitness;

	std::cout << "Ended current evaluation..." << std::endl;
	std::cout << "FITNESS : " << currentFitness << std::endl;
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
	currentFitness = 1.0f;

	std::cout << "Beginning evaluation..." << std::endl;
	std::cout << "GENERATION: " << currentGeneration << std::endl;
	std::cout << "ORGANISM: " << currentOrganism << std::endl;
}

void NEATBot::ConfigureInputs()
{
	input[0] = 1.0; // Bias
	input[1] = 1.0;
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

void NEATBot::UpdateFitness()
{
	// Check for win condition
	if(GetNodeState(_playerPositionXNode, _playerPositionYNode, NODE_COORDS) == spExit)
	{
		std::cout << "Found the exit" << std::endl;
		currentFitness += scoreExit;
		organism->winner = true;
		_shouldSuicide = true;
	}
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
		}
	}
	
	return isIdle;
}
