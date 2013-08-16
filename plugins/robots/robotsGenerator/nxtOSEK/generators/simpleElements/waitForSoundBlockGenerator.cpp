#include "waitForSoundBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

WaitForSoundBlockGenerator::WaitForSoundBlockGenerator()
{
}

QList<SmartLine_old> WaitForSoundBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	QList<SmartLine_old> result;

	QString const port = "NXT_PORT_S" + nxtGen->api()->stringProperty(logicElementId, "Port");

	QString const volume = nxtGen->api()->stringProperty(logicElementId, "Volume");
	QString const inequalitySign = transformSign(QString(nxtGen->api()->stringProperty(logicElementId
			, "Sign").toUtf8()));

	QString const condition = inequalitySign + " " + volume;

	result.append(SmartLine_old("while (!(ecrobot_get_sound_sensor(" + port
			+ ") * 100 / 1023 " + condition + "))", elementId));
	result.append(SmartLine_old("{", elementId));
	result.append(SmartLine_old("}", elementId));

	return result;
}
