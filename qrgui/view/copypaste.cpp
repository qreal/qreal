#include "copypaste.h"

QDataStream& operator<< (QDataStream &out, const ElementSerializationData &data)
{
	return out << data.mId
			   << data.mLogicalId
			   << data.mName
			   << data.mProperties;
}

QDataStream& operator<< (QDataStream &out, const NodeElementSerializationData &data)
{
	return out << (ElementSerializationData)data
			   << data.mParentId
			   << data.mPos
			   << data.mContenets;
}

QDataStream& operator<< (QDataStream &out, const EdgeElementSerializationData &data)
{
	return out << (ElementSerializationData)data
			   << data.mSrcId
			   << data.mDstId
			   << data.mPortFrom
			   << data.mPortTo;
}

QDataStream& operator>> (QDataStream& in, NodeElementSerializationData &data)
{
	in >> data.mId
	   >> data.mLogicalId
	   >> data.mName
	   >> data.mProperties
	   >> data.mParentId
	   >> data.mPos
	   >> data.mContenets;

	return in;
}

QDataStream& operator>> (QDataStream& in, EdgeElementSerializationData &data)
{
	in >> data.mId
	   >> data.mLogicalId
	   >> data.mName
	   >> data.mProperties
	   >> data.mSrcId
	   >> data.mDstId
	   >> data.mPortFrom
	   >> data.mPortTo;

	return in;
}
