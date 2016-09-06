#include "BehaviorTreeBot.h"

using namespace std;
using namespace bt;

class MoveBehavior : public Behavior
{
private:
    BehaviorTreeBot* bot;
    double prevXNode = -1;
    double prevYNode = -1;
    bool lookingRight = true;
public:
    MoveBehavior(BehaviorTreeBot* bot)
    {
        this->bot = bot;
    }

    Status update()
    {
        double x = this->bot->_playerPositionXNode;
        double y = this->bot->_playerPositionYNode;

        if(this->bot->_goRight)
        {
            double nextNodeState = this->bot->GetNodeState(x + 1, y, false);

            if(nextNodeState == spEmptyNode) {
                this->bot->_goRight = true;
            } else {
                this->bot->_goRight = false;
                this->bot->_goLeft = true;
            }
        }
        else
        {
            double nextNodeState = this->bot->GetNodeState(x - 1, y, false);

            if(nextNodeState == spEmptyNode) {
                this->bot->_goLeft = true;
            } else {
                this->bot->_goRight = true;
                this->bot->_goLeft = false;
            }
        }

        return SUCCESS;
    }
};

BehaviorTreeBot::BehaviorTreeBot()
{
    Sequence* s = new Sequence();
    Behavior* m = new MoveBehavior(this);

    s->add(m);

    cout << "Init BehaviorTreeBot" << endl;
    bt = new BehaviorTree(*s);
}

BehaviorTreeBot::~BehaviorTreeBot()
{
    cout << "Destroy BehaviorTreeBot" << endl;
    //bt.stop();
}

void BehaviorTreeBot::Update()
{
    bt->step();
}

void BehaviorTreeBot::Reset()
{
}

void BehaviorTreeBot::NewLevel()
{
}
