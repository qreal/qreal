#include "serializationData.h"

QDataStream& operator<< (QDataStream &out, NodeData const &data)
{
	out << data.id << data.parentId << data.pos
			<< data.contents << data.properties;
	return out;
}

QDataStream& operator>> (QDataStream &in, NodeData &data)
{
	in >> data.id >> data.parentId >> data.pos
			>> data.contents >> data.properties;
	return in;
}

QDataStream& operator<< (QDataStream &out, EdgeData const &data)
{
	out << data.id << data.srcId << data.dstId
			<< data.portFrom << data.portTo
			<< data.configuration << data.pos;
	return out;
}

QDataStream& operator>> (QDataStream &in, EdgeData &data)
{
	in >> data.id >> data.srcId >> data.dstId
			>> data.portFrom >> data.portTo
			>> data.configuration >> data.pos;
	return in;
}

bool operator== (NodeData const &first, NodeData const &second)
{
	return first.id == second.id && first.parentId == second.parentId
			&& first.pos == second.pos && first.properties == second.properties
			&& first.contents == second.contents;
}
