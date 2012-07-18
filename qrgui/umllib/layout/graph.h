#pragma once

#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QPair>

namespace qReal {

class Graph : public QObject
{
	Q_OBJECT

public:
	Graph();

	typedef size_t VertexId;
	typedef size_t EdgeId;

	VertexId addVertex();
	EdgeId addEdge(VertexId from, VertexId to);

	const QList<EdgeId> &edges() const;
	const QList<VertexId> &vertices() const;

	const QList<EdgeId> &adjacentEdges(VertexId v) const;
	const QPair<VertexId, VertexId> &adjacentVertices(EdgeId e) const;

private:
	QList<VertexId> mVertices;
	QList<EdgeId> mEdges;
	QMap<EdgeId, QPair<VertexId, VertexId> > mEdgeVertices;
	QMap<VertexId, QList<EdgeId> > mVertexEdges;
};

}
