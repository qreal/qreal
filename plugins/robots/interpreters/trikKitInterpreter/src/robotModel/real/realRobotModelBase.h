#pragma once

#include <robotModel/trikRobotModelBase.h>
#include <utils/tcpRobotCommunicator.h>

#include <toolPluginInterface/usedInterfaces/errorReporterInterface.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {

class RealRobotModelBase : public TrikRobotModelBase
{
	Q_OBJECT

public:
	explicit RealRobotModelBase(QString const &kitId);

	bool needsConnection() const override;
	void connectToRobot() override;
	void disconnectFromRobot() override;

	void setErrorReporter(qReal::ErrorReporterInterface *errorReporter);

protected:
	interpreterBase::robotModel::robotParts::Device *createDevice(
			interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &deviceInfo) override;

private:
	utils::TcpRobotCommunicator *mRobotCommunicator;
};

}
}
}
