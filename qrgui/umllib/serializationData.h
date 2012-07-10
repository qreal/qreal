#pragma once

#include <QtCore>
#include <QMap>
#include <QDataStream>
#include "../qrkernel/ids.h"

struct EdgeData
{
	qReal::Id mId;
	qReal::Id mLogicalId;

	qReal::Id mSrcId;
	qReal::Id mDstId;

	qreal mPortFrom;
	qreal mPortTo;
};

struct NodeData
{
	qReal::Id mId;
	qReal::Id mLogicalId;
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
