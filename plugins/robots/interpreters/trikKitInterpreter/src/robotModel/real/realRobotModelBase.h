#pragma once

#include <QtCore/QScopedPointer>

#include <robotModel/trikRobotModelBase.h>
#include <utils/tcpRobotCommunicator.h>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

namespace trikKitInterpreter {
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
	interpreterBase::robotModel::robotParts::Device *createDevice(
			const interpreterBase::robotModel::PortInfo &port
			, const interpreterBase::robotModel::DeviceInfo &deviceInfo) override;

private:
	QScopedPointer<utils::TcpRobotCommunicator> mRobotCommunicator;
};

}
}
}
