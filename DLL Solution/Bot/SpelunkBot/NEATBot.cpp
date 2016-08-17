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
}
