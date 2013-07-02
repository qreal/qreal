#include "rulesPlugin.h"

using namespace qReal;
using namespace qReal::rulesChecker;


RulesPlugin::RulesPlugin()
	: mChecker(NULL)
	, mRunAction(NULL)
{

}

RulesPlugin::~RulesPlugin()
{
	delete mChecker;
}

void RulesPlugin::init(const PluginConfigurator &configurator)
{
	mChecker = new RulesChecker(configurator.graphicalModelApi(), configurator.mainWindowInterpretersInterface());

	initAction();
}

QList<ActionInfo> RulesPlugin::actions()
{
	return mActionInfos;
}


void RulesPlugin::initAction()
{
	mRunAction = new QAction(QIcon(":/icons/startChecker.png"), QObject::tr("Run"), NULL);
	mRunAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
	ActionInfo runActionInfo(mRunAction, "generators", "tools");
	QObject::connect(mRunAction, SIGNAL(triggered()), mChecker, SLOT(check()));

	mActionInfos << runActionInfo;

}
