#include "speaker.h"

#include <qrutils/inFile.h>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Speaker::Speaker(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikSpeaker(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
}

void Speaker::play(const QString &filePath)
{
	const QString pathToCommand = ":/trikQts/templates/playTone.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand)
			.replace("@@FILENAME@@", filePath) + "script.run();";

	mRobotCommunicator.runDirectCommand(directCommand);
}
