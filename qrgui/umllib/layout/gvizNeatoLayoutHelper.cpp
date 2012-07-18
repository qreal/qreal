#include <QtCore/QRectF>
#include <QtCore/QPointF>

#include "graphviz/gvc.h"
#include "graphviz/graph.h"

#include "gvizNeatoLayoutHelper.h"
#include "ui_gvizNeatoLayoutHelperSettings.h"

using namespace qReal;

GvizNeatoLayoutHelper::GvizNeatoLayoutHelper()
		: mSettingsPage(new QWidget())
		, mSettingsUi(new Ui::gvizNeatoSettingsForm())
{
	mSettingsUi->setupUi(mSettingsPage);
}

GvizNeatoLayoutHelper::~GvizNeatoLayoutHelper()
{
	delete mSettingsPage;
	delete mSettingsUi;
}

QMap<Graph::VertexId, QPointF> GvizNeatoLayoutHelper::arrange(Graph const &graph
		, QMap<Graph::VertexId, QRectF> const &graphGeometry)
{
	QByteArray sep = QString("+%1").arg(mSettingsUi->spinBox->value()).toAscii();
	QByteArray mode = mSettingsUi->modeBox->currentText().toAscii();
	QByteArray overlap = mSettingsUi->overlapBox->currentText().toAscii();

	QHash<Graph::VertexId, Agnode_t *> vertexToAgnode;

	#ifndef Q_OS_WIN32
		extern gvplugin_library_t gvplugin_neato_layout_LTX_library;
		extern gvplugin_library_t gvplugin_core_LTX_library;

		lt_symlist_t ltPreloadedSymbols[] =
		{
			{ "gvplugin_neato_layout_LTX_library", &gvplugin_neato_layout_LTX_library }
			, { "gvplugin_core_LTX_library", &gvplugin_core_LTX_library }
			, { 0, 0 }
		};

		GVC_t *gvzContext = gvContextPlugins(ltPreloadedSymbols, 1);
	#else
		GVC_t *gvzContext = gvContext();
	#endif

	Agraph_t *aggraph = agopen("", AGDIGRAPH);

	agraphattr(aggraph, "overlap", overlap.data());
	agraphattr(aggraph, "normalize", "true");
	agraphattr(aggraph, "mode", mode.data());
	agraphattr(aggraph, "sep", sep.data());
	agraphattr(aggraph, "splines", "false");

	foreach (Graph::VertexId const &vertex, graph.vertices()) {
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
			agattr(node, "shape", "rect");
		}
		vertexToAgnode[vertex] = node;
	}

	foreach (Graph::EdgeId const &edgeId, graph.edges()) {
		QPair<Graph::VertexId, Graph::VertexId> const adjVertices = graph.adjacentVertices(edgeId);
		Agedge_t *edge = agedge(aggraph, vertexToAgnode[adjVertices.first], vertexToAgnode[adjVertices.second]);
		Q_UNUSED(edge);
	}

	gvLayout(gvzContext, aggraph, "neato");
	gvRender(gvzContext, aggraph, "dot", NULL);

	QMap<Graph::VertexId, QPointF> positionData;
	foreach (Graph::VertexId const &vertex, graph.vertices()) {
		Agnode_t *node = vertexToAgnode[vertex];
		positionData[vertex] = QPointF(node->u.coord.x, node->u.coord.y);
	}

	gvFreeLayout(gvzContext, aggraph);
	agclose(aggraph);

	return positionData;
}

QWidget *GvizNeatoLayoutHelper::settingsPage() const
{
	return mSettingsPage;
}


QString GvizNeatoLayoutHelper::name() const
{
	return "Graphviz Neato";
}
