#include "printTextBlockGenerator.h"

using namespace qReal::robots::generators::simple;

PrintTextBlockGenerator::PrintTextBlockGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(model, customizer, id, "printText.t", QList<Binding *>(), parent)
{
}

//QList<SmartLine_old> PrintTextBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
//		, qReal::Id const &elementId, qReal::Id const &logicElementId)
//{
//	QList<SmartLine_old> result;

//	QString const x = nxtGen->intExpression(logicElementId, "XCoordinateText");
//	QString const y = nxtGen->intExpression(logicElementId, "YCoordinateText");
//	QString text = nxtGen->api()->stringProperty(logicElementId, "PrintText").toUtf8();

//	result.append(SmartLine_old("display_goto_xy(" + x + ", " + y + ");", elementId));
//	result.append(SmartLine_old("display_string(\"" + text.replace("\"", "\\\"") + "\");", elementId));
//	result.append(SmartLine_old("display_update();", elementId));

//	return result;
//}
