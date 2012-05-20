#include "gvizNeatoLayoutHelper.h"
#include "ui_gvizNeatoLayoutHelperSettings.h"

#include <QRectF>
#include <QPointF>

#include <graphviz/gvc.h>
#include <graphviz/graph.h>

gvizNeatoLayoutHelper::gvizNeatoLayoutHelper()
		: mSettingsUi(new Ui::gvizNeatoSettingsForm),
		mSettingsPage(new QWidget)
{
	mSettingsUi->setupUi(mSettingsPage);
}

QMap<Graph::VertexId, QPointF> gvizNeatoLayoutHelper::arrange(Graph const &graph
		, QMap<Graph::VertexId, QRectF> const &graphGeometry)
{
	QByteArray sep = QString("+%1").arg(mSettingsUi->spinBox->value()).toAscii();
	QByteArray mode = mSettingsUi->modeBox->currentText().toAscii();
	QByteArray overlap = mSettingsUi->overlapBox->currentText().toAscii();

	QHash<Graph::VertexId, Agnode_t*> h;
	GVC_t *gvc = gvContext();
	Agraph_t *G = agopen("", AGDIGRAPH);
	agraphattr(G, "overlap", overlap.data());
	agraphattr(G, "normalize", "true");
	agraphattr(G, "mode", mode.data());
	agraphattr(G, "sep", sep.data());
	agraphattr(G, "splines", "false");

	foreach (Graph::VertexId const vertex, graph.getVertices()) {
		QString number = QString("%1").arg(vertex);
		Agnode_t *node = agnode(G, number.toAscii().data());
		if (graphGeometry.contains(vertex)) {
			QRectF const &rect = graphGeometry[vertex];
			QString xPos = QString("%1").arg(rect.x());
			QString yPos = QString("%1").arg(rect.y());

			// width and height should be in inches
			QString width = QString("%1").arg(rect.width() / 48.);
			QString height = QString("%1").arg(rect.height() / 48.);

			agattr(node, "x", xPos.toAscii().data());
			agattr(node, "y", yPos.toAscii().data());

			agset(node, "width", width.toAscii().data());
			agset(node, "height", height.toAscii().data());
			agattr(node, "width", width.toAscii().data());
			agattr(node, "height", height.toAscii().data());

			agattr(node, "fixedsize", "true");
			agattr(node, "shape", "rect");
		}
		h[vertex] = node;
	}

	foreach (Graph::EdgeId const edgeId, graph.getEdges()) {
		const QPair<Graph::VertexId, Graph::VertexId>& v = graph.getAdjacentVertices(edgeId);
		Agedge_t *edge = agedge(G, h[v.first], h[v.second]);
	}

	gvLayout(gvc, G, "neato");
	gvRender(gvc, G, "dot", NULL);

	QMap<Graph::VertexId, QPointF> positionData;
	foreach (Graph::VertexId const vertex, graph.getVertices()) {
		Agnode_t *node = h[vertex];
		positionData[vertex] = QPointF(node->u.coord.x, node->u.coord.y);
	}

	gvFreeLayout(gvc, G);
	agclose(G);

	return positionData;
}

QWidget* gvizNeatoLayoutHelper::settingsPage() const
{
	return mSettingsPage;
}
