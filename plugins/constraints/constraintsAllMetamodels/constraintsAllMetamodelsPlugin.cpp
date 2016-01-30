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

#include "constraintsAllMetamodelsPlugin.h"
#include <QtCore/QtPlugin>
#include <QtCore/QVariant>

using namespace constraints;

ConstraintsPlugin::ConstraintsPlugin()
{
}

ConstraintsPlugin::~ConstraintsPlugin()
{
}

QList<qReal::CheckStatus> ConstraintsPlugin::check(
		qReal::Id const &element
		, qrRepo::LogicalRepoApi const &logicalApi
		, qReal::EditorManagerInterface const &editorManager)
{
	QList<qReal::CheckStatus> checkings;
	QString const elementName = element.element();
	QString const languageName = element.diagram();
	if (((mConstraintsAllLanguages.languageName() == languageName)
			|| (mConstraintsAllLanguages.languageName() == "AllLanguages"))
		&& (mConstraintsAllLanguages.elementsNames().contains("AllEdges")
			|| mConstraintsAllLanguages.elementsNames().contains("AllNodes")
			|| mConstraintsAllLanguages.elementsNames().contains(elementName))) {
		checkings.append(mConstraintsAllLanguages.check(element, logicalApi, editorManager));
	}

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QString ConstraintsPlugin::metamodelName() const
{
	return "AllMetamodels";
}

QString ConstraintsPlugin::id() const
{
	return "AllMetamodelsConstraints";
}
