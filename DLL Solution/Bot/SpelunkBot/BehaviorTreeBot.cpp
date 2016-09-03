#include "BehaviorTreeBot.h"

using namespace std;
using namespace bt;

class GoRightBehavior : public Behavior
{
private:
    BehaviorTreeBot* bot;
public:
    GoRightBehavior(BehaviorTreeBot* bot)
    {
        this->bot = bot;
    }

    Status update()
    {
        std::cout << "UPDATE" << std::endl;
        bot->SetGoRight(true);
        return SUCCESS;
    }
};

BehaviorTreeBot::BehaviorTreeBot()
{
    Behavior* b = new GoRightBehavior(this);

    cout << "Init BehaviorTreeBot" << endl;
    bt = new BehaviorTree(*b);
}

BehaviorTreeBot::~BehaviorTreeBot()
{
    cout << "Destroy BehaviorTreeBot" << endl;
    //bt.stop();
}

void BehaviorTreeBot::Update()
{
    cout << "Update BehaviorTreeBot" << endl;
    bt->step();
}

void BehaviorTreeBot::Reset()
{
    cout << "Reset BehaviorTreeBot" << endl;
}

void BehaviorTreeBot::NewLevel()
{
    cout << "NewLevel BehaviorTreeBot" << endl;
}
