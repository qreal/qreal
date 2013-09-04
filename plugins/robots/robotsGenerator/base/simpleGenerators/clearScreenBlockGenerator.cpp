#include "clearScreenBlockGenerator.h"

using namespace qReal::robots::generators::simple;

ClearScreenBlockGenerator::ClearScreenBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "clear.t", QList<Binding *>(), parent)
{
}

//QList<SmartLine_old> ClearScreenBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
//		, qReal::Id const &elementId, qReal::Id const &logicElementId)
//{
//	Q_UNUSED(logicElementId)
//	QList<SmartLine_old> result;
//	if(nxtGen->imageGenerator().bmpFilesCount()) {
//		result.append(SmartLine_old("memset(lcd, 0x00, sizeof(lcd));", elementId));
//		result.append(SmartLine_old("memset(lcd_copy, 0x00, sizeof(lcd));", elementId));
//	}
//	result.append(SmartLine_old("display_clear(1);", elementId));

//	return result;
//}
