#pragma once

#include <QtCore/QObject>

#include <robotsInterpreterCore/blocks/block.h>
#include <robotsInterpreterCore/robotModel/robotModelInterface.h>

namespace robotsInterpreterCore {
namespace blocks {
namespace details {

class InitialBlock : public Block
{
	Q_OBJECT

public:
	InitialBlock(robotModel::RobotModelInterface &robotModel);
	virtual void run();

private:
	robotModel::RobotModelInterface &mRobotModel;
};

}
}
}
