#pragma once

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowDockInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/projectManagementInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/systemEvents.h>

namespace qReal {

class NullMainWindow : public QObject
		, public gui::MainWindowInterpretersInterface
		, public gui::MainWindowDockInterface
{
	Q_OBJECT

public:
	NullMainWindow(ErrorReporterInterface &errorReporter, SystemEvents &events);
	NullMainWindow(ErrorReporterInterface &errorReporter, SystemEvents &events
			, ProjectManagementInterface const &projectManager, GraphicalModelAssistInterface const &graphicalModel);
	~NullMainWindow();

	void selectItem(const Id &graphicalId) override;
	void selectItemOrDiagram(const Id &graphicalId) override;
	void highlight(const Id &graphicalId, bool exclusive = true, const QColor &color = Qt::red) override;
	void dehighlight(const Id &graphicalId) override;
	void dehighlight() override;
	ErrorReporterInterface *errorReporter() override;

	Id activeDiagram() const override;

	void openSettingsDialog(const QString &tab) override;

	void reinitModels() override;

	QWidget *windowWidget() override;

	bool unloadPlugin(const QString &pluginName) override;

	bool loadPlugin(const QString &fileName, const QString &pluginName) override;

	bool pluginLoaded(const QString &pluginName) override;

	void saveDiagramAsAPictureToFile(const QString &fileName) override;

	void arrangeElementsByDotRunner(const QString &algorithm, const QString &absolutePathToDotFiles) override;

	IdList selectedElementsOnActiveDiagram() override;

	void activateItemOrDiagram(const Id &id, bool setSelected = true) override;

	void updateActiveDiagram() override;

	void deleteElementFromDiagram(const Id &id) override;

	void reportOperation(invocation::LongOperation *operation) override;

	QWidget *currentTab() override;
	void openTab(QWidget *tab, const QString &title) override;
	void closeTab(QWidget *tab) override;
	void setTabText(QWidget *tab, const QString &text) override;

	void beginPaletteModification() override;

	void setElementInPaletteVisible(const Id &metatype, bool visible) override;

	void setVisibleForAllElementsInPalette(bool visible) override;

	void setElementInPaletteEnabled(const Id &metatype, bool enabled) override;

	void setEnabledForAllElementsInPalette(bool enabled) override;

	void endPaletteModification() override;

	QMap<QString, gui::PreferencesPage *> preferencesPages() const override;

	QDockWidget *logicalModelDock() const override;
	QDockWidget *graphicalModelDock() const override;
	QDockWidget *propertyEditorDock() const override;
	QDockWidget *errorReporterDock() const override;
	QDockWidget *paletteDock() const override;

	void tabifyDockWidget(QDockWidget *first, QDockWidget *second) override;
	void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockWidget) override;

private:
	void openFirstDiagram();
	void openTabWithEditor(const Id &id);

	ErrorReporterInterface &mErrorReporter;
	SystemEvents &mEvents;
	GraphicalModelAssistInterface const *mGraphicalModel;
	Id mActiveId;
	QDockWidget *mLogicalModelDock;  // Takes ownership
	QDockWidget *mGraphicalModelDock;  // Takes ownership
	QDockWidget *mPropertyEditorDock;  // Takes ownership
	QDockWidget *mErrorReporterDock;  // Takes ownership
	QDockWidget *mPaletteDock;  // Takes ownership
};

}
