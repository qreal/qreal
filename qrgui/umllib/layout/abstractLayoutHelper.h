#pragma once

#include "../../../qrkernel/ids.h"

#include "graph.h"

namespace qReal {

/// Interface to add new auto layout algorithms
class AbstractLayoutHelper : public QObject
{
	Q_OBJECT
	
public:
	virtual ~AbstractLayoutHelper(){}
	
	/// Arranges graph nodes and edges
	/// @param graph Graph to arrange
	/// @param graphGeometry Geometry of vertices of graph
	/// @return Map of vertices and points on the scene where to draw it
	virtual QMap<Graph::VertexId, QPointF> arrange(Graph const &graph
			, QMap<Graph::VertexId, QRectF> const &graphGeometry) = 0;

	/// Algorithm name
	virtual QString name() const = 0;
	
	/// Algorithm configuration page
	virtual QWidget *settingsPage() const = 0;
};

}
