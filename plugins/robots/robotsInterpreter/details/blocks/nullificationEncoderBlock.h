#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "block.h"
#include "../robotParts/robotModel.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class NullificationEncoderBlock : public Block
{
	Q_OBJECT

public:
	NullificationEncoderBlock(RobotModel *  const robotModel);
	virtual void run();

private:
	robotParts::EncoderSensor * mEncoderSensor;  // Doesn't have ownership
	RobotModel * const mRobotModel;
};

}
}
}
}
}
