#include "beepGenerator.h"
#include "../../trikRobotGenerator.h"

using namespace robots::trikGenerator;

BeepGenerator::BeepGenerator()
{
}

QList<SmartLine> BeepGenerator::convertElementIntoDirectCommand(TrikRobotGenerator *generator
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	return QList<SmartLine>() << SmartLine(QString("brick.playSound(\"beep-8.mp3\")"), logicElementId);
}
