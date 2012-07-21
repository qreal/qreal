/** @file propertyeditorproxymodel.h
 *	@brief Модель редактора свойств
 * */
#pragma once

#include <QAbstractTableModel>
#include <QtCore/QStringList>

#include "../pluginManager/editorManager.h"
#include "../../qrrepo/logicalRepoApi.h"

/** @class PropertyEditorModel
 *	@brief Модель редактора свойств
 * */
class PropertyEditorModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	explicit PropertyEditorModel(qReal::EditorManager const &editorManager,
			QObject *parent = 0);

	int rowCount(const QModelIndex &index) const;
	int columnCount(const QModelIndex &index) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	void setSourceModels(QAbstractItemModel * const logicalSourceModel, QAbstractItemModel * const graphicalSourceModel);
	void setModelIndexes(QModelIndex const &logicalModelIndex, QModelIndex const &graphicalModelIndex);
	void clearModelIndexes();

	QStringList enumValues(const QModelIndex &index) const;

	// Methods needed by "Reference button" delegate
	QString typeName(const QModelIndex &index) const;

	// Methods for use in delegate, allow to determine where in actual models to put data
	QModelIndex modelIndex(int row) const;
	int roleByIndex(int row) const;
	bool isReference(QModelIndex const &index, QString const &propertyName);

	const QModelIndex& logicalModelIndex() const;
	const QModelIndex& graphicalModelIndex() const;

	QAbstractItemModel* logicalTargetModel() const;
	QAbstractItemModel* graphicalTargetModel() const;

	bool isCurrentIndex(QModelIndex const &index) const;

	qReal::Id idByIndex(QModelIndex const &index) const;

private slots:
	void rereadData(QModelIndex const &, QModelIndex const &);

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

	qReal::EditorManager const &mEditorManager;

	bool isValid() const;
};
