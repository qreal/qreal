#include "rulesBPMNPlugin.h"

using namespace qReal;
using namespace qReal::rulesChecker;

RulesPlugin::RulesPlugin()
	: mChecker(NULL)
	, mRunAction(NULL)
{
	initAction();
}

RulesPlugin::~RulesPlugin()
{
	delete mChecker;
}

void RulesPlugin::init(const PluginConfigurator &configurator)
{
	mChecker = new RulesChecker(configurator.graphicalModelApi().graphicalRepoApi(), configurator.mainWindowInterpretersInterface());
	QObject::connect(mRunAction, SIGNAL(triggered()), mChecker, SLOT(check()));
}

QList<ActionInfo> RulesPlugin::actions()
{
	return mActionInfos;
}

void RulesPlugin::initAction()
{
	mRunAction = new QAction(QObject::tr("Diagram Validation"), NULL);
	mRunAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
	ActionInfo runActionInfo(mRunAction, "generators", "tools");
	mActionInfos << runActionInfo;
}
