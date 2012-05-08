#include <QMap>
#include <QList>
#include <QPair>

#ifndef GRAPH_H
#define GRAPH_H

class Graph {
public:
	Graph();

	typedef size_t VertexId;
	typedef size_t EdgeId;

	VertexId addVertex();
	EdgeId addEdge(VertexId from, VertexId to);

	const QList<EdgeId>& getEdges() const { return mEdges; }
	const QList<VertexId>& getVertices() const { return mVertices; }

	const QList<EdgeId>& getAdjacentEdges(VertexId v) const;
	const QPair<VertexId, VertexId>& getAdjacentVertices(EdgeId e) const;

private:
	QList<VertexId> mVertices;
	QList<EdgeId> mEdges;
	QMap<EdgeId, QPair<VertexId, VertexId> > mEdgeVertices;
	QMap<VertexId, QList<EdgeId> > mVertexEdges;
};

#endif // GRAPH_H
