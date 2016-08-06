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

#include "qrmcLauncher.h"

#include <qrrepo/repoApi.h>

#include "qrmc/metaCompiler.h"
#include "defs.h"

using namespace editorPluginTestingFramework;

void QrmcLauncher::launchQrmc(const QString &fileName, const QString &targetDirectory)
{
	/// @todo: shall use qmake here
	qDebug() << "Launching QRMC...";
	QString normalizedFileName = fileName;
	if (!fileName.contains(".qrs")) {
		normalizedFileName += ".qrs";
	}

	qrRepo::RepoApi repoApi(normalizedFileName);
	qrmc::MetaCompiler metaCompiler(repoApi, targetDirectory);
	const qReal::IdList metamodels = repoApi.children(qReal::Id::rootId());

	for (const qReal::Id &key : metamodels) {
		const QString &objectType = key.element();
		if (objectType == "MetamodelDiagram" && repoApi.isLogicalElement(key)) {
			const QString &nameOfMetamodel = repoApi.stringProperty(key, "name");

			if (!metaCompiler.compile(nameOfMetamodel)) {
				qDebug() << "QRMC failed for whatever reason it is failed. Have a nice day.";
			}
		}
	}

	qDebug() << "QRMC successfully did whatever it did.";
	qDebug() << stringSeparator;
}
