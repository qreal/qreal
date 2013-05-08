#include "playToneGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

QString const defaultFrequency = "1000";
QString const defaultDuration = "100";
QString const defaultVolume = "50";


PlayToneGenerator::PlayToneGenerator()
{
}

QList<SmartLine> PlayToneGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	QString const frequency = nxtGen->intExpression(logicElementId, "Frequency");
	QString const duration = nxtGen->intExpression(logicElementId, "Duration");
	QString const volume = nxtGen->intExpression(logicElementId, "Volume");
	result.append(SmartLine(QString("ecrobot_sound_tone(%1, %2, %3);").arg(
			(frequency.trimmed().isEmpty() ? defaultFrequency : frequency)
			, (duration.trimmed().isEmpty() ? defaultDuration : duration)
			, (volume.trimmed().isEmpty() ? defaultVolume : volume)), elementId));

	return result;
}
