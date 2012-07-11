#include "serializationData.h"

QDataStream& operator<< (QDataStream &out, NodeData const &data)
{
	out << data.mId << data.mLogicalId  << data.mParentId
			<< data.mPos << data.mContents << data.mProperties
			<< data.mConfiguration << data.mTypeId;
	return out;
}

QDataStream& operator>> (QDataStream &in, NodeData &data)
{
	in >> data.mId >> data.mLogicalId >> data.mParentId >> data.mPos
		>> data.mContents >> data.mProperties
		>> data.mConfiguration >> data.mTypeId;
	return in;
}

QDataStream& operator<< (QDataStream &out, EdgeData const &data)
{
	out << data.mId << data.mLogicalId << data.mSrcId
			<< data.mDstId << data.mPortFrom << data.mPortTo
			<< data.mConfiguration << data.mPos << data.mTypeId;
	return out;
}

QDataStream& operator>> (QDataStream &in, EdgeData &data)
{
	in >> data.mId >> data.mLogicalId >> data.mSrcId
			>> data.mDstId >> data.mPortFrom >> data.mPortTo
			>> data.mConfiguration >> data.mPos >> data.mTypeId;
	return in;
}

bool operator== (NodeData const &first, NodeData const &second)
{
	return first.mId == second.mId && first.mLogicalId == second.mLogicalId
			&& first.mParentId == second.mParentId && first.mPos == second.mPos
			&& first.mProperties == second.mProperties && first.mTypeId == second.mTypeId;
}
