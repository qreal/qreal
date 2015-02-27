#pragma once

#include <ev3Kit/robotModel/ev3RobotModelBase.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace ev3 {
namespace robotModel {
namespace real {

class RealRobotModel : public Ev3RobotModelBase
{
	Q_OBJECT

public:
	RealRobotModel(const QString &kitId, const QString &robotId);

	QString name() const override;
	QString friendlyName() const override;
	bool needsConnection() const override;

	void connectToRobot() override;
	void disconnectFromRobot() override;

public slots:
	void rereadSettings() override;

private:
	kitBase::robotModel::robotParts::Device *createDevice(
			const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &deviceInfo) override;

	// WARNING: This class must be disposed in the last turn so do not make it storing by value.
	utils::robotCommunication::RobotCommunicator *mRobotCommunicator;  // Takes ownership
	QString mLastCommunicationValue;
};

}
}
}
