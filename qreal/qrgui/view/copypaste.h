#pragma once

#include <QtCore>
#include <QMap>
#include <QDataStream>
#include "../kernel/ids.h"

struct ElementSerializationData
{
	qReal::Id mId;
	qReal::Id mLogicalId;

	int mSerialId;

	QMap<QString, QVariant> mProperties;
};

struct NodeElementSerializationData : public ElementSerializationData
{
	int mParentSerialId;

	QPointF mPos;
	QRectF mContenets;
};

struct EdgeElementSerializationData : public ElementSerializationData
{
	int mSrcSerialId;
	int mDstSerialId;

	qreal mPortFrom;
	qreal mPortTo;
};

QDataStream& operator<< (QDataStream &out, const ElementSerializationData &data);
QDataStream& operator<< (QDataStream &out, const NodeElementSerializationData &data);
QDataStream& operator<< (QDataStream &out, const EdgeElementSerializationData &data);

QDataStream& operator>> (QDataStream& in, ElementSerializationData &data);
QDataStream& operator>> (QDataStream& in, NodeElementSerializationData &data);
QDataStream& operator>> (QDataStream& in, EdgeElementSerializationData &data);
