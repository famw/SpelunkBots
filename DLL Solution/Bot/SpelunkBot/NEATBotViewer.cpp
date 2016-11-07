#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <string>

#include "NEATBotViewer.h"

NEATBotViewer::NEATBotViewer()
{
	currentOrganism = 0;
	InitializeNeat();
}

NEATBotViewer::~NEATBotViewer()
{
}

void NEATBotViewer::Update()
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

	// win condition
	if(GetNodeState(_playerPositionXNode, _playerPositionYNode, NODE_COORDS) == spExit)
	{
		organism->winner = true;
		_shouldSuicide = true;
	}

}

void NEATBotViewer::Reset()
{
	_goRight = false;
	_goLeft = false;
	_jump = false;
	_attack = false;
	_shouldSuicide = false;
	_run = false;
}

void NEATBotViewer::NewLevel()
{
	ResetExperiment();
}

void NEATBotViewer::InitializeNeat()
{
	std::cerr << "===============" << std::endl;
	std::cerr << "BEGIN NEAT SETUP..." << std::endl;

	std::cerr << "Creating population..." << std::endl;
	population = std::make_unique<NEAT::Population>("neat/gen_xxx.pop");
	population->verify();

	std::cerr << "NEAT SETUP IS COMPLETE." << std::endl;
	std::cerr << "===============" << std::endl << std::endl;

	organism = population->organisms.at(0);

	std::cerr << "Beginning evaluation..." << std::endl;
	std::cerr << "ORGANISM: " << currentOrganism << std::endl << std::endl;
}

void NEATBotViewer::ResetExperiment()
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

		// Reset
		std::cerr << "Restarting population..." << std::endl;
		currentOrganism = 0;
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

	std::cerr << "ORGANISM: " << currentOrganism << std::endl;
}

void NEATBotViewer::ConfigureInputs()
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
	// TODO(martin): implement this

	// bias input
	input[inputSize-1] = 1.0;
}

void NEATBotViewer::ConfigureOutputs()
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

			default: break; // shouldnt happen
		}
		current++;
	}
}

float NEATBotViewer::getFitness()
{
	// distance travelled (based on manhattan distance)
	float distX = std::fabs(startX - _playerPositionXNode);
	float distY = std::fabs(startY - _playerPositionYNode);
	float distance = distX + distY;
	float distance2 = distX + distY*2;
	float maxDistance = 39 + 31; // (x=39,y=31): (1,1) -> (40,32)
	float maxDistance2 = 39 + 31*2;
	float normalizedDistance = distance / maxDistance;
	if(GetNodeState(_playerPositionXNode, _playerPositionYNode, NODE_COORDS) == spExit)
	{
		normalizedDistance = 1.0f;
	}
	// sanity check
	if(distance <= 0.0f) distance = 0.0001f;
	if(distance2 <= 0.0f) distance = 0.0001f;
	std::cerr << "DISTANCE TRAVELLED: " << distance << std::endl;
	std::cerr << "DISTANCE TRAVELLED (Y*2): " << distance2 << std::endl;
	std::cerr << "NORMALIZED DISTANCE: " << normalizedDistance << std::endl;
	std::cerr << "NORMALIZED DISTANCE (Y*2): " << distance2/maxDistance2 << std::endl << std::endl;

	// time taken (penalize if idle)
	float normalizedTime = 0.0001f; // sanity check
	if(!IsIdleTooLong() && GetTimeElapsed() < GetTestSeconds()) 
	{
		normalizedTime = (GetTestSeconds() - GetTimeElapsed())
			/ GetTestSeconds();
	}
	std::cerr << "NORMALIZED TIME: " << normalizedTime << std::endl;

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
	std::cerr << "FITNESS (HM): " << fitnessHM << std::endl << std::endl;

	return fitnessHM;
}

bool NEATBotViewer::IsIdleTooLong()
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
