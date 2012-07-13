#pragma once

#include <QtCore>
#include <QMap>
#include <QDataStream>
#include <QPolygon>
#include "../../qrkernel/ids.h"

using namespace qReal;

/** @class EdgeData
  * @brief EdgeElement serialization data for copy/paste
  */
class EdgeData
{
public:
	EdgeData()
		: id(Id::rootId()), srcId(Id::rootId()), dstId(Id::rootId())
		, pos(QPointF(0, 0))
	{}

	Id id;
	Id srcId;
	Id dstId;

	qreal portFrom;
	qreal portTo;

	QPolygon configuration;
	QPointF pos;

};

/** @class NodeData
  * @brief NodeElement serialization data for copy/paste
  */
class NodeData
{
public:
	NodeData()
		: id(Id::rootId()), parentId(Id::rootId()), pos(QPointF(0, 0))
	{}

	qReal::Id id;

	QMap<QString, QVariant> properties;

	qReal::Id parentId;

	QPointF pos;
	QRectF contents;

};

QDataStream& operator<< (QDataStream &out, NodeData const &data);
QDataStream& operator>> (QDataStream &in, NodeData &data);

QDataStream& operator<< (QDataStream &out, EdgeData const &data);
QDataStream& operator>> (QDataStream &in, EdgeData &data);

bool operator== (NodeData const &first, NodeData const &second);
