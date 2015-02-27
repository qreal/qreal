#pragma once

#include <nxtKit/robotModel/nxtRobotModelBase.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace nxt {
namespace robotModel {
namespace real {

class RealRobotModel : public NxtRobotModelBase
{
	Q_OBJECT

public:
	RealRobotModel(const QString &kitId, const QString &robotId);

	QString name() const override;
	QString friendlyName() const override;
	bool needsConnection() const override;

	void connectToRobot() override;
	void disconnectFromRobot() override;

	/// Checks if connection can be established or emits errorOccured();
	void checkConnection();

signals:
	/// Emitted when communicator throws an error to be displayed with error reporter.
	void errorOccured(const QString &text);

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
