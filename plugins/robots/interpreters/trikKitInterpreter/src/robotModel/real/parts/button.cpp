#include "button.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Button::Button(DeviceInfo const &info, PortInfo const &port
	, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotParts::Button(info, port), mRobotCommunicator(tcpRobotCommunicator)
{
}

void Button::read()
{
	emit newData(1);
}
