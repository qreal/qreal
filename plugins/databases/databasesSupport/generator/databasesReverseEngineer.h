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

#pragma once

#include <QtWidgets/QMessageBox>
#include <QtSql>

#include "qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h"
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <plugins/toolPluginInterface/pluginConfigurator.h>

#include "databasesPreferencesPage.h"

namespace qReal {
namespace databasesSupport {

/// Class for reverse engineering physical schema from the file
class DatabasesReverseEngineer : public QObject
{
	Q_OBJECT
public:
	DatabasesReverseEngineer(PluginConfigurator const configurator
		, DatabasesPreferencesPage const *preferencesPage);

signals:

public slots:
	/// Generates physical schema from the file
	void generateSchema(const QString &dbms, QString const &filePath);
private:
	/// Creates element of diagram from string
	qReal::Id createElementFromString(QString const &elemName
			, QPointF coord = QPointF(0,0)
			, Id const &parentLogicalId = Id::rootId()
			, bool coordByParent = false);
	qReal::Id createTable(QString const &tableName
			, QPointF const &coord = QPointF(0,0)
			, Id const &parentLogicalId = Id::rootId());
	qReal::Id createColumn(QString const &columnName
			, QString const &columnType
			, Id const &parentId);

	QString mDbms;

	// do not make const & (some methods don`t work)
	qReal::gui::MainWindowInterpretersInterface &mInterpretersInterface;
	qReal::LogicalModelAssistInterface &mLogicalModelApi;
	qReal::GraphicalModelAssistInterface &mGraphicalModelApi;
	qReal::ErrorReporterInterface *mErrorReporter;
	DatabasesPreferencesPage const *mPreferencesPage;
};

}
}
