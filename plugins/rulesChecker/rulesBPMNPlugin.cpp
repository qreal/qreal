#include "rulesBPMNPlugin.h"

#include <QtWidgets/QApplication>

using namespace qReal;
using namespace qReal::rulesChecker;

RulesPlugin::RulesPlugin()
	: mChecker(NULL)
	, mRunAllDiagram(NULL)
	, mRunCurrentDiagram(NULL)
{
	mTranslator.load(":/robotsGenerator_" + QLocale::system().name());
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
}

qReal::Customizer * RulesPlugin::customizationInterface()
{
	return &mCustomizer;
}
