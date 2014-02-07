#pragma once

#include <QtCore/QObject>
#include <QtCore/QVector>

#include <interpreterBase/blocksBase/block.h>
#include <interpreterBase/robotModel/robotParts/motor.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// @todo Move abstract blocks to interpreterBase::blocksBase
class EngineCommandBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	EngineCommandBlock(/*robotParts::Motor &motor1, robotParts::Motor &motor2, robotParts::Motor &motor3*/);
	virtual void run() = 0;

protected slots:
	void timeout();

protected:
//	QVector<robotParts::Motor *> mMotors;  // Doesn't have ownership.
};

}
}
}
