#pragma once

#include <robotModel/nxtRobotModelBase.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {

class RealRobotModel : public NxtRobotModelBase
{
	Q_OBJECT

public:
	QString name() const override;
	QString friendlyName() const override;
	bool needsConnection() const override;

public slots:
	void rereadSettings() override;

private:
	void configureKnownDevices() override;

	utils::robotCommunication::RobotCommunicator mRobotCommunicator;
	QString mLastCommunicationValue;
};

}
}
}
