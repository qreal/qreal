#include "serializationData.h"

QDataStream& operator<< (QDataStream &out, NodeData const &data)
{
	out << data.mId << data.mParentId << data.mPos
			<< data.mContents << data.mProperties;
	return out;
}

QDataStream& operator>> (QDataStream &in, NodeData &data)
{
	in >> data.mId >> data.mParentId >> data.mPos
			>> data.mContents >> data.mProperties;
	return in;
}

QDataStream& operator<< (QDataStream &out, EdgeData const &data)
{
	out << data.mId << data.mSrcId << data.mDstId
			<< data.mPortFrom << data.mPortTo
			<< data.mConfiguration << data.mPos;
	return out;
}

QDataStream& operator>> (QDataStream &in, EdgeData &data)
{
	in >> data.mId >> data.mSrcId >> data.mDstId
			>> data.mPortFrom >> data.mPortTo
			>> data.mConfiguration >> data.mPos;
	return in;
}

bool operator== (NodeData const &first, NodeData const &second)
{
	return first.mId == second.mId && first.mParentId == second.mParentId
			&& first.mPos == second.mPos && first.mProperties == second.mProperties
			&& first.mContents == second.mContents;
}
