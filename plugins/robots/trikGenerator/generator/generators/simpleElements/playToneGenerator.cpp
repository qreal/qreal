#include "playToneGenerator.h"

#include "../../trikRobotGenerator.h"

using namespace robots::trikGenerator;

QString const defaultFrequency = "1000";
QString const defaultDuration = "100";
QString const defaultVolume = "50";


PlayToneGenerator::PlayToneGenerator()
{
}

QList<SmartLine> PlayToneGenerator::convertElementIntoDirectCommand(TrikRobotGenerator *generator
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	QString const fileName = generator->api()->stringProperty(logicElementId, "FileName");
	return QList<SmartLine>() << SmartLine(QString("brick.playSound(\"%1\");").arg(fileName), logicElementId);
}
