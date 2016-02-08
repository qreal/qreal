/* Copyright 2014-2015 Anastasia Semenova
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

#include <QtCore/QTranslator>
#include <QtWidgets/QApplication>
#include <QProcess>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <plugins/toolPluginInterface/pluginConfigurator.h>
#include <qrkernel/ids.h>

#include "databasesPreferencesPage.h"

namespace qReal {
namespace databasesSupport {

/// Class for generation physical db model from logical db model
/// and generation SQL-code for dbmss from physical model
/// Designations: physical model - pm, logical model - lm
class DatabasesGenerator : public QObject
{
	Q_OBJECT
public:
	DatabasesGenerator(PluginConfigurator const configurator
			, DatabasesPreferencesPage const *preferencesPage);
	~DatabasesGenerator();

public slots:
	/// Checks simple correctness rules
	bool checkCorrectness();
	/// Generates physical model from lm
	void generatePhysicalModel();
	/// Generates SQL-code from pm
	void generateSQLCode();
	/// Changes current dbms-editor
	void changeEditor(QString const &dbmsName);

private:
	/// These methods generate SQL-code in addiction of dbms
	void generateWithSqlServer2008();
	void generateWithMySql5();
	void generateWithSqlite();
	void generateWithMicrosoftAccess();
	void generateWithPostgreSql();

	/// Creates element of diagram from string
	qReal::Id createElementFromString(QString const &elemName
			, QPointF coord = QPointF()
			, Id const &parentLogicalId = Id::rootId()
			, bool coordByParent = false); // true means, that coordinates will be taken from parent
	/// Creates elem of pm "Column" from elem of lm "Attribute"
	qReal::Id makeColumnFromAttribute(Id const &attributeId, Id const &parentId);
	/// Creates elem of pm "Table" from elem of lm "Entity"
	qReal::Id makeTableFromEntity(Id const &entityId, Id const &parentId);
	/// Creates elem of pm "Column" from set of entities
	/// Combines attributes of all entities
	qReal::Id makeTableFromEntitySet(IdList const &set, Id const &parentId);
	/// Creates elem of pm "PhysicalOneToManyRelationship"
	/// from elem of lm "OneToManyRelationship"
	qReal::Id copyOneToManyRelationship(Id const &logicalModelId
			, Id const &parentId
			, Id const &from
			, Id const &to);
	/// Creates two elems of pm "PhysicalManyToManyRelationship"
	/// from elem of lm "OneToManyRelationship"
	qReal::Id copyManyToManyRelationship(Id const &logicalModelId
			, Id const &parentId
			, Id const &from
			, Id const &to);

	/// Searches for alone entities (we make table for each alone entity)
	/// Formations of one-to-one sets
	/// (we will union entities bounded by one-to-one relationship in one table)
	QList<IdList> processEntities(Id const &logicalDiagramId);
	/// Sets identificators of one-to-many in match matrix
	/// Searches for errors
	/// Adds bounding attribute
	bool processOneToManyRelationships(QList<IdList> const &oneToOneBoundedEntitiesSets
			, IdList setTables
			, Id const &logicalDiagramId);
	/// Sets identificators of many-to-many in match matrix
	/// Searches for errors
	/// Adds bounding table
	bool processManyToManyRelationships(QList<IdList> const &oneToOneBoundedEntitiesSets
			, IdList setTables
			, Id const &logicalDiagramId);

	// Finds element of diagram with specified name
	IdList findNodes(QString const &name);
	QVariant getProperty(Id const &id, QString const &propertyName);
	Id getParent(Id const &id);
	IdList getChildren(Id const &id);
	IdList getBoundedWithOneToOneRealationship(Id const &id);
	void error(QString const &error, bool isCritical);
	Id getPrimaryKey(Id const &entityId);
	void clearPhysicalModel();

	Id getPrimaryKeyOfSet(IdList const &entitySet);
	QString getPrimaryKeyNameOfSet(IdList const &entitySet);
	int getParentNumber(Id const &childEntity, QList<IdList> const &set);
	QString getListTableName(IdList const &list);
	bool checkEntities();
	bool checkAttributes();
	bool checkRelationships();

	// do not make const & (some methods don`t work)
	qReal::gui::MainWindowInterpretersInterface &mInterpretersInterface;
	qReal::LogicalModelAssistInterface &mLogicalModelApi;
	qReal::GraphicalModelAssistInterface &mGraphicalModelApi;
	Id mCurrentDiagram;
	IdList mPassedElements;
	QFile codeFile;
	QString mDbms;
	int **mRelMatrix; // Relationship matrix
	DatabasesPreferencesPage const *mPreferencesPage;
	gui::MainWindowInterpretersInterface &mMainWindowInterface;
	qReal::ErrorReporterInterface *mErrorReporter;
};

}
}

