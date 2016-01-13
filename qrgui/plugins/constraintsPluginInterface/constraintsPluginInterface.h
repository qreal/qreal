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

#pragma once

#include "plugins/pluginManager/editorManagerInterface.h"
#include "qrrepo/logicalRepoApi.h"
#include "qrrepo/graphicalRepoApi.h"
#include "checkStatus.h"

namespace qReal {

const QString keywordForAllMetamodels = "AllMetamodels";//qwerty_copypast

class ConstraintsPluginInterface
{
public:
	virtual QList<CheckStatus> check(
			const Id &element
			, const qrRepo::LogicalRepoApi &logicalApi
			, const EditorManagerInterface &editorManager) = 0;

	virtual ~ConstraintsPluginInterface() {}

	virtual QString metamodelName() const = 0;
	virtual QString id() const = 0;

	bool isCorrectMetamodelName(const Id &element) const
	{
		return ((metamodelName() == element.editor() || metamodelName() == keywordForAllMetamodels));
	}
};

}

Q_DECLARE_INTERFACE(qReal::ConstraintsPluginInterface, "ru.qreal.QReal.ConstraintsPluginInterface/0.1")
