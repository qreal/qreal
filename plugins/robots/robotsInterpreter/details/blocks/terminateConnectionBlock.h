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

class TerminateConnectionBlock : public Block
{
	Q_OBJECT
public:
	TerminateConnectionBlock();
	virtual ~TerminateConnectionBlock() {}
	virtual void run();

protected:
	virtual QString name() const;
};

}
}
}
}
}
