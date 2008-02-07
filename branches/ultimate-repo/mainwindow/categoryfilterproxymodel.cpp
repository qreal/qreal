#include <QtGui>

#include "categoryfilterproxymodel.h"

CategoryFilterProxyModel::CategoryFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool CategoryFilterProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
	if ( sourceParent == QModelIndex() )
		return QSortFilterProxyModel::filterAcceptsRow(sourceRow,sourceParent);
	else
		return true;
}
