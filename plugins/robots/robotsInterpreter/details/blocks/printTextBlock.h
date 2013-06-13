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


class PrintTextBlock : public Block
{
	Q_OBJECT

public:
	PrintTextBlock(robotParts::Display &display);
	virtual ~PrintTextBlock() {}
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
