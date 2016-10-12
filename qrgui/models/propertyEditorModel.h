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
class QRGUI_MODELS_EXPORT PropertyEditorModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit PropertyEditorModel(const qReal::EditorManagerInterface &editorManagerInterface, QObject *parent = 0);

	QModelIndex parent(const QModelIndex &index) const;

	void setValueForIndex(const QModelIndex &index, QString value);
	QString getValueFromIndex(const QModelIndex &index);

	int rowCount(const QModelIndex &index) const;
	int columnCount(const QModelIndex &index) const;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

	int countOfChilds(const QModelIndex &index) const;

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

	/// Returns internal (not displayed) property name of the given index.
	QString propertyName(const QModelIndex &index) const;

	/// Overload for convenience.
	/// May not set actual property value if element different from \a id is selected. False will be returned then,
	/// if property was successully setted true will be returned.
	bool setData(const qReal::Id &id, const QString &propertyName, const QVariant &value);

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

	enum AttributeClassEnum {
		namePseudoattribute
		, logicalAttribute
		, graphicalAttribute
		, graphicalIdPseudoattribute
		, logicalIdPseudoattribute
		, metatypePseudoattribute
	};

	class Field {

	public:
		Field(QString fieldName_, AttributeClassEnum attributeClass_, int role_, Field *parent)
			: mFieldName(fieldName_), mAttributeClass(attributeClass_), mRole(role_)
		{
			mParentItem = parent;
		}

		Field(QString fieldName_, AttributeClassEnum attributeClass_, Field *parent)
			: mFieldName(fieldName_), mAttributeClass(attributeClass_), mRole(-1)
		{
			mParentItem = parent;
		}

		void appendChild(Field *item)
		{
			mChildItems.append(item);
		}

		Field* child(int row)
		{
			return mChildItems.value(row);
		}

		int childCount() const
		{
			return mChildItems.count();
		}

		Field* parentItem()
		{
			return mParentItem;
		}

		QList<Field*> getChilds(Field* parent)
		{
			QList<Field*> result;
			for (int i = 0; i < mChildItems.count(); ++i) {
				if ((mChildItems.at(i)->parentItem() == parent)) {
					result.append(mChildItems.at(i));
				}
			}

			return result;
		}

		int numberOfChilds(Field* parent)
		{
			int result = 0;
			for (int i = 0; i < mChildItems.count(); ++i) {
				if ((mChildItems.at(i)->parentItem() == parent)) {
					++result;
				}
			}

			return result;
		}

		int row() const
		{
			if (mParentItem)
				return mParentItem->mChildItems.indexOf(const_cast<Field*>(this));
			return 0;
		}

		AttributeClassEnum attributeClass()
		{
			return mAttributeClass;
		}

		QString fieldName()
		{
			return mFieldName;
		}

		int role()
		{
			return mRole;
		}

		void setValue(int row, QString value)
		{
			Field *child = mChildItems.at(row);
			child->mValue = value;
		}

		QString value()
		{
			return mValue;
		}

	private:
		QList<Field*> mChildItems;
		Field *mParentItem;

		QString mFieldName;
		AttributeClassEnum mAttributeClass;
		int mRole;
		QString mValue;

	};

private slots:
	void rereadData(const QModelIndex &, const QModelIndex &);

private:
	QAbstractItemModel *mTargetLogicalModel;
	QAbstractItemModel *mTargetGraphicalModel;
	QPersistentModelIndex mTargetLogicalObject;
	QPersistentModelIndex mTargetGraphicalObject;

	Field* mField;

	const qReal::EditorManagerInterface &mEditorManagerInterface;

	bool isValid() const;
};
