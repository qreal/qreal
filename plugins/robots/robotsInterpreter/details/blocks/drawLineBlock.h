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


class DrawLineBlock : public Block
{
	Q_OBJECT

public:
	DrawLineBlock(robotParts::Display &display);
	virtual ~DrawLineBlock() {}
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
