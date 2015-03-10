#pragma once

#include <trikKit/robotModel/trikRobotModelBase.h>
#include <utils/tcpRobotCommunicator.h>

namespace trik {
namespace robotModel {
namespace real {

class RealRobotModelBase : public TrikRobotModelBase
{
	Q_OBJECT

public:
	RealRobotModelBase(const QString &kitId, const QString &robotId);

	bool needsConnection() const override;
	void connectToRobot() override;
	void stopRobot() override;
	void disconnectFromRobot() override;

	void setErrorReporter(qReal::ErrorReporterInterface *errorReporter);

protected:
	kitBase::robotModel::robotParts::Device *createDevice(
			const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &deviceInfo) override;

private:
	QScopedPointer<utils::TcpRobotCommunicator> mRobotCommunicator;
};

}
}
}
