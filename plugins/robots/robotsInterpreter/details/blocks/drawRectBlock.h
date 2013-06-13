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


class DrawRectBlock : public Block
{
	Q_OBJECT

public:
	DrawRectBlock(robotParts::Display &display);
	virtual ~DrawRectBlock() {}
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
