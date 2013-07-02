#include "rulesChecker.h"

using namespace qReal::rulesChecker;

RulesChecker::RulesChecker(GraphicalModelAssistInterface &graphicalModelApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface)
	: mGraphicalModelApi(&graphicalModelApi)
	, mWindowInterface(&interpretersInterface)
{
}

void RulesChecker::makeDetour()
{

}


void RulesChecker::check()
{
	mWindowInterface->errorReporter()->addInformation("Probe");
	getElementsList();
}


void RulesChecker::getElementsList()
{
	IdList const metamodels = mGraphicalModelApi->children(Id::rootId());
	foreach (Id const key, metamodels) {
		qDebug() << "element" << key.element();
		qDebug() << "diagram" << key.diagram();
		qDebug() << "id" << key.id();
		qDebug() << "rootID" << key.rootId();
		qDebug() << "type" << key.type();
		qDebug() << "same type" << key.sameTypeId();
		qDebug() << "   _   ";
	}
}
