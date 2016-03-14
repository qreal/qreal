/* Copyright 2014-2016 Anastasia Semenova
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

#include "databasesCustomizer.h"

namespace qReal {
namespace databasesSupport {

DatabasesCustomizer::~DatabasesCustomizer()
{
}

QString DatabasesCustomizer::windowTitle() const
{
	return QObject::tr("QReal:Databases") + " " + productVersion();
}

QIcon DatabasesCustomizer::applicationIcon() const
{
	return QIcon(":/icons/kroki.png");
}

QString DatabasesCustomizer::productVersion() const
{
	return "1.0";
}

QString DatabasesCustomizer::aboutText() const
{
	return "<b>" + windowTitle();
}

bool DatabasesCustomizer::showInterpeterButton() const
{
	return false;
}

void DatabasesCustomizer::customizeDocks(gui::MainWindowDockInterface *dockInterface)
{
	mDockInterface = dockInterface;
	mDockInterface->logicalModelDock()->hide();

	dockInterface->propertyEditorDock()->setMaximumWidth(260);
}

QDockWidget *DatabasesCustomizer::produceDockWidget(QString const &title, QWidget *content) const
{
	QDockWidget *dock = new QDockWidget(title);
	dock->setWidget(content);
	return dock;
}

QString DatabasesCustomizer::userPaletteTitle() const
{
	return QObject::tr("Databases");
}

QString DatabasesCustomizer::userPaletteDescription() const
{
	return QObject::tr("The list of all declared elements in the project");
}

}
}
