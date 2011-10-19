#pragma once

#include <QtCore/QString>
#include <QHash>

#include "../../../../qrkernel/ids.h"
#include "../../../../qrutils/outFile.h"
#include "../../../../qrgui/mainwindow/errorReporter.h"
#include "../../../../qrrepo/logicalRepoApi.h"

namespace qReal {

class DatabaseEditorSchemeGenerator
{
public:
	explicit DatabaseEditorSchemeGenerator(LogicalModelAssistInterface const &api, ErrorReporterInterface& errorReporter);
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


