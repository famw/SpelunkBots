#include "stdafx.h"
#include "PlaygroundBot.h"

#include "../lib/NEAT/innovation.h"

void PlaygroundBot::Update()
{
    std::cout << "I am a dummy bot " << _playerPositionX << "," << _playerPositionY << std::endl;
    _goRight = true;

    if(GetNodeState(_playerPositionXNode, _playerPositionYNode, NODE_COORDS) == spExit) {
        std::cout << "FOUND IT" << std::endl;
        _goRight = false;
        _lookUp = true;
    }

    NEAT::Innovation inn = NEAT::Innovation(1, 2, 3, 4, 5, false);

    printf("%d\n", inn.node_in_id);
}
