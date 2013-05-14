#include "beepGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

QString const defaultVolume = "50";

BeepGenerator::BeepGenerator()
{
}

QList<SmartLine> BeepGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QString const volume = nxtGen->intExpression(logicElementId, "Volume");
	return QList<SmartLine>() << SmartLine(QString("ecrobot_sound_tone(1000, 100, %1);")
			.arg(volume.isEmpty() ? defaultVolume : volume), elementId);
}
