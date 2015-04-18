#include "diffPluginWrapper.h"

#include <QtWidgets/QTabWidget>

#include <plugins/tools/visualDiff/model/diffModel.h>
#include <plugins/tools/visualDiff/view/diffWindow.h>


using namespace versioning;
using namespace qReal;

DiffPluginWrapper::DiffPluginWrapper()
{
}

Customizer *DiffPluginWrapper::customizationInterface()
{
	return nullptr;
}

void DiffPluginWrapper::updateSettings()
{
}

QList<ActionInfo> DiffPluginWrapper::actions()
{
	return QList<qReal::ActionInfo>();
}

void DiffPluginWrapper::init(const PluginConfigurator &configurator)
{
	Q_UNUSED(configurator)
}

QPair<QString, gui::PreferencesPage *> DiffPluginWrapper::preferencesPage()
{
	///to do: make color options
	return QPair<QString, gui::PreferencesPage *>();
}

void DiffPluginWrapper::configure(
	ProjectManagementInterface *projectManager
	, ErrorReporterInterface *errorReporter
	, qrRepo::WorkingCopyManagementInterface *workingCopyManager
	, BriefVersioningInterface *vcs
	, QWidget *parent
	, EditorManagerInterface *manager
)
{
	mWorkingCopyManager = workingCopyManager;
	mVcs = vcs;
	mMainWindow = parent;
	mEditorManager = manager;
	mErrorReporter = errorReporter;
	mProjectManager = projectManager;
	mLoader = new details::ModelLoader(mVcs, mErrorReporter, mEditorManager, mWorkingCopyManager);
	connect(mLoader, SIGNAL(modelLoaded(DiffModel*)), this, SLOT(onModelLoaded(DiffModel*)));
}

void DiffPluginWrapper::showDiff(const QString &targetProject, QWidget *parentWidget, bool compactMode)
{
	mCompactMode = compactMode;
	mParentWidget = parentWidget;
	mLoader->startModelLoading(targetProject);
}

void DiffPluginWrapper::showDiff(
	const QString &repoRevision
	, const QString &targetProject
	, QWidget *parentWidget
	, bool compactMode
)
{
	mCompactMode = compactMode;
	mParentWidget = parentWidget;
	mLoader->startModelLoading(repoRevision, targetProject);
}

void DiffPluginWrapper::showDiff(
	const QString &oldRepoRevision
	, const QString &newRepoRevision
	, const QString &targetProject
	, QWidget *parentWidget
	, bool compactMode
)
{
	mCompactMode = compactMode;
	mParentWidget = parentWidget;
	mLoader->startModelLoading(oldRepoRevision, newRepoRevision, targetProject);
}

void DiffPluginWrapper::onModelLoaded(DiffModel *model)
{
	if (!model) {
		return;
	}

	int diagrams = model->newModel()->graphicalModelAssistApi().childrenOfRootDiagram();
	QTabWidget *diffWindowSet = new QTabWidget;
	for (int i = 0; i < diagrams; i++){
		DiffWindow *tmp = new DiffWindow(model, mCompactMode, i, mMainWindow);
		diffWindowSet->addTab(tmp, QString(tr("diagram ") + QString::number(i+1)));
	}

	mParentWidget->layout()->addWidget(diffWindowSet);
}

