/* Copyright 2007-2017 QReal Research Group, Dmitry Mordvinov
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

#include "umlCheckerCustomizer.h"

#include <QtWidgets/QApplication>

#include <qrkernel/settingsManager.h>
#include <qrkernel/platformInfo.h>


using namespace qReal;

QString UmlCheckerCustomizer::windowTitle() const
{
	return QObject::tr("QReal-Uml") + " " + productVersion();
}

QIcon UmlCheckerCustomizer::applicationIcon() const
{
	return QIcon(":/icons/icon.png");
}

QImage UmlCheckerCustomizer::applicationLogo() const
{
	return QImage(":/icons/icon.png");
}

QString UmlCheckerCustomizer::productVersion() const
{
	return "0.0.2";
}

QString UmlCheckerCustomizer::aboutText() const
{
	return "<b>" + windowTitle() + "<b><br><br><a href=\"http://qreal.ru/\">http://qreal.ru/</a>";
}

QString UmlCheckerCustomizer::examplesDirectory() const
{
	return qReal::PlatformInfo::invariantSettingsPath("pathToExamples");
}

QList<QPair<QString, qReal::ActionVisibility> > UmlCheckerCustomizer::actionsVisibility() const
{
	return {
		{ "actionNew_Diagram", qReal::ActionVisibility::VisibleOnlyInMenu }
		, { "actionFullscreen", qReal::ActionVisibility::Invisible }
	};
}

bool UmlCheckerCustomizer::showInterpeterButton() const
{
	return false;
}

QString UmlCheckerCustomizer::userPaletteTitle() const
{
	return QObject::tr("Subprograms");
}

QString UmlCheckerCustomizer::userPaletteDescription() const
{
	return QObject::tr("The list of all declared subprograms in the project");
}
