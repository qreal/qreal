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

#include "qrmcLauncher.h"
#include "qrmc/metaCompiler.h"
#include "defs.h"

using namespace qReal;
using namespace qrmc;
using namespace qrRepo;
using namespace editorPluginTestingFramework;

void QrmcLauncher::launchQrmc(const QString &fileName, const QString &pathToQrmc, const QString &pathToGeneratedCode)
{
	qDebug() << "STARTING QRMC LAUNCHING";
	QString normalizedFileName = fileName;
	if (!fileName.contains(".qrs")) {
		normalizedFileName += ".qrs";
	}

	RepoApi * const mRepoApi = new RepoApi(normalizedFileName);
	MetaCompiler metaCompiler(pathToQrmc, mRepoApi);
	const IdList metamodels = mRepoApi->children(Id::rootId());

	for (Id const &key : metamodels) {
		const QString &objectType = key.element();
		if (objectType == "MetamodelDiagram" && mRepoApi->isLogicalElement(key)) {
			const QString &nameOfMetamodel = mRepoApi->stringProperty(key, "name");

			if (!metaCompiler.compile(nameOfMetamodel)) {
				qDebug() << "compilation failed";
			}
		}
	}

	qDebug() << stringSeparator;
}
