#include "stdafx.h"
#include "PlaygroundBot.h"

#include "../vendor/NEAT/innovation.h"

void PlaygroundBot::Update()
{
    std::cout << "I am a dummy bot" << std::endl;
    _goLeft = true;

    NEAT::Innovation inn = NEAT::Innovation(1, 2, 3, 4, 5, false);

    printf("%d\n", inn.node_in_id);
}
