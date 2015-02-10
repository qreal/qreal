#pragma once

#include <QtCore/QMap>
#include <QtCore/QDataStream>
#include <QtGui/QPolygon>

#include <qrkernel/ids.h>

using namespace qReal;

/**
* @brief EdgeElement serialization data for copy/paste
*/

class EdgeData
{
public:
	EdgeData()
		: id(Id::rootId())
		, logicalId(Id::rootId())
		, srcId(Id::rootId())
		, dstId(Id::rootId())
		, pos(QPointF(0, 0))
	{}

	Id id;
	Id logicalId;

	Id srcId;
	Id dstId;

	qreal portFrom;
	qreal portTo;

	QPolygon configuration;
	QPointF pos;

	int shapeType;

	QMap<QString, QVariant> logicalProperties;
};

/**
* @brief NodeElement serialization data for copy/paste
*/

class NodeData
{
public:
	NodeData()
		: id(Id::rootId())
		, logicalId(Id::rootId())
		, parentId(Id::rootId())
		, pos(QPointF(0, 0))
	{}

	Id id;
	Id logicalId;

	QMap<QString, QVariant> logicalProperties;
	QMap<QString, QVariant> graphicalProperties;

	Id parentId;

	QPointF pos;
	QRectF contents;
	Id explosion;
};

QDataStream &operator<< (QDataStream &out, const NodeData &data);
QDataStream &operator>> (QDataStream &in, NodeData &data);

QDataStream &operator<< (QDataStream &out, const EdgeData &data);
QDataStream &operator>> (QDataStream &in, EdgeData &data);

bool operator== (const NodeData &first, const NodeData &second);
bool operator== (const EdgeData &first, const EdgeData &second);
