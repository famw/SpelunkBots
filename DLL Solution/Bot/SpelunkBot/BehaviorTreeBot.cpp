using namespace std;

#include "BehaviorTreeBot.h"

class Behavior : bt4::Behavior
{
};

BehaviorTreeBot::BehaviorTreeBot()
{
    cout << "Init BehaviorTreeBot" << endl;
}

BehaviorTreeBot::~BehaviorTreeBot()
{
    cout << "Destroy BehaviorTreeBot" << endl;
}

void BehaviorTreeBot::Update()
{
    cout << "Update BehaviorTreeBot" << endl;
}

void BehaviorTreeBot::Reset()
{
    cout << "Reset BehaviorTreeBot" << endl;
}

void BehaviorTreeBot::NewLevel()
{
    cout << "NewLevel BehaviorTreeBot" << endl;
}
