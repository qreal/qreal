#include "../../qrkernel/ids.h"

#include "graph.h"

#ifndef ABSTRACTLAYOUTHELPER_H
#define ABSTRACTLAYOUTHELPER_H

class AbstractLayoutHelper
{
public:
	virtual QMap<Graph::VertexId, QPointF> arrange(Graph const &graph
			, QMap<Graph::VertexId, QRectF> const &graphGeometry) = 0;

	virtual QString name() const = 0;
	virtual QWidget *settingsPage() const = 0;
};

#endif // ABSTRACTLAYOUTHELPER_H
