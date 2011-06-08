#pragma once

#include <QtCore>
#include <QMap>
#include <QDataStream>
#include "../kernel/ids.h"

struct ElementSerializationData
{
	qReal::Id mId;
	qReal::Id mLogicalId;

	QMap<QString, QVariant> mProperties;
};

struct NodeElementSerializationData : public ElementSerializationData
{
	qReal::Id mParentId;

	QPointF mPos;
	QRectF mContenets;
};

struct EdgeElementSerializationData : public ElementSerializationData
{
	qReal::Id mSrcId;
	qReal::Id mDstId;

	qreal mPortFrom;
	qreal mPortTo;
};

QDataStream& operator<< (QDataStream &out, const ElementSerializationData &data);
QDataStream& operator<< (QDataStream &out, const NodeElementSerializationData &data);
QDataStream& operator<< (QDataStream &out, const EdgeElementSerializationData &data);

QDataStream& operator>> (QDataStream& in, ElementSerializationData &data);
QDataStream& operator>> (QDataStream& in, NodeElementSerializationData &data);
QDataStream& operator>> (QDataStream& in, EdgeElementSerializationData &data);
