#pragma once

#include <QtCore/QObject>
#include <QtCore/QByteArray>

#include <utils/robotCommunication/robotCommunicationThreadInterface.h>
#include <interpreterBase/robotModel/portInfo.h>

namespace nxtKitInterpreter {
namespace communication {

/// @todo: something wrong here...
class RobotCommunicationThreadBase : public utils::robotCommunication::RobotCommunicationThreadInterface
{
protected:
	virtual void send(QByteArray const &buffer, unsigned const responseSize, QByteArray &outputBuffer) = 0;
};

}
}
