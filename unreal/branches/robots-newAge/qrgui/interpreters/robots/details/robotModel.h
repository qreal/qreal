#pragma once

#include <QtCore/QObject>

#include "robotParts/brick.h"
#include "../robotCommunicationInterface.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class RobotModel : public QObject {
	Q_OBJECT

public:
	RobotModel(RobotCommunicationInterface * const robotCommunicationInterface);  // Gets ownership
	~RobotModel();

	void init();
	void clear();

	robotParts::Brick &brick();

signals:
	void connected();

private:
	RobotCommunicationInterface *mRobotCommunicationInterface;  // Has ownership
	robotParts::Brick mBrick;
};

}
}
}
}
