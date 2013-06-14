#pragma once

#include "../robotParts/robotModel.h"
#include "block.h"

namespace qReal
{
namespace interpreters
{
namespace robots
{
namespace details
{
namespace blocks
{


class DrawCircleBlock : public Block
{
	Q_OBJECT

public:
	DrawCircleBlock(robotParts::Display &display);
	virtual ~DrawCircleBlock() {}
	virtual void run();

protected:
	virtual QString name() const;

private:
	robotParts::Display &mDisplay;
};

}
}
}
}
}
