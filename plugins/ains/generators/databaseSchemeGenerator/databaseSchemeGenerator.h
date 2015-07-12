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

#include <QtCore/QString>
#include <QtCore/QHash>

#include <qrkernel/ids.h>
#include <qrutils/outFile.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrrepo/logicalRepoApi.h>

namespace qReal {

class DatabaseEditorSchemeGenerator
{
public:
	explicit DatabaseEditorSchemeGenerator(LogicalModelAssistInterface const &api
			, ErrorReporterInterface& errorReporter);

	ErrorReporterInterface& generateDatabaseScheme(Id const &modelId, QString const &pathToFile);
	QHash<Id, QPair<QString, QString> > modelList();

private:
	LogicalModelAssistInterface const &mApi;
	ErrorReporterInterface& mErrorReporter;
	void processingColumns(Id const &tableId, utils::OutFile &outFile);
	void addForeignKeys(Id const &tableId, utils::OutFile &outFile);
	bool isColumn(QString const &type);
	QString const columnType(Id const &id);
	bool isKeyWord(QString const &name);
	void fillKeyWords();

	QStringList mKeyWords;
};
}


