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

#include <qrkernel/exception/exception.h>
#include <qrrepo/logicalRepoApi.h>

#include "models/modelsDeclSpec.h"
#include "plugins/pluginManager/editorManagerInterface.h"

/// Proxy model for property editor, maps single element from main model
/// (logical or graphical) to a list model with element properties.
class QRGUI_MODELS_EXPORT PropertyEditorModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit PropertyEditorModel(const qReal::EditorManagerInterface &editorManagerInterface, QObject *parent = nullptr);

	QModelIndex parent(const QModelIndex &index) const;

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
		Field(
				const QString &fieldName
				, AttributeClassEnum attributeClass
				, int role
				, Field *parent
				, QPersistentModelIndex targetLogicalObject
				, QPersistentModelIndex targetGraphicalObject
				)
			: mFieldName(fieldName)
			, mAttributeClass(attributeClass)
			, mRole(role)
			, mParentItem(parent)
			, mTargetLogicalObject(targetLogicalObject)
			, mTargetGraphicalObject(targetGraphicalObject)
		{
		}

		Field(const QString &name)
			: Field(name, logicalAttribute, -1, nullptr, QPersistentModelIndex(), QPersistentModelIndex())
		{
		}

		Field()
			: Field("root")
		{
		}

		~Field()
		{
			qDeleteAll(mChildItems);
		}

		void appendChild(Field *item)
		{
			mChildItems.append(item);
		}

		Field* child(int row) const
		{
			return mChildItems.value(row);
		}

		int childCount() const
		{
			return mChildItems.count();
		}

		Field *parentItem() const
		{
			return mParentItem;
		}

		QList<Field*> children(Field* parent) const
		{
			QList<Field*> result;
			for (int i = 0; i < mChildItems.count(); ++i) {
				if ((mChildItems.at(i)->parentItem() == parent)) {
					result.append(mChildItems.at(i));
				}
			}

			return result;
		}

		int numberOfChildren(Field* parent) const
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
			if (mParentItem) {
				return mParentItem->mChildItems.indexOf(const_cast<Field*>(this));
			}

			return 0;
		}

		AttributeClassEnum attributeClass() const
		{
			return mAttributeClass;
		}

		QString fieldName() const
		{
			return mFieldName;
		}

		int role() const
		{
			return mRole;
		}

		QString value() const
		{
			switch (mAttributeClass) {
			case namePseudoattribute:
			case logicalAttribute:
			case logicalIdPseudoattribute:
			case metatypePseudoattribute: {
				const QVariant data = mTargetLogicalObject.data(mRole);
				if (mRole == qReal::roles::idRole) {
					return data.value<qReal::Id>().id();
				} else {
					return data.toString();
				}
			}
			case graphicalIdPseudoattribute:
			case graphicalAttribute: {
				const QVariant data = mTargetGraphicalObject.data(mRole);
				if (mRole == qReal::roles::idRole) {
					return data.value<qReal::Id>().id();
				} else {
					return data.toString();
				}
			}
			}

			throw qReal::Exception("Unknown AttributeClass in PropertyEditorModel::Field::value");
		}

	private:
		const QString mFieldName;

		const AttributeClassEnum mAttributeClass;

		const int mRole;

		/// Has ownership over contained items.
		QList<Field*> mChildItems;

		/// Does not have ownership.
		Field * const mParentItem;

		const QPersistentModelIndex mTargetLogicalObject;
		const QPersistentModelIndex mTargetGraphicalObject;
	};

private slots:
	void rereadData(const QModelIndex &, const QModelIndex &);

private:
	/// Does not have ownership.
	QAbstractItemModel *mTargetLogicalModel;

	/// Does not have ownership.
	QAbstractItemModel *mTargetGraphicalModel;

	QPersistentModelIndex mTargetLogicalObject;
	QPersistentModelIndex mTargetGraphicalObject;

	QScopedPointer<Field> mField;

	const qReal::EditorManagerInterface &mEditorManagerInterface;

	bool isValid() const;

	QString fullPropertyName(const QModelIndex &index) const;
};
