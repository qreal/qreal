#ifndef CATEGORYFILTERPROXYMODEL_H
#define CATEGORYFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class CategoryFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT

	public:
		CategoryFilterProxyModel(QObject *parent = 0);

	protected:
		bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
};

#endif
