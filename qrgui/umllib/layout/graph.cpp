#include "graph.h"

using namespace qReal;

Graph::Graph()
{
}

Graph::VertexId Graph::addVertex()
{
	VertexId vertex = mVertices.size();
	mVertices.push_back(vertex);
	return vertex;
}

Graph::EdgeId Graph::addEdge(VertexId from, VertexId to)
{
	EdgeId edge = mEdges.size();
	mEdges.push_back(edge);
	mEdgeVertices[edge] = qMakePair(from, to);
	return edge;
}

const QList<Graph::EdgeId> &Graph::edges() const
{
	return mEdges;
}

const QList<Graph::VertexId> &Graph::vertices() const
{
	return mVertices;
}

const QList<Graph::EdgeId> &Graph::adjacentEdges(VertexId v) const
{
	return mVertexEdges.value(v);
}

const QPair<Graph::VertexId, Graph::VertexId> &Graph::adjacentVertices(EdgeId e) const
{
	return mEdgeVertices.value(e);
}
