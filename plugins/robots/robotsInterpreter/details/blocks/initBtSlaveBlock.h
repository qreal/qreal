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

class InitBtSlaveBlock : public Block
{
	Q_OBJECT
public:
	InitBtSlaveBlock();
	virtual ~InitBtSlaveBlock() {}
	virtual void run();

protected:
	virtual QString name() const;
};

}
}
}
}
}
