#pragma once

#include <QtCore>
#include <QMap>
#include <QDataStream>
#include <QPolygon>
#include "../qrkernel/ids.h"

/** @struct EdgeData
  * @brief EdgeElement serialization data for copy/paste
  */
struct EdgeData
{
	qReal::Id mId;

	qReal::Id mSrcId;
	qReal::Id mDstId;

	qreal mPortFrom;
	qreal mPortTo;

	QPolygon mConfiguration;
	QPointF mPos;
};

/** @struct NodeData
  * @brief NodeElement serialization data for copy/paste
  */
struct NodeData
{
	qReal::Id mId;

	QMap<QString, QVariant> mProperties;

	qReal::Id mParentId;

	QPointF mPos;
	QRectF mContents;
};

QDataStream& operator<< (QDataStream &out, NodeData const &data);
QDataStream& operator>> (QDataStream &in, NodeData &data);

QDataStream& operator<< (QDataStream &out, EdgeData const &data);
QDataStream& operator>> (QDataStream &in, EdgeData &data);

bool operator== (NodeData const &first, NodeData const &second);
