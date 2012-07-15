#include <QtCore/QRectF>
#include <QtCore/QPointF>

#include <cstdio>

#include <graphviz/gvc.h>
#include <graphviz/graph.h>

#include "gvizDotLayoutHelper.h"
#include "ui_gvizDotLayoutHelperSettings.h"

using namespace qReal;

GvizDotLayoutHelper::GvizDotLayoutHelper()
		: mSettingsPage(new QWidget())
		, mSettingsUi(new Ui::gvizDotSettingsForm())
{
	mSettingsUi->setupUi(mSettingsPage);
}

GvizDotLayoutHelper::~GvizDotLayoutHelper()
{
	delete mSettingsPage;
	delete mSettingsUi;
}

QMap<Graph::VertexId, QPointF> GvizDotLayoutHelper::arrange(Graph const &graph
		, QMap<Graph::VertexId, QRectF> const &graphGeometry)
{
	QByteArray nodeSep = QString::number(mSettingsUi->nodeSep->value()).toAscii();
	QByteArray minLen = QString::number(mSettingsUi->minLen->value()).toAscii();
	QByteArray rankDir;
	switch (mSettingsUi->rankdir->currentIndex()) {
	case TB:
		rankDir = QString("BT").toAscii();
		break;
	case BT:
		rankDir = QString("TB").toAscii();
		break;
	case LR:
		rankDir = QString("LR").toAscii();
		break;
	case RL:
		rankDir = QString("RL").toAscii();
		break;
	}

	GVC_t *gvzContext = gvContext();
	Agraph_t *aggraph = agopen("", AGDIGRAPH);

	agraphattr(aggraph, "dpi", "48");
	agraphattr(aggraph, "nodesep", nodeSep.data());
	agraphattr(aggraph, "ranksep", "1");
	agraphattr(aggraph, "rankdir", rankDir.data());
	agraphattr(aggraph, "remincross", "true");
	agraphattr(aggraph, "overlap", "scalexy");
	agraphattr(aggraph, "splines", "false");

	QHash<Graph::VertexId, Agnode_t *> vertexToAgnode;
	foreach (Graph::VertexId const &vertex, graph.getVertices()) {
		QString const number = QString::number(vertex);
		Agnode_t *node = agnode(aggraph, number.toAscii().data());
		if (graphGeometry.contains(vertex)) {
			QRectF const &rect = graphGeometry[vertex];
			QString const xPos = QString::number(rect.x());
			QString const yPos = QString::number(rect.y());

			// width and height should be in inches, but if we divide by 96, it'd be too small
			QString const width = QString::number(rect.width() / 48.0);
			QString const height = QString::number(rect.height() / 48.0);

			agattr(node, "x", xPos.toAscii().data());
			agattr(node, "y", yPos.toAscii().data());

			agset(node, "width", width.toAscii().data());
			agset(node, "height", height.toAscii().data());
			agattr(node, "width", width.toAscii().data());
			agattr(node, "height", height.toAscii().data());

			agattr(node, "fixedsize", "true");
			agattr(node, "shape", "box");
		}
		vertexToAgnode[vertex] = node;
	}

	foreach (Graph::EdgeId const &edgeId, graph.getEdges()) {
		QPair<Graph::VertexId, Graph::VertexId> const adjVertices = graph.getAdjacentVertices(edgeId);
		Agedge_t *edge = agedge(aggraph, vertexToAgnode[adjVertices.first], vertexToAgnode[adjVertices.second]);
		agattr(edge, "minlen", minLen.data());
	}

	gvLayout(gvzContext, aggraph, "dot");
	gvRender(gvzContext, aggraph, "dot", NULL);

	QMap<Graph::VertexId, QPointF> positionData;
	foreach (Graph::VertexId const &vertex, graph.getVertices()) {
		Agnode_t *node = vertexToAgnode[vertex];
		positionData[vertex] = QPointF(node->u.coord.x, node->u.coord.y);
	}

	gvFreeLayout(gvzContext, aggraph);
	agclose(aggraph);
	gvFreeContext(gvzContext);

	return positionData;
}

QWidget *GvizDotLayoutHelper::settingsPage() const
{
	return mSettingsPage;
}

QString GvizDotLayoutHelper::name() const
{
	return "Graphviz Dot";
}
