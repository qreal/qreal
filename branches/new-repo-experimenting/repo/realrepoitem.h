#ifndef REALREPOITEM_H
#define REALREPOITEM_H

#include <QtCore/QHash>
#include <QtSql/QSqlQuery>


class RealRepoItem
{
public:
    enum NodeType { Root = 0, Category = 1, Element = 2,
	Diagram = 10, Class = 11, Package = 12, Link = 13
    };

    RealRepoItem(NodeType type, int row, int id, RealRepoItem *parent = 0);
    ~RealRepoItem();

    RealRepoItem *child(int i);
    RealRepoItem *parent()
	    { return parentItem; };
    
    int row() const
	    { return rowNumber; };
    int rowCount() const;
    
    QString name() const
	    { return m_name; };
    int id() const
	    { return m_id; };
    NodeType type() const
	    { return m_type; };

    void setName(QString name);

    void updateData();
    void childrenCleanup();

private:
    QSqlQuery childQuery;
    QHash<int,RealRepoItem*> childItems;
    RealRepoItem *parentItem;

    int rowNumber;
    int childCount;

    QString m_name;
    int m_id;
    NodeType m_type;

    QString childTableName;

};

// FIXME: only one open DB
extern QHash<int,QList<RealRepoItem *> > listByUuid;
extern QHash<int,QList<QPersistentModelIndex> > indexByUuid;

#endif
