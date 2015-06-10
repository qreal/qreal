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

#include "nullMainWindow.h"

#include <qrkernel/settingsManager.h>

#include <QtCore/QCoreApplication>
#include <QtWidgets/QStatusBar>

using namespace qReal;

NullMainWindow::NullMainWindow(ErrorReporterInterface &errorReporter
		, SystemEvents &events)
	: mErrorReporter(errorReporter)
	, mEvents(events)
	, mGraphicalModel(nullptr)
	, mWindowWidget(new QWidget)
	, mLogicalModelDock(new QDockWidget(mWindowWidget))
	, mGraphicalModelDock(new QDockWidget(mWindowWidget))
	, mPropertyEditorDock(new QDockWidget(mWindowWidget))
	, mErrorReporterDock(new QDockWidget(mWindowWidget))
	, mPaletteDock(new QDockWidget(mWindowWidget))
	, mStatusBar(new QStatusBar(mWindowWidget))
{
}

NullMainWindow::NullMainWindow(ErrorReporterInterface &errorReporter
		, SystemEvents &events
		, const ProjectManagementInterface &projectManager
		, const GraphicalModelAssistInterface &graphicalModel)
	: mErrorReporter(errorReporter)
	, mEvents(events)
	, mGraphicalModel(&graphicalModel)
	, mWindowWidget(new QWidget)
	, mLogicalModelDock(new QDockWidget(mWindowWidget))
	, mGraphicalModelDock(new QDockWidget(mWindowWidget))
	, mPropertyEditorDock(new QDockWidget(mWindowWidget))
	, mErrorReporterDock(new QDockWidget(mWindowWidget))
	, mPaletteDock(new QDockWidget(mWindowWidget))
	, mStatusBar(new QStatusBar(mWindowWidget))
{
	connect(&projectManager, &ProjectManagementInterface::afterOpen, this, &NullMainWindow::openFirstDiagram);
}

NullMainWindow::~NullMainWindow()
{
	delete mWindowWidget;
	SettingsManager::instance()->saveData();
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
	return mWindowWidget;
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

	const Id rootId = mGraphicalModel->rootId();
	const IdList rootIds = mGraphicalModel->children(rootId);
	if (rootIds.count() == 0) {
		return;
	}

	openTabWithEditor(rootIds[0]);
}

void NullMainWindow::openTabWithEditor(const Id &id)
{
	mActiveId = id;
	emit mEvents.activeTabChanged(TabInfo(id, nullptr));
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

QStatusBar *NullMainWindow::statusBar() const
{
	return mStatusBar;
}

QList<QToolBar *> NullMainWindow::toolBars() const
{
	return {};
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

QByteArray NullMainWindow::saveState(int version) const
{
	Q_UNUSED(version)
	return QByteArray();
}

bool NullMainWindow::restoreState(const QByteArray &state, int version)
{
	Q_UNUSED(state)
	Q_UNUSED(version)
	return true;
}

void NullMainWindow::setCorner(Qt::Corner corner, Qt::DockWidgetArea area)
{
	Q_UNUSED(corner)
	Q_UNUSED(area)
}

void NullMainWindow::emulateClose(int returnCode)
{
	if (mClosed) {
		return;
	}

	mClosed = true;
	emit mEvents.closedMainWindow();
	QCoreApplication::exit(returnCode);
}
