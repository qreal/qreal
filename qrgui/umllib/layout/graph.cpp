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

const QList<Graph::EdgeId> &Graph::getEdges() const
{
	return mEdges;
}

const QList<Graph::VertexId> &Graph::getVertices() const
{
	return mVertices;
}

const QList<Graph::EdgeId> &Graph::getAdjacentEdges(VertexId v) const
{
	return mVertexEdges.value(v);
}

const QPair<Graph::VertexId, Graph::VertexId> &Graph::getAdjacentVertices(EdgeId e) const
{
	return mEdgeVertices.value(e);
}
