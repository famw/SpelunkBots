using namespace std;

#include "BehaviorTreeBot.h"

class GoRightBehavior : public bt4::Behavior
{
private:
    BehaviorTreeBot* bot;
public:
    GoRightBehavior(BehaviorTreeBot* bot)
    {
        this->bot = bot;
    }

    bt4::Status update()
    {
        std::cout << "UPDATE" << std::endl;
        bot->SetGoRight(true);
        return bt4::BH_SUCCESS;
    }
};

BehaviorTreeBot::BehaviorTreeBot()
{
    bt4::Behavior* b = new GoRightBehavior(this);

    cout << "Init BehaviorTreeBot" << endl;
    bt = bt4::BehaviorTree();
    bt.start(*b);
}

BehaviorTreeBot::~BehaviorTreeBot()
{
    cout << "Destroy BehaviorTreeBot" << endl;
    //bt.stop();
}

void BehaviorTreeBot::Update()
{
    cout << "Update BehaviorTreeBot" << endl;
    bt.step();
}

void BehaviorTreeBot::Reset()
{
    cout << "Reset BehaviorTreeBot" << endl;
}

void BehaviorTreeBot::NewLevel()
{
    cout << "NewLevel BehaviorTreeBot" << endl;
}
