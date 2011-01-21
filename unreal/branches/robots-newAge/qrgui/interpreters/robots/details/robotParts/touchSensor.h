#pragma once

#include <QtCore/QObject>

#include "../../../../kernel/ids.h"

#include "robotPart.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class TouchSensor : public RobotPart
{
	Q_OBJECT

public:
	TouchSensor(RobotCommunicationInterface *robotCommunicationInterface);
	void configure(int port);
	void read();

signals:
	void response(int reading);
	void configured();

private:
	enum State {
		idle
		, pending
	};

	State mState;
	int mPort;

	virtual void processResponse(QByteArray const &reading);
};

}
}
}
}
}
