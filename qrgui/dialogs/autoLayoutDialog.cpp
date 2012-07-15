#include <QtGui/QPushButton>

#include "autoLayoutDialog.h"
#include "ui_autoLayoutDialog.h"

#include "../umllib/layout/gvizDotLayoutHelper.h"
#include "../umllib/layout/gvizNeatoLayoutHelper.h"

#include "../umllib/edgeElement.h"
#include "../umllib/nodeElement.h"

AutoLayoutDialog::AutoLayoutDialog(QWidget *parent)
		: QDialog(parent)
		, mUi(new Ui::AutoLayoutDialog())
{
	mUi->setupUi(this);
	connect(mUi->methodSelector, SIGNAL(currentIndexChanged(QString)), this, SLOT(methodSelected(QString)));
	QPushButton *applyButton = mUi->buttonBox->button(QDialogButtonBox::Apply);
	connect(applyButton, SIGNAL(clicked()), this, SLOT(prepareAndRunMethod()));

#ifdef GRAPHVIZ_INSTALLED
	addLayoutHelper(new GvizDotLayoutHelper());
	addLayoutHelper(new GvizNeatoLayoutHelper());
#endif
}

AutoLayoutDialog::~AutoLayoutDialog()
{
	delete mUi;
	foreach (AbstractLayoutHelper *lh, mLayoutHelpers) {
		delete lh;
	}
}

void AutoLayoutDialog::setMvIface(EditorViewMViface *mvIface)
{
	mMvIface = mvIface;
}

void AutoLayoutDialog::addLayoutHelper(AbstractLayoutHelper *abstractLayoutHelper)
{
	mLayoutHelpers.push_back(abstractLayoutHelper);
	mUi->methodSelector->insertItem(mLayoutHelpers.size(), abstractLayoutHelper->name());

	QWidget *widget = abstractLayoutHelper->settingsPage();
	widget->setParent(mUi->methodSettingsContainer);
	widget->hide();
}

void AutoLayoutDialog::methodSelected(QString const &methodName)
{
	foreach (AbstractLayoutHelper *lh, mLayoutHelpers) {
		lh->settingsPage()->hide();
	}

	foreach (AbstractLayoutHelper *lh, mLayoutHelpers) {
		if (lh->name() == methodName) {
			lh->settingsPage()->show();
			break;
		}
	}
}

void AutoLayoutDialog::prepareAndRunMethod()
{
	mElements.clear();
	
	models::GraphicalModelAssistApi *gAPI = mMvIface->graphicalAssistApi();
	EditorViewScene *scene = mMvIface->scene();

	foreach (Id const &id, gAPI->children(scene->rootItemId())) {
		Element *e = scene->getElem(id);
		if (e == NULL) {
			continue;
		}
		mElements.push_back(e);
	}
	
	runMethod();
}


void AutoLayoutDialog::runMethod()
{
	int index = mUi->methodSelector->currentIndex();
	if (index == 0) {
		return;
	}

	AbstractLayoutHelper *layoutHelper = mLayoutHelpers.at(index - 1);

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
		Element *from = e->src();
		Element *to = e->dst();
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
