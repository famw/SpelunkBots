#include <iostream>
#include <fstream>
#include "NEATBot.h"

NEATBot::NEATBot()
{
	InitializeNeat();
	currentGeneration = 1;
	currentOrganism = 1;
	currentFitness = 0.0f;
}

NEATBot::~NEATBot()
{
}

void NEATBot::Update()
{
	//std::cout << "Position: " << _playerPositionX << "," << _playerPositionY << std::endl;

	// If the player is at the exit
	/*if(GetNodeState(_playerPositionXNode, _playerPositionYNode, NODE_COORDS) == spExit)
	{
		// He won :)
		std::cout << "Found the exit" << std::endl;
		organism->winner = true;
	}*/

	// Set up inputs (sensory nodes)
	// TODO(Martin): set this up properly
	input[0] = 1.0; // Bias
	input[1] = 1.0;

	// Load network with inputs
	organism->net->load_sensors(input);

	// Activate network
	organism->net->activate();

	// Set up outputs
	// TODO(Martin): set this up properly
	
	// Act based on outputs
	// TODO(Martin): link Spelunky actions with organism's network outputs
}

void NEATBot::Reset()
{
	_goRight = false;
	_goLeft = false;
	_jump = false;
	_attack = false;
}

void NEATBot::NewLevel()
{
	std::cout << "Entering new level..." << std::endl;
	ResetExperiment();
}

void NEATBot::InitializeNeat()
{
	// Seed the random-number generator
	srand((unsigned)time(NULL));

	// Load parameters file
	NEAT::load_neat_params("neat_parameters.ne", false);

	// Load starter genome file
	char curword[20];
	int id;
	std::ifstream iFile("neat_startgenes", std::ios::in);
	iFile >> curword;
	iFile >> id;
	genome = std::make_unique<NEAT::Genome>(id, iFile);
	iFile.close();

	// Spawn the first population from the starter gene
	population = std::make_unique<NEAT::Population>(genome.get(), NEAT::pop_size);
	population->verify();
}

void NEATBot::ResetExperiment()
{
	// Assign organism fitness
	organism->fitness = currentFitness;

	// If we have reached the end of the population
	if(currentOrganism > NEAT::pop_size)
	{
		// Activate population epoch
		population->epoch(currentGeneration);

		// TODO(Martin): log stats properly
		population->print_to_file_by_species("test.pop");

		// Reset our population
		currentOrganism = 1;
		// Advance to next generation
		currentGeneration++;

	}
	else
	{
		// Advance to next individual on population
		currentOrganism++;
	}

	// Get reference to current organism
	organism = population->organisms.at(currentOrganism-1);

	// Reset fitness score
	currentFitness = 0.0f;
}
