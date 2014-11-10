#pragma once

#include <robotModel/ev3RobotModelBase.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace ev3KitInterpreter {
namespace robotModel {
namespace real {

class RealRobotModel : public Ev3RobotModelBase
{
	Q_OBJECT

public:
	RealRobotModel(QString const &kitId, QString const &robotId);

	QString name() const override;
	QString friendlyName() const override;
	bool needsConnection() const override;

	void connectToRobot() override;
	void disconnectFromRobot() override;

public slots:
	void rereadSettings() override;

private:
	interpreterBase::robotModel::robotParts::Device *createDevice(
			interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &deviceInfo) override;

	// WARNING: This class must be disposed in the last turn so do not make it storing by value.
	utils::robotCommunication::RobotCommunicator *mRobotCommunicator;  // Takes ownership
	QString mLastCommunicationValue;
};

}
}
}
