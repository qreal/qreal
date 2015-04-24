/* Copyright 2007-2015 QReal Research Group
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

#include "interpreterCore/customizer.h"

#include <QtWidgets/QApplication>

#include <qrkernel/settingsManager.h>

using namespace interpreterCore;

QString Customizer::windowTitle() const
{
	return QObject::tr("TRIK Studio") + " " + productVersion();
}

QIcon Customizer::applicationIcon() const
{
	return QIcon(":/icons/icon.png");
}

QImage Customizer::applicationLogo() const
{
	return QImage(":/icons/splashscreen.png");
}

QString Customizer::productVersion() const
{
	/// @todo other storage for it?
	return "3.1.0-α1";
}

QString Customizer::aboutText() const
{
	return "<b>" + windowTitle() + "<b><br><br><a href=\"http://robots.qreal.ru/\">http://robots.qreal.ru/</a>";
}

QString Customizer::examplesDirectory() const
{
	return QApplication::applicationDirPath() + "/examples";
}

bool Customizer::showInterpeterButton() const
{
	return false;
}

bool Customizer::enableNewDiagramAction() const
{
	return false;
}

void Customizer::customizeDocks(qReal::gui::MainWindowDockInterface *dockInterface)
{
	mDockInterface = dockInterface;
	dockInterface->logicalModelDock()->hide();
	dockInterface->tabifyDockWidget(dockInterface->graphicalModelDock(), dockInterface->propertyEditorDock());
	dockInterface->graphicalModelDock()->setWindowTitle(QObject::tr("Blocks"));
}

void Customizer::placeDevicesConfig(QWidget *devicesWidget)
{
	QDockWidget *devicesDock = produceDockWidget(QObject::tr("Configure devices"), devicesWidget);
	mDockInterface->addDockWidget(Qt::LeftDockWidgetArea, devicesDock);
}

void Customizer::placeWatchPlugins(QDockWidget *watchWindow, QWidget *graphicsWatch)
{
	mDockInterface->addDockWidget(Qt::LeftDockWidgetArea, watchWindow);
	watchWindow->setFloating(false);

	QDockWidget * const graphWatchDock = produceDockWidget(QObject::tr("Sensors state"), graphicsWatch);
	mDockInterface->addDockWidget(Qt::LeftDockWidgetArea, graphWatchDock);

	mDockInterface->tabifyDockWidget(watchWindow, graphWatchDock);
}

QDockWidget *Customizer::produceDockWidget(const QString &title, QWidget *content) const
{
	QDockWidget *dock = new QDockWidget(title);
	dock->setWidget(content);
	return dock;
}

QString Customizer::userPaletteTitle() const
{
	return QObject::tr("Subprograms");
}

QString Customizer::userPaletteDescription() const
{
	return QObject::tr("The list of all declared subprograms in the project");
}
