#include "nullMainWindow.h"

using namespace qReal;

NullMainWindow::NullMainWindow(ErrorReporterInterface &errorReporter
		, SystemEvents &events)
	: mErrorReporter(errorReporter)
	, mEvents(events)
	, mGraphicalModel(nullptr)
	, mLogicalModelDock(new QDockWidget)
	, mGraphicalModelDock(new QDockWidget)
	, mPropertyEditorDock(new QDockWidget)
	, mErrorReporterDock(new QDockWidget)
	, mPaletteDock(new QDockWidget)
{
}

NullMainWindow::NullMainWindow(ErrorReporterInterface &errorReporter
		, SystemEvents &events
		, ProjectManagementInterface const &projectManager
		, GraphicalModelAssistInterface const &graphicalModel)
	: mErrorReporter(errorReporter)
	, mEvents(events)
	, mGraphicalModel(&graphicalModel)
	, mLogicalModelDock(new QDockWidget)
	, mGraphicalModelDock(new QDockWidget)
	, mPropertyEditorDock(new QDockWidget)
	, mErrorReporterDock(new QDockWidget)
	, mPaletteDock(new QDockWidget)
{
	connect(&projectManager, &ProjectManagementInterface::afterOpen, this, &NullMainWindow::openFirstDiagram);
}

NullMainWindow::~NullMainWindow()
{
	delete mLogicalModelDock;
	delete mGraphicalModelDock;
	delete mPropertyEditorDock;
	delete mErrorReporterDock;
	delete mPaletteDock;
}


void NullMainWindow::selectItem(const Id &graphicalId)
{
	Q_UNUSED(graphicalId)
}

void NullMainWindow::selectItemOrDiagram(const Id &graphicalId)
{
	Q_UNUSED(graphicalId)
}

void NullMainWindow::highlight(const Id &graphicalId, bool exclusive, const QColor &color)
{
	Q_UNUSED(graphicalId)
	Q_UNUSED(exclusive)
	Q_UNUSED(color)
}

void NullMainWindow::dehighlight(const Id &graphicalId)
{
	Q_UNUSED(graphicalId)
}

void NullMainWindow::dehighlight()
{
}

ErrorReporterInterface *NullMainWindow::errorReporter()
{
	return &mErrorReporter;
}

Id NullMainWindow::activeDiagram() const
{
	return mActiveId;
}

void NullMainWindow::openSettingsDialog(const QString &tab)
{
	Q_UNUSED(tab)
}

void NullMainWindow::reinitModels()
{
}

QWidget *NullMainWindow::windowWidget()
{
	return nullptr;
}

bool NullMainWindow::unloadPlugin(const QString &pluginName)
{
	Q_UNUSED(pluginName)
	return true;
}

bool NullMainWindow::loadPlugin(const QString &fileName, const QString &pluginName)
{
	Q_UNUSED(fileName)
	Q_UNUSED(pluginName)
	return true;
}

bool NullMainWindow::pluginLoaded(const QString &pluginName)
{
	Q_UNUSED(pluginName)
	return true;
}

void NullMainWindow::saveDiagramAsAPictureToFile(const QString &fileName)
{
	Q_UNUSED(fileName)
}

void NullMainWindow::arrangeElementsByDotRunner(const QString &algorithm, const QString &absolutePathToDotFiles)
{
	Q_UNUSED(algorithm)
	Q_UNUSED(absolutePathToDotFiles)
}

IdList NullMainWindow::selectedElementsOnActiveDiagram()
{
	return IdList();
}

void NullMainWindow::activateItemOrDiagram(const Id &id, bool setSelected)
{
	Q_UNUSED(id)
	Q_UNUSED(setSelected)
}

void NullMainWindow::updateActiveDiagram()
{
}

void NullMainWindow::deleteElementFromDiagram(const Id &id)
{
	Q_UNUSED(id)
}

void NullMainWindow::reportOperation(invocation::LongOperation *operation)
{
	Q_UNUSED(operation)
}

QWidget *NullMainWindow::currentTab()
{
	return nullptr;
}

void NullMainWindow::openTab(QWidget *tab, const QString &title)
{
	Q_UNUSED(tab)
	Q_UNUSED(title)
}

void NullMainWindow::closeTab(QWidget *tab)
{
	Q_UNUSED(tab)
}

void NullMainWindow::setTabText(QWidget *tab, const QString &text)
{
	Q_UNUSED(tab)
	Q_UNUSED(text)
}

void NullMainWindow::beginPaletteModification()
{
}

void NullMainWindow::setElementInPaletteVisible(const Id &metatype, bool visible)
{
	Q_UNUSED(metatype)
	Q_UNUSED(visible)
}

void NullMainWindow::setVisibleForAllElementsInPalette(bool visible)
{
	Q_UNUSED(visible)
}

void NullMainWindow::setElementInPaletteEnabled(const Id &metatype, bool enabled)
{
	Q_UNUSED(metatype)
	Q_UNUSED(enabled)
}

void NullMainWindow::setEnabledForAllElementsInPalette(bool enabled)
{
	Q_UNUSED(enabled)
}

void NullMainWindow::endPaletteModification()
{
}

QMap<QString, gui::PreferencesPage *> NullMainWindow::preferencesPages() const
{
	return {};
}

void NullMainWindow::openFirstDiagram()
{
	if (!mGraphicalModel) {
		return;
	}

	Id const rootId = mGraphicalModel->rootId();
	IdList const rootIds = mGraphicalModel->children(rootId);
	if (rootIds.count() == 0) {
		return;
	}

	openTabWithEditor(rootIds[0]);
}

void NullMainWindow::openTabWithEditor(const Id &id)
{
	mActiveId = id;
	emit mEvents.activeTabChanged(id);
}

QDockWidget *NullMainWindow::logicalModelDock() const
{
	return mLogicalModelDock;
}

QDockWidget *NullMainWindow::graphicalModelDock() const
{
	return mGraphicalModelDock;
}

QDockWidget *NullMainWindow::propertyEditorDock() const
{
	return mPropertyEditorDock;
}

QDockWidget *NullMainWindow::errorReporterDock() const
{
	return mErrorReporterDock;
}

QDockWidget *NullMainWindow::paletteDock() const
{
	return mPaletteDock;
}

void NullMainWindow::tabifyDockWidget(QDockWidget *first, QDockWidget *second)
{
	Q_UNUSED(first)
	Q_UNUSED(second)
}

void NullMainWindow::addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockWidget)
{
	Q_UNUSED(area)
	Q_UNUSED(dockWidget)
}
