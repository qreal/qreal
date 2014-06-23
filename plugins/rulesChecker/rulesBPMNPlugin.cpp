#include "rulesBPMNPlugin.h"

#include <QtWidgets/QApplication>

using namespace qReal;
using namespace qReal::rulesChecker;

RulesPlugin::RulesPlugin()
	: mChecker(NULL)
	, mRunAllDiagram(NULL)
	, mRunCurrentDiagram(NULL)
	, mExportToXml(NULL)
{
	mTranslator.load(":/rulesChecker_" + QLocale().name());
	QApplication::installTranslator(&mTranslator);

	initAction();
}

RulesPlugin::~RulesPlugin()
{
	delete mChecker;
}

void RulesPlugin::init(PluginConfigurator const &configurator)
{
	mChecker = new RulesChecker(configurator.graphicalModelApi().graphicalRepoApi(), configurator.mainWindowInterpretersInterface());
	QObject::connect(mRunAllDiagram, SIGNAL(triggered()), mChecker, SLOT(checkAllDiagrams()));
	QObject::connect(mRunCurrentDiagram, SIGNAL(triggered()), mChecker, SLOT(checkCurrentDiagram()));
	QObject::connect(mExportToXml, SIGNAL(triggered()), mChecker, SLOT(exportToXml()));
}

QList<ActionInfo> RulesPlugin::actions()
{
	return mActionInfos;
}

void RulesPlugin::initAction()
{
	mRunAllDiagram = new QAction(QObject::tr("Diagram Validation"), NULL);
	mRunAllDiagram->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
	ActionInfo runAllInfo(mRunAllDiagram, "generators", "tools");
	mActionInfos << runAllInfo;

	mRunCurrentDiagram = new QAction(QObject::tr("Validate active diagram"), NULL);
	ActionInfo runCurrentInfo(mRunCurrentDiagram, "generators", "tools");
	mActionInfos << runCurrentInfo;

	mExportToXml = new QAction(QObject::tr("Generate XML"), NULL);
	ActionInfo exportXmlInfo(mExportToXml, "generators", "tools");
	mActionInfos << exportXmlInfo;

}

qReal::Customizer * RulesPlugin::customizationInterface()
{
	return &mCustomizer;
}
