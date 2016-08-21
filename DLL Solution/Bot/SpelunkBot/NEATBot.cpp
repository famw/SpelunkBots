#include <iostream>
#include <fstream>
#include "NEATBot.h"

NEATBot::NEATBot()
{
	std::cout << "NEATBot ctor" << std::endl;
	LoadNeat();
}

NEATBot::~NEATBot()
{
	std::cout << "NEATBot dtor" << std::endl;
}

void NEATBot::Update()
{
	std::cout << "Position: " << _playerPositionX << "," << _playerPositionY << std::endl;

	_goRight = true;

	if(GetNodeState(_playerPositionXNode, _playerPositionYNode, NODE_COORDS) == spExit)
	{
		std::cout << "Found the exit" << std::endl;
		_goRight = false;
	}
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
}

void NEATBot::LoadNeat()
{
	// Seed the random-number generator
	srand((unsigned)time(NULL));

	// Load parameters file
	NEAT::load_neat_params("neat_parameters.ne", true);

	// Load starter genome file
	char curword[20];
	int id;

	std::ifstream iFile("neat_startgenes", std::ios::in);
	std::cout << "Reading starter genome" << std::endl;
	iFile >> curword;
	iFile >> id;
	std::cout << "Reading Genome ID" << std::endl;
	genome = std::make_unique<NEAT::Genome>(id, iFile);
	iFile.close();

	genome->genome_id;
	genome->print_to_filename("test.genome");
}
