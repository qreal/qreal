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


class ClearScreenBlock : public Block
{
	Q_OBJECT
public:
	ClearScreenBlock(robotParts::Display &display);
	virtual ~ClearScreenBlock() {}
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
