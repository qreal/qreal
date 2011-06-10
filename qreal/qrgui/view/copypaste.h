#pragma once

#include <QtCore>
#include <QMap>
#include <QDataStream>
#include "../kernel/ids.h"

struct ElementSerializationData
{
	qReal::Id mId;
	qReal::Id mLogicalId;

	QString mName;

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

inline bool operator== (const NodeElementSerializationData &data1, const NodeElementSerializationData &data2)
{
	return data1.mId == data2.mId
		&& data1.mLogicalId == data2.mLogicalId
		&& data1.mName == data2.mName
		&& data1.mProperties == data2.mProperties
		&& data1.mParentId == data2.mParentId
		&& data1.mPos == data2.mPos
		&& data1.mContenets == data2.mContenets;
}

inline bool operator== (const EdgeElementSerializationData &data1, const EdgeElementSerializationData &data2)
{
	return data1.mId == data2.mId
		&& data1.mLogicalId == data2.mLogicalId
		&& data1.mName == data2.mName
		&& data1.mProperties == data2.mProperties
		&& data1.mSrcId == data2.mSrcId
		&& data1.mDstId == data2.mDstId
		&& data1.mPortFrom == data2.mPortFrom
		&& data1.mPortTo == data2.mPortTo;
}

inline bool operator!= (const NodeElementSerializationData &data1, const NodeElementSerializationData &data2)
{
	return !(data1 == data2);
}

inline bool operator!= (const EdgeElementSerializationData &data1, const EdgeElementSerializationData &data2)
{
	return !(data1 == data2);
}
