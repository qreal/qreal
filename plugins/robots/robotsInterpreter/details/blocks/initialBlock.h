#pragma once

#include <QtCore/QObject>

#include "block.h"
#include "../robotParts/robotModel.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class InitialBlock : public Block
{
	Q_OBJECT

public:
	InitialBlock(RobotModel &robotModel);
	virtual void run();

private:
	RobotModel &mRobotModel;
};

}
}
}
}
}
