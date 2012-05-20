#include "gvizDotLayoutHelper.h"
#include "ui_gvizDotLayoutHelperSettings.h"

#include <QRectF>
#include <QPointF>

#include <graphviz/gvc.h>
#include <graphviz/graph.h>

#include <cstdio>

gvizDotLayoutHelper::gvizDotLayoutHelper()
		: mSettingsUi(new Ui::gvizDotSettingsForm),
		mSettingsPage(new QWidget)
{
	mSettingsUi->setupUi(mSettingsPage);
}

QMap<Graph::VertexId, QPointF> gvizDotLayoutHelper::arrange(Graph const &graph
		, QMap<Graph::VertexId, QRectF> const &graphGeometry)
{
	QByteArray nodeSep = QString("%1").arg(mSettingsUi->doubleSpinBox->value()).toAscii();

	GVC_t *gvc = gvContext();
	Agraph_t *G = agopen("", AGDIGRAPH);

	agraphattr(G, "dpi", "48");
	agraphattr(G, "nodesep", nodeSep.data());
	agraphattr(G, "ranksep", "1");
	agraphattr(G, "remincross", "true");
	agraphattr(G, "overlap", "scalexy");
	agraphattr(G, "splines", "false");

	QHash<Graph::VertexId, Agnode_t*> vertex2gvNode;
	foreach (Graph::VertexId const vertex, graph.getVertices()) {
		QString number = QString("%1").arg(vertex);
		Agnode_t *node = agnode(G, number.toAscii().data());
		if (graphGeometry.contains(vertex)) {
			QRectF const &rect = graphGeometry[vertex];
			QString xPos = QString("%1").arg(rect.x());
			QString yPos = QString("%1").arg(rect.y());

			// width and height should be in inches, but if we divide by 96, it'd be too small
			QString width = QString("%1").arg(rect.width() / 48.);
			QString height = QString("%1").arg(rect.height() / 48.);

			agattr(node, "x", xPos.toAscii().data());
			agattr(node, "y", yPos.toAscii().data());

			// fckin gvz won't work with only 2 of these 4 lines
			agset(node, "width", width.toAscii().data());
			agset(node, "height", height.toAscii().data());
			agattr(node, "width", width.toAscii().data());
			agattr(node, "height", height.toAscii().data());

			agattr(node, "fixedsize", "true");
			agattr(node, "shape", "box");
		}
		vertex2gvNode[vertex] = node;
	}

	foreach (Graph::EdgeId const edgeId, graph.getEdges()) {
		const QPair<Graph::VertexId, Graph::VertexId>& v = graph.getAdjacentVertices(edgeId);
		Agedge_t *edge = agedge(G, vertex2gvNode[v.first], vertex2gvNode[v.second]);
		Q_UNUSED(edge);
	}

	gvLayout(gvc, G, "dot");
	gvRender(gvc, G, "dot", NULL);

	QMap<Graph::VertexId, QPointF> positionData;
	foreach (Graph::VertexId const &vertex, graph.getVertices()) {
		Agnode_t *node = vertex2gvNode[vertex];

		positionData[vertex] = QPointF(node->u.coord.x, node->u.coord.y);
	}

	gvFreeLayout(gvc, G);
	agclose(G);
	gvFreeContext(gvc);

	return positionData;
}

QWidget* gvizDotLayoutHelper::settingsPage() const
{
	return mSettingsPage;
}
