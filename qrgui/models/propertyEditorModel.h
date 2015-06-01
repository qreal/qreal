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

#include <QtCore/QAbstractTableModel>
#include <QtCore/QStringList>

#include <qrrepo/logicalRepoApi.h>

#include "models/modelsDeclSpec.h"
#include "plugins/pluginManager/editorManagerInterface.h"

/// Proxy model for property editor, maps single element from main model
/// (logical or graphical) to a list model with element properties.
class QRGUI_MODELS_EXPORT PropertyEditorModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	explicit PropertyEditorModel(const qReal::EditorManagerInterface &editorManagerInterface, QObject *parent = 0);

	int rowCount(const QModelIndex &index) const;
	int columnCount(const QModelIndex &index) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	void setSourceModels(QAbstractItemModel * const logicalSourceModel
				, QAbstractItemModel * const graphicalSourceModel);

	void setModelIndexes(const QModelIndex &logicalModelIndex, const QModelIndex &graphicalModelIndex);
	void clearModelIndexes();

	bool enumEditable(const QModelIndex &index) const;
	QList<QPair<QString, QString>> enumValues(const QModelIndex &index) const;

	// Methods needed by "Reference button" delegate
	QString typeName(const QModelIndex &index) const;

	// Methods for use in delegate, allow to determine where in actual models to put data
	QModelIndex modelIndex(int row) const;
	int roleByIndex(int row) const;
	bool isReference(const QModelIndex &index, const QString &propertyName);

	const QModelIndex& logicalModelIndex() const;
	const QModelIndex& graphicalModelIndex() const;

	QAbstractItemModel* logicalTargetModel() const;
	QAbstractItemModel* graphicalTargetModel() const;

	bool isCurrentIndex(const QModelIndex &index) const;

	qReal::Id idByIndex(const QModelIndex &index) const;

private slots:
	void rereadData(const QModelIndex &, const QModelIndex &);

private:
	enum AttributeClassEnum {
		namePseudoattribute
		, logicalAttribute
		, graphicalAttribute
		, graphicalIdPseudoattribute
		, logicalIdPseudoattribute
		, metatypePseudoattribute
	};

	struct Field {
		QString fieldName;
		AttributeClassEnum attributeClass;
		int role;

		Field(QString fieldName_, AttributeClassEnum attributeClass_, int role_)
			: fieldName(fieldName_), attributeClass(attributeClass_), role(role_)
		{
		}

		Field(QString fieldName_, AttributeClassEnum attributeClass_)
			: fieldName(fieldName_), attributeClass(attributeClass_), role(-1)
		{
		}
	};

	QAbstractItemModel *mTargetLogicalModel;
	QAbstractItemModel *mTargetGraphicalModel;
	QPersistentModelIndex mTargetLogicalObject;
	QPersistentModelIndex mTargetGraphicalObject;

	QList<Field> mFields;

	const qReal::EditorManagerInterface &mEditorManagerInterface;

	bool isValid() const;
};
