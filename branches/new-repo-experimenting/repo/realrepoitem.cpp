#include <QtSql>

#include "realrepoitem.h"
#include "realreporoles.h"

#define UNCONST const_cast<RealRepoItem *>(this)->

QString getColumnName(int elementNum, int roleNum);

static QVariant getQuery(QString query)
{
	qDebug() << query;
	QSqlQuery q1(query);
	if (!q1.next()) {
		return QVariant();
	}
	return q1.value(0);
}

QHash<int,QList<RealRepoItem *> > listByUuid;
QHash<int,QList<QPersistentModelIndex> > indexByUuid;

RealRepoItem::RealRepoItem(NodeType type, int row, int id, RealRepoItem *parent)
	: parentItem(parent), rowNumber(row), childCount(-1),
		m_id(id), m_type(type)
{
	switch (type) {
		case Root:	childTableName = "el_0";			break;
		case Category:	childTableName = QString("el_%1").arg(id);	break;
		default:	childTableName = QString("cont_%1").arg(id);	break;
	}

	updateData();

	if ( m_id )
		listByUuid[m_id].append(this);
}

RealRepoItem::RealRepoItem(int row, RealRepoItem *parent)
	: parentItem(parent), rowNumber(row), childCount(-1),
		m_id(0), m_type(Undefined)
{
}

void RealRepoItem::setId(int id)
{
	if ( !m_id ) {
		m_id = id;

		if ( m_id && m_type )
			getQuery(QString("INSERT INTO %1 (id,type) VALUES (%2,%3);")
				.arg(parentItem->childTableName).arg(m_id).arg(m_type));

		listByUuid[m_id].append(this);
	}
}

void RealRepoItem::setType(NodeType type)
{
	if ( !m_type ) {
		m_type = type;

		switch (type) {
			case Root:      childTableName = "el_0";                        break;
			case Category:  childTableName = QString("el_%1").arg(m_id);      break;
			default:        childTableName = QString("cont_%1").arg(m_id);    break;
		}
		
		if ( m_id && m_type )
			getQuery(QString("INSERT INTO %1 (id,type) VALUES (%2,%3);")
				.arg(parentItem->childTableName).arg(m_id).arg(m_type));

		updateData();

	}
}

RealRepoItem::~RealRepoItem()
{
	if ( m_id )
		listByUuid[m_id].removeAll(this);

	qDeleteAll(childItems);

	qDebug() << "deleted" << m_name;
}

void RealRepoItem::updateData()
{
	if ( m_id )
		m_name = getQuery(QString("SELECT name FROM el_%1 WHERE id=%2;")
				.arg(m_type > Element ? m_type : 0).arg(m_id)).toString();
}

void RealRepoItem::setName(QString name)
{
	if ( m_id )
		getQuery(QString("UPDATE el_%1 SET name='%3' WHERE id=%2;")
				.arg(m_type).arg(m_id).arg(name.replace("'","''")));
}

QVariant RealRepoItem::property(int key)
{
	if ( key == Unreal::PositionRole ) {
		QString query = QString("SELECT x, y FROM %1 WHERE id=%2")
			.arg(parent()->childTableName).arg(m_id);
		qDebug() << query;
		QSqlQuery q1(query);
		if (!q1.next()) {
			return QVariant();
		}
		return QPointF(q1.value(0).toInt(),q1.value(1).toInt());
	}

	qDebug() << "getting property" << key;
	if ( key > Unreal::UserRole ) {
		return getQuery(QString("SELECT `%1` FROM el_%2 WHERE id=%3;")
				.arg(getColumnName(m_type,key)).arg(m_type).arg(m_id));
	} else {
		return QVariant();
	}
}

bool RealRepoItem::setProperty(int key, const QVariant &data)
{
	if ( key == Unreal::PositionRole ) {
		getQuery(QString("UPDATE `%1` set x=%3, y=%4 WHERE id=%2")
				.arg(parent()->childTableName).arg(m_id)
				.arg(data.toPointF().x()).arg(data.toPointF().y()));
		return true;
	}

	qDebug() << "setting property" << key << "to" << data;
	if ( key > Unreal::UserRole ) {
		getQuery(QString("UPDATE `el_%1` SET `%3`='%4' WHERE id=%2;")
				.arg(m_type).arg(m_id)
				.arg(getColumnName(m_type,key)).arg(data.toString()));
	} else {
		getQuery(QString("UPDATE `%1` SET `%3`='%4' WHERE id=%2;")
				.arg(parent()->childTableName).arg(m_id)
				.arg(getColumnName(m_type,key)).arg(data.toString()));
	}
	return false;
}

RealRepoItem *RealRepoItem::child(int row)
{
	if (childItems.contains(row))
		return childItems[row];

	RealRepoItem *child;
	QString query;

	if ( m_type < Element ) {
		query = QString("SELECT id FROM %1;").arg(childTableName);
	} else {
		query = QString("SELECT id, type FROM %1;").arg(childTableName);
	}

	if ( ( childCount == -1 ) || ( ! childQuery.isActive() ) ) {
		qDebug() << query;
		childQuery.exec(query);
	}

	if ( !childQuery.seek(row) ) {
		qDebug() << "INTERNAL DB INCONSISTENCY";
		return 0;
	}

	int childId = childQuery.value(0).toInt();
	NodeType childCategory;
	if ( m_type == Root )
		childCategory = Category;
	else if ( m_type == Category )
		childCategory = static_cast<NodeType>(m_id);
	else
		childCategory = static_cast<NodeType>(childQuery.value(1).toInt());

	child = new RealRepoItem(childCategory, row, childId, this);
	childItems[row] = child;
	return child;
}

bool RealRepoItem::insertChild(int row, int count)
{
	for ( int i = 0; i < count; i++ ) {
		RealRepoItem *child = new RealRepoItem(row+i,this);
		childItems[row+i] = child;
	}
	if ( childCount != -1 ) {
		childCount += count;
	}
	return false;
}

bool RealRepoItem::createChild(int /*row*/, int type)
{
	int id = getQuery(QString("SELECT get_id();")).toInt();
	
	QString tableName = QString("cont_%1").arg(id);
	
	childCount = -1;

	// Create record in parent table;
	//getQuery(QString("INSERT INTO %1 (id,type) VALUES (%2,%3);")
	//		.arg(childTableName).arg(id).arg(type));

	// Create record in type table;
	getQuery(QString("INSERT INTO el_%1 (id) VALUES (%2);")
			.arg(type).arg(id));

	// Create container table;
	getQuery(QString("CREATE TABLE `%1` (id mediumint, type mediumint, x mediumint, y mediumint);").arg(tableName));

	return true;
}

int RealRepoItem::rowCount() const
{
	if ( childCount == -1 ) {
		if ( childQuery.isActive() ) {
			UNCONST childCount = childQuery.size();
		} else {
			UNCONST childCount = getQuery(QString("SELECT count(*) FROM %1;").arg(childTableName) ).toInt();
		}
	}
	return childCount;
}

void RealRepoItem::childrenCleanup()
{
	childCount = -1;
	childQuery.clear();
	qDeleteAll(childItems);
	childItems.clear();
}
