#include "constraintsRobotsDiagram_2.h"
#include <QtCore/QVariant>

using namespace constraints;

ConstraintsRobotsDiagram_2::ConstraintsRobotsDiagram_2()
{
}

ConstraintsRobotsDiagram_2::~ConstraintsRobotsDiagram_2()
{
}

qReal::CheckStatus ConstraintsRobotsDiagram_2::checkEnginesForward_1(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi)
{
	bool res = true;
		bool propertyNodeRes_1 = true;
	if (logicalApi.hasProperty(element, "Power")) {
		propertyNodeRes_1 = (logicalApi.property(element, "Power").toInt() <= 100);
	}
	bool propertyNodeRes_2 = true;
	if (logicalApi.hasProperty(element, "Power")) {
		propertyNodeRes_2 = (logicalApi.property(element, "Power").toInt() >= -100);
	}
		bool nodeRes_1 = propertyNodeRes_1 && propertyNodeRes_2;
	res = nodeRes_1;

	return qReal::CheckStatus(res, QObject::tr("EnginesForward must have Power between -100 and 100"), qReal::CheckStatus::warning);
}

qReal::CheckStatus ConstraintsRobotsDiagram_2::checkEnginesBackward_1(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi)
{
	bool res = true;
		bool propertyNodeRes_1 = true;
	if (logicalApi.hasProperty(element, "Power")) {
		propertyNodeRes_1 = (logicalApi.property(element, "Power").toInt() <= 100);
	}
	bool propertyNodeRes_2 = true;
	if (logicalApi.hasProperty(element, "Power")) {
		propertyNodeRes_2 = (logicalApi.property(element, "Power").toInt() >= -100);
	}
		bool nodeRes_1 = propertyNodeRes_1 && propertyNodeRes_2;
	res = nodeRes_1;

	return qReal::CheckStatus(res, QObject::tr("EnginesBackward must have Power between -100 and 100"), qReal::CheckStatus::warning);
}

QList<qReal::CheckStatus> ConstraintsRobotsDiagram_2::checkEnginesBackward(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi)
{
	QList<qReal::CheckStatus> checkings;
	checkings.append(checkEnginesBackward_1(element, logicalApi));

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}
QList<qReal::CheckStatus> ConstraintsRobotsDiagram_2::checkEnginesForward(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi)
{
	QList<qReal::CheckStatus> checkings;
	checkings.append(checkEnginesForward_1(element, logicalApi));

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QList<qReal::CheckStatus> ConstraintsRobotsDiagram_2::check(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi, qReal::EditorManagerInterface const &editorManager)
{
	mEditorManager = &editorManager;

	if (element.element() == "EnginesForward") {
		return checkEnginesForward(element, logicalApi);
	}
	if (element.element() == "EnginesBackward") {
		return checkEnginesBackward(element, logicalApi);
	}

	return qReal::CheckStatus::defaultCheckStatusAsList();
}

QString ConstraintsRobotsDiagram_2::languageName() const
{
	return "RobotsDiagram";
}

QList<QString> ConstraintsRobotsDiagram_2::elementsNames() const
{
	QList<QString> elementsList;
	elementsList.append("EnginesForward");
	elementsList.append("EnginesBackward");

	return elementsList;
}
