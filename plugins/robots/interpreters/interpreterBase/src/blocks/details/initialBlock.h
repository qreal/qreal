#pragma once

#include <QtCore/QObject>

#include <interpreterBase/blocks/block.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

namespace interpreterBase {
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
