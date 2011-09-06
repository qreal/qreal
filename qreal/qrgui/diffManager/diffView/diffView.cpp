#include "diffView.h"
#include "../details/elementType.h"

using namespace qReal::diffManager::diffView;
using namespace qReal::diffManager::details;

DiffView::DiffView(qReal::MainWindow *mainWindow, DiffProvider *diffProvider, bool isOldModel, QWidget *parent)
	: EditorView(parent), mDiffProvider(diffProvider), mIsOldModel(isOldModel),
	mModel((mIsOldModel) ? mDiffProvider->oldModel() : mDiffProvider->newModel())
{
	setEditorViewScene(new DiffScene(this));
	setMainWindow(mainWindow);
	mDetailsWidget = NULL;

	mvIface()->setAssistApi(mModel->graphicalModelAssistApi(), mModel->logicalModelAssistApi());
	mvIface()->setModel(mModel->graphicalModel());
	mvIface()->setLogicalModel(mModel->logicalModel());
	mvIface()->setRootIndex(mModel->graphicalModel()->index(0,0));

	diffScene()->initForDiff();
	highlightElements();
}

void DiffView::setDetailsWidget(detailsWidget::DiffDetailsWidget *widget)
{
	mDetailsWidget = widget;
	connect(mDetailsWidget, SIGNAL(mouseEnteredIdWidget(qReal::Id)),
		this, SLOT(hintItem(qReal::Id)));
	connect(mDetailsWidget, SIGNAL(mouseLeavedIdWidget(qReal::Id)),
		this, SLOT(unhintItem(qReal::Id)));
}

void DiffView::mousePressEvent(QMouseEvent *event)
{
	QGraphicsView::mousePressEvent(event);
	if (event->button() != Qt::LeftButton)
	{
		event->ignore();
		return;
	}
	event->accept();

	if (QGraphicsItem *item = itemAt(event->pos()))
	{
		if (UML::Element *elem = dynamic_cast < UML::Element * >(item))
		{
			onClick(elem->id());
		}
	}
	else
	{
		onClickCancel();
	}
}

void DiffView::dragEnterEvent(QDragEnterEvent *event)
{
	event->ignore();
}

void DiffView::dragLeaveEvent(QDragLeaveEvent *event)
{
	event->ignore();
}

void DiffView::dragMoveEvent(QDragMoveEvent *event)
{
	event->ignore();
}

void DiffView::dropEvent(QDropEvent *event)
{
	event->ignore();
}

void DiffView::keyPressEvent(QKeyEvent *event)
{
	QGraphicsView::keyPressEvent(event);
	event->accept();
}

void DiffView::keyReleaseEvent(QKeyEvent *event)
{
	QGraphicsView::keyReleaseEvent(event);
	event->accept();
}

void DiffView::mouseDoubleClickEvent(QMouseEvent *event)
{
	QGraphicsView::mouseDoubleClickEvent(event);
	event->accept();
}

void DiffView::mouseMoveEvent(QMouseEvent *event)
{
	QGraphicsView::mouseMoveEvent(event);
	event->accept();
}

void DiffView::mouseReleaseEvent(QMouseEvent *event)
{
	QGraphicsView::mouseReleaseEvent(event);
	event->accept();
}

void DiffView::onClick(const qReal::Id &id)
{
	if (mDetailsWidget)
		mDetailsWidget->setId(id);
}

void DiffView::onClickCancel()
{
	if (mDetailsWidget)
		mDetailsWidget->reset();
}

DiffScene *DiffView::diffScene()
{
	return dynamic_cast<DiffScene *>(mvIface()->scene());
}

void DiffView::highlightElements()
{
	qReal::IdList ids = (mIsOldModel) ? mDiffProvider->oldGraphicalElements() : mDiffProvider->newGraphicalElements();
	foreach (qReal::Id id, ids)
	{
		highlight(id);
	}
}

void DiffView::highlight(const qReal::Id &id)
{
	Difference *difference = mDiffProvider->difference(id);
	if (difference)
	{
		DiffState state = (PurelyGraphical == difference->elementType()) ?
				  difference->graphicalDifference()->state() : difference->logicalDifference()->state();
		diffScene()->highlight(id, state);
	}
}

void DiffView::hintItem(const qReal::Id &graphicalId)
{
	diffScene()->hint(graphicalId);
}

void DiffView::unhintItem(const qReal::Id &graphicalId)
{
	highlight(graphicalId);
}
