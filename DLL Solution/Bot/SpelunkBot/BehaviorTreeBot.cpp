#include "BehaviorTreeBot.h"

class MoveBehavior : public bt::Leaf
{
private:
	BehaviorTreeBot *bot;
	double prevXNode{-1};
	double prevYNode{-1};
	bool lookingRight{true};
public:
	MoveBehavior(BehaviorTreeBot *bot)
	{
		this->bot = bot;
	}

	bt::Node::Status update() override
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

        return bt::Node::Status::Success;
	}
};

BehaviorTreeBot::BehaviorTreeBot()
{
	std::cout << "Init BehaviorTreeBot" << std::endl;
	auto moveBehavior = std::make_shared<MoveBehavior>(this);
	tree.setRoot(moveBehavior);
}

BehaviorTreeBot::~BehaviorTreeBot()
{
	std::cout << "Destroy BehaviorTreeBot" << std::endl;
}

void BehaviorTreeBot::Update()
{
	tree.update();
}

void BehaviorTreeBot::Reset()
{
}

void BehaviorTreeBot::NewLevel()
{
}
