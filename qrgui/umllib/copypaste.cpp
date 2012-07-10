#include "copypaste.h"

QDataStream& operator<< (QDataStream &out, NodeData const &data)
{
	out << data.mId.toString() << data.mLogicalId.toString()
			<< data.mParentId.toString() << data.mPos << data.mContents;
//	out << data.mProperties;
	return out;
}

QDataStream& operator>> (QDataStream &in, NodeData &data)
{
	QString mIdString;
	QString mLogicalIdString;
	QString mParentIdString;

	in >> mIdString >> mLogicalIdString >> mParentIdString
			>> data.mPos >> data.mContents;// >> data.mProperties;

	data.mId = qReal::Id::loadFromString(mIdString);
	data.mLogicalId = qReal::Id::loadFromString(mLogicalIdString);
	data.mParentId = qReal::Id::loadFromString(mParentIdString);

	return in;
}

QDataStream& operator<< (QDataStream &out, EdgeData const &data)
{
	out << data.mId.toString() << data.mLogicalId.toString()
			<< data.mSrcId.toString() << data.mDstId.toString()
			<< data.mPortFrom << data.mPortTo;
	return out;
}

QDataStream& operator>> (QDataStream &in, EdgeData &data)
{
	QString mId;
	QString mLogicalId;
	QString mSrcId;
	QString mDstId;

	in >> mId >> mLogicalId >> mSrcId >> mDstId >> data.mPortFrom >> data.mPortTo;

	data.mId = qReal::Id::loadFromString(mId);
	data.mLogicalId = qReal::Id::loadFromString(mLogicalId);
	data.mSrcId = qReal::Id::loadFromString(mSrcId);
	data.mDstId = qReal::Id::loadFromString(mDstId);

	return in;
}

bool operator== (NodeData const &first, NodeData const &second)
{
	return first.mId == second.mId && first.mLogicalId == second.mLogicalId
			&& first.mParentId == second.mParentId && first.mPos == second.mPos;
	//		&& first.mProperties == second.mProperties;
}
