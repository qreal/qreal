#include "databasesGenerator.h"

#include <QtWidgets/QApplication>

using namespace qReal;
using namespace databasesSupport;

QString const databases = "Databases";

DatabasesGenerator::DatabasesGenerator(const PluginConfigurator configurator)
	: mErrorReporter(NULL)
{
	//mAppTranslator.load(":/DatabasesGenerator_" + QLocale::system().name());
	//QApplication::installTranslator(&mAppTranslator);

	mErrorReporter = configurator.mainWindowInterpretersInterface().errorReporter();
	//mParser = new BlockParser(mErrorReporter);

	/*mVisualDebugger = new VisualDebugger(configurator.logicalModelApi()
			, configurator.graphicalModelApi()
			, configurator.mainWindowInterpretersInterface()
			, mParser
	); */
}

DatabasesGenerator::~DatabasesGenerator()
{
}

void DatabasesGenerator::activeTabChanged(Id const &rootElementId)
{
	/*bool const enabled = rootElementId.diagram() == blockDiagram;
	foreach (ActionInfo const &actionInfo, mActionInfos) {
		if (actionInfo.isAction()) {
			actionInfo.action()->setEnabled(enabled);
		} else {
			actionInfo.menu()->setEnabled(enabled);
		}
	}*/
}


void DatabasesGenerator::generateSQL()
{
	mErrorReporter->clear();
	/*
	//mVisualDebugger->setCurrentDiagram();

	//mVisualDebugger->generateCode();

			if (!mDebuggerConnector->hasBuildError()) {
				mErrorReporter->addInformation(tr("Code generated and builded successfully"));
			} */
}






