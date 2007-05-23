#ifndef PROPERTYEDITORPROXYMODEL_H
#define PROPERTYEDITORPROXYMODEL_H

#include <QAbstractTableModel>

#include "realrepoinfo.h"

class PropertyEditorModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    PropertyEditorModel(QObject *parent = 0);

	int rowCount(const QModelIndex &index) const;
	int columnCount(const QModelIndex &index) const;

	Qt::ItemFlags flags (const QModelIndex &index) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	void setSourceModel(QAbstractItemModel *sourceModel);

public slots:
	void setIndex(const QModelIndex &sourceIndex);

private:
	QAbstractItemModel *targetModel;
	QPersistentModelIndex targetObject;
	RealRepoInfo info;

	int type;
	QStringList roleNames;
};

#endif
