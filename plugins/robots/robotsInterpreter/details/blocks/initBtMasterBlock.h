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

class InitBtMasterBlock : public Block
{
	Q_OBJECT
public:
	InitBtMasterBlock();
	virtual ~InitBtMasterBlock() {}
	virtual void run();

protected:
	virtual QString name() const;
};

}
}
}
}
}
