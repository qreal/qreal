/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowDockInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/projectManagementInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/systemEvents.h>

#include "qrgui/systemFacade/systemFacadeDeclSpec.h"

namespace qReal {

class QRGUI_SYSTEM_FACADE_EXPORT NullMainWindow : public QObject
		, public gui::MainWindowInterpretersInterface
		, public gui::MainWindowDockInterface
{
	Q_OBJECT

public:
	NullMainWindow(ErrorReporterInterface &errorReporter, SystemEvents &events);
	NullMainWindow(ErrorReporterInterface &errorReporter, SystemEvents &events
			, const ProjectManagementInterface &projectManager, const GraphicalModelAssistInterface &graphicalModel);
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
	QStatusBar *statusBar() const override;
	QList<QToolBar *> toolBars() const override;

	void tabifyDockWidget(QDockWidget *first, QDockWidget *second) override;
	void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockWidget) override;

	QByteArray saveState(int version = 0) const override;
	bool restoreState(const QByteArray &state, int version = 0) override;

	void setCorner(Qt::Corner corner, Qt::DockWidgetArea area) override;

	/// Sends close event via SystemEvents.
	void emulateClose(int returnCode = 0);

private:
	void openFirstDiagram();
	void openTabWithEditor(const Id &id);

	ErrorReporterInterface &mErrorReporter;
	SystemEvents &mEvents;
	const GraphicalModelAssistInterface *mGraphicalModel;
	Id mActiveId;
	QWidget *mWindowWidget;  // Takes ownership
	QDockWidget *mLogicalModelDock;  // Takes ownership
	QDockWidget *mGraphicalModelDock;  // Takes ownership
	QDockWidget *mPropertyEditorDock;  // Takes ownership
	QDockWidget *mErrorReporterDock;  // Takes ownership
	QDockWidget *mPaletteDock;  // Takes ownership
	QStatusBar *mStatusBar;  // Takes ownership
	bool mClosed = false;
};

}
