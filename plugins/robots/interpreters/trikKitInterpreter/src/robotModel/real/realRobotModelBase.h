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
	RealRobotModelBase(QString const &kitId, QString const &robotId);

	bool needsConnection() const override;
	void connectToRobot() override;
	void stopRobot() override;
	void disconnectFromRobot() override;

	void setErrorReporter(qReal::ErrorReporterInterface *errorReporter);

protected:
	interpreterBase::robotModel::robotParts::Device *createDevice(
			interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &deviceInfo) override;

private:
	QScopedPointer<utils::TcpRobotCommunicator> mRobotCommunicator;
};

}
}
}
