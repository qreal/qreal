#include "autoLayoutDialog.h"
#include "ui_autoLayoutDialog.h"

#include "../umllib/layout/gvizDotLayoutHelper.h"
#include "../umllib/layout/gvizNeatoLayoutHelper.h"

#include "../umllib/edgeElement.h"
#include "../umllib/nodeElement.h"

AutoLayoutDialog::AutoLayoutDialog(QWidget *parent) :
		QDialog(parent),
		mUi(new Ui::AutoLayoutDialog)
{
	mUi->setupUi(this);
	connect(mUi->methodSelector, SIGNAL(currentIndexChanged(QString)), this, SLOT(methodSelected(QString)));
	connect(mUi->buttonBox, SIGNAL(accepted()), this, SLOT(runMethod()));

//	mUi->methodSettingsContainer->hide();
#ifdef GRAPHVIZ_INSTALLED
	addLayoutHelper(new gvizDotLayoutHelper);
	addLayoutHelper(new gvizNeatoLayoutHelper);
#else
	// TODO: add message about installing graphViz
#endif
}

AutoLayoutDialog::~AutoLayoutDialog()
{
	delete mUi;
	foreach (AbstractLayoutHelper *lh, mLayoutHelpers) {
		delete lh;
	}
}


void AutoLayoutDialog::addLayoutHelper(AbstractLayoutHelper *layoutHelper)
{
	mLayoutHelpers.push_back(layoutHelper);
	mUi->methodSelector->insertItem(mLayoutHelpers.size(), layoutHelper->name());

	QWidget *widget = layoutHelper->settingsPage();
	widget->setParent(mUi->methodSettingsContainer);
	widget->hide();
//	mUi->methodSettingsContainer->addWidget(widget);
}

void AutoLayoutDialog::methodSelected(QString methodName)
{
//	if (methodIdx == 0) {
//		mUi->methodSettingsContainer->hide();
//		return;
//	}
	foreach (AbstractLayoutHelper *lh, mLayoutHelpers) {
		lh->settingsPage()->hide();
	}

	foreach (AbstractLayoutHelper *lh, mLayoutHelpers) {
		if (lh->name() == methodName) {
			lh->settingsPage()->show();
			break;
		}
	}
//	mUi->methodSettingsContainer->show();
//	mUi->methodSettingsContainer->setCurrentIndex(methodIdx);
}

void AutoLayoutDialog::runMethod()
{
	int idx = mUi->methodSelector->currentIndex();
	if (idx == 0) {
		return;
	}

	AbstractLayoutHelper *layoutHelper = mLayoutHelpers.at(idx - 1);

	Graph graph;
	QMap<Graph::VertexId, QRectF> graphGeometry;
	QMap<Element *, Graph::VertexId> map;
	QList<Element *> nodes;
	QList<EdgeElement *> edges;

	foreach (Element *e, mElements) {
		EdgeElement *edge = dynamic_cast<EdgeElement*>(e);
		if (edge == NULL) {
			nodes.push_back(e);
		} else {
			edges.push_back(edge);
		}
	}

	foreach (Element *e, nodes) {
		Graph::VertexId vertex = graph.addVertex();

		map[e] = vertex;
		graphGeometry[vertex] = e->boundingRect();
	}

	foreach (EdgeElement *e, edges) {
		Element *from = e->src(),
				*to = e->dst();
		graph.addEdge(map.value(from), map.value(to));
	}

	QMap<Graph::VertexId, QPointF> positionData = layoutHelper->arrange(graph, graphGeometry);

	foreach (Element *e, nodes) {
		Graph::VertexId vertex = map[e];
		QPointF pos = positionData[vertex];
		e->setPos(pos);
		NodeElement *node = dynamic_cast<NodeElement *>(e);
		node->adjustLinks();
		node->arrangeLinks();
		node->storeGeometry();
	}

}
