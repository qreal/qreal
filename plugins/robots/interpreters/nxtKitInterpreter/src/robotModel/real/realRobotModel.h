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
	RealRobotModel();

	QString name() const override;
	QString friendlyName() const override;
	bool needsConnection() const override;

	void disconnectFromRobot() override;

public slots:
	void rereadSettings() override;

private:
	void doConnectToRobot() override;

	void configureKnownDevices() override;

	utils::robotCommunication::RobotCommunicator mRobotCommunicator;
	QString mLastCommunicationValue;
};

}
}
}
