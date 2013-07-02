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


class DrawPixelBlock : public Block
{
	Q_OBJECT

public:
	DrawPixelBlock(robotParts::Display &display);
	virtual ~DrawPixelBlock() {}
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
