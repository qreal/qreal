/* Copyright 2007-2016 QReal Research Group, Yurii Litvinov
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

#include "metamodelXmlGeneratorLauncher.h"

#include <QtCore/QDir>

#include "plugins/metaEditor/metaEditorSupport/editorGenerator.h"
#include "qrrepo/repoApi.h"
#include "qrgui/mainWindow/errorReporter.h"

#include "defs.h"

using namespace metaEditor;
using namespace editorPluginTestingFramework;

void MetamodelXmlGeneratorLauncher::launchEditorGenerator(const QString &fileName, const QString &pathToQRealSources
		, const QString &pathToGeneratedCode)
{
	qDebug() << "Starting editor generator...";

	const qrRepo::RepoApi repoApi(fileName);
	qReal::gui::ErrorReporter reporter;
	EditorGenerator editorGenerator(repoApi, reporter);

	QDir dir(".");

	const QHash<qReal::Id, QPair<QString, QString>> metamodelList = editorGenerator.getMetamodelList();

	qDebug() << "Generating QRXC .pro file and .xml file with metamodel into" << pathToGeneratedCode;

	dir.mkpath(pathToGeneratedCode);
	for (const qReal::Id &key : metamodelList.keys()) {
		if (repoApi.isLogicalElement(key)) {
			editorGenerator.generateEditor(key, pathToGeneratedCode, pathToQRealSources);
		}
	}

	qDebug() << stringSeparator;
}
