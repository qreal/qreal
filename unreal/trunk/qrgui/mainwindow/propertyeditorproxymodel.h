/** @file propertyeditorproxymodel.h
 *	@brief Модель редактора свойств
 * */
#pragma once

#include <QAbstractTableModel>
#include <QtCore/QStringList>

#include "../editorManager/editorManager.h"

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
	int roleByIndex(int const index) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	void setSourceModel(QAbstractItemModel *sourceModel);
	QStringList getEnumValues(const QModelIndex &index) const;

	QModelIndex getModelIndex() const;

public slots:
	void setIndex(const QModelIndex &sourceIndex);

private slots:
	void rereadData();

private:
	QAbstractItemModel *targetModel;
	QPersistentModelIndex targetObject;

	QStringList mFieldNames;

	int mPseudoAttributesCount;

	int mEditablePseudoAttributesCount;

	qReal::EditorManager const &mEditorManager;
};
