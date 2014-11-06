#include "speaker.h"

#include <qrutils/inFile.h>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Speaker::Speaker(DeviceInfo const &info, PortInfo const &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikSpeaker(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
}

void Speaker::play(QString const &filePath)
{
	QString const pathToCommand = ":/trik/templates/playTone.t";
	QString const directCommand = utils::InFile::readAll(pathToCommand)
			.replace("@@FILENAME@@", filePath) + "brick.run();";

	mRobotCommunicator.runDirectCommand(directCommand);
}
