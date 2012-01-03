#include "hascolSupportPlugin.h"

#include <QtGui/QApplication>
#include <QtGui/QFileDialog>

#include "hascolGenerator.h"
#include "hascolParser.h"

Q_EXPORT_PLUGIN2(hascolSupportPlugin, hascol::HascolSupportPlugin)

using namespace qReal;
using namespace hascol;

HascolSupportPlugin::HascolSupportPlugin()
		: mGenerateCodeAction(NULL)
		, mParseSourcesAction(NULL)
{
	mAppTranslator.load(":/hascolSupport_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);
}

HascolSupportPlugin::~HascolSupportPlugin()
{
}

void HascolSupportPlugin::init(PluginConfigurator const &configurator)
{
	mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
	mLogicalRepoApi = &configurator.logicalModelApi().mutableLogicalRepoApi();
}

QList<ActionInfo> HascolSupportPlugin::actions()
{
	mGenerateCodeAction.setText(tr("Generate hascol code"));
	ActionInfo generateCodeActionInfo(&mGenerateCodeAction, "generators", "tools");
	connect(&mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateHascolSourceCode()));

	mParseSourcesAction.setText(tr("Parse hascol sources"));
	ActionInfo parseSourcesActionInfo(&mParseSourcesAction, "generators", "tools");
	connect(&mParseSourcesAction, SIGNAL(triggered()), this, SLOT(parseHascolSources()));

	return QList<ActionInfo>() << generateCodeActionInfo << parseSourcesActionInfo;
}

void HascolSupportPlugin::generateHascolSourceCode()
{
	HascolGenerator hascolGenerator(*mLogicalRepoApi, *mMainWindowInterface->errorReporter());
	hascolGenerator.generate();
}

void HascolSupportPlugin::parseHascolSources()
{
	QStringList const fileNames = QFileDialog::getOpenFileNames(NULL, tr("Select Hascol files to parse"), ".", "*.md;;*.*");
	if (fileNames.empty())
		return;

	HascolParser parser(*mLogicalRepoApi, *mMainWindowInterface->errorReporter());
	parser.parse(fileNames);

	mMainWindowInterface->reinitModels();
}
