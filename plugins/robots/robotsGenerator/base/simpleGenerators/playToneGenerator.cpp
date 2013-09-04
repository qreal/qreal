#include "playToneGenerator.h"

using namespace qReal::robots::generators::simple;

QString const defaultFrequency = "1000";
QString const defaultDuration = "100";
QString const defaultVolume = "50";

PlayToneGenerator::PlayToneGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "playTone.t", QList<Binding *>(), parent)
{
}

//QList<SmartLine_old> PlayToneGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
//		, qReal::Id const &elementId, qReal::Id const &logicElementId)
//{
//	QList<SmartLine_old> result;
//	QString const frequency = nxtGen->intExpression(logicElementId, "Frequency");
//	QString const duration = nxtGen->intExpression(logicElementId, "Duration");
//	QString const volume = nxtGen->intExpression(logicElementId, "Volume");
//	result.append(SmartLine_old(QString("ecrobot_sound_tone(%1, %2, %3);").arg(
//			(frequency.trimmed().isEmpty() ? defaultFrequency : frequency)
//			, (duration.trimmed().isEmpty() ? defaultDuration : duration)
//			, (volume.trimmed().isEmpty() ? defaultVolume : volume)), elementId));

//	return result;
//}
