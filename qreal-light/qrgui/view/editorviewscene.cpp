#include "editorviewscene.h"
#include "math.h"

#include <QGraphicsTextItem>
#include <QtGui>
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QGraphicsItem>

#include "editorviewmviface.h"
#include "editorview.h"
#include "mainwindow.h"
#include "../mainwindow/mainwindow.h"
#include "../model/model.h"

using namespace qReal;

EditorViewScene::EditorViewScene(QObject * parent)
	:  QGraphicsScene(parent), mWindow(NULL), mPrevParent(0)
{
	QSettings settings("SPbSU", "QReal");
	mNeedDrawGrid = settings.value("ShowGrid", true).toBool();
	mWidthOfGrid = static_cast<double>(settings.value("GridWidth", 10).toInt()) / 100;
	setItemIndexMethod(NoIndex);
	setEnabled(false);

	mActionSignalMapper = new QSignalMapper(this);
}

EditorViewScene::~EditorViewScene()
{
	delete mActionSignalMapper;
}

void EditorViewScene::drawGrid(QPainter *painter, const QRectF &rect)
{
	qreal sceneX = rect.x(), sceneY = rect.y();
	int startX = static_cast<int>(sceneX + 10) / indexGrid * indexGrid;
	int endX = static_cast<int>(sceneX + rect.width() - 10) / indexGrid * indexGrid;
	int startY = static_cast<int>(sceneY + 10) / indexGrid * indexGrid;
	int endY = static_cast<int>(sceneY + rect.height() - 10) / indexGrid * indexGrid;
	for (int i = startX; i <= endX; i = i + indexGrid) {
		QLineF line(i, startY, i, endY);
		painter->drawLine(line);
	}
	for (int i = startY; i <= endY; i = i + indexGrid) {
		QLineF line(startX, i, endX, i);
		painter->drawLine(line);
	}
}

void EditorViewScene::setEnabled(bool enabled)
{
	foreach (QGraphicsView *view, views())
		view->setEnabled(enabled);
}

void EditorViewScene::clearScene()
{
	foreach (QGraphicsItem *item, items())
		// looks really insane, but some elements were alreadt deleted together with their parent
		if (items().contains(item))
			removeItem(item);
}

UML::Element * EditorViewScene::getElem(qReal::Id const &uuid)
{
	if (uuid == ROOT_ID)
		return NULL;

	// FIXME: SLOW!
	QList < QGraphicsItem * > list = items();
	for (QList < QGraphicsItem * >::Iterator it = list.begin(); it != list.end(); ++it) {
		if (UML::Element * elem = dynamic_cast < UML::Element * >(*it)) {
			if (elem->uuid() == uuid) {
				return elem;
			}
		}
	}
	return NULL;
}

UML::Element * EditorViewScene::getElemByModelIndex(const QModelIndex &ind)
{
	// FIXME: SLOW!
	QList < QGraphicsItem * > list = items();
	for (QList < QGraphicsItem * >::Iterator it = list.begin(); it != list.end(); ++it) {
		if (UML::Element * elem = dynamic_cast < UML::Element * >(*it)) {
			if (elem->index() == ind)
				return elem;
		}
	}
	return NULL;
}

void EditorViewScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	const QMimeData *mimeData = event->mimeData();
	if (mimeData->hasFormat("application/x-real-uml-data"))
		QGraphicsScene::dragEnterEvent(event);
	else
		event->ignore();
}

void EditorViewScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
	Q_UNUSED(event);
}

void EditorViewScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
	Q_UNUSED(event);
}

bool EditorViewScene::canBeContainedBy(qReal::Id container, qReal::Id candidate)
{
	Q_UNUSED(container);
	Q_UNUSED(candidate); // TODO: update xml descriptions to remove
	return true;

	/*
	bool allowed = false;
	foreach (qReal::Id type, mWindow->manager()->getContainedTypes(container.type())){
		if (candidate.element() ==  type.editor())
			allowed = true;
	}
	return allowed;
	*/
}

void EditorViewScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	Q_ASSERT(mWindow); // should be initialized separately.
	// constuctor is bad for this, because the scene is created in generated .ui file

	// if there's no diagrams. create nothing
	if (mv_iface->model()->rowCount(QModelIndex()) == 0)
		return;

	createElement(event->mimeData(),event->scenePos());
}

qReal::Id *EditorViewScene::createElement(const QString &str)
{
	qReal::Id* result = createElement(str, mCreatePoint);
	lastCreatedWithEdge = getElem(*result);
	return result;
}

qReal::Id *EditorViewScene::createElement(const QString &str, QPointF scenePos)
{
	Id typeId = Id::loadFromString(str);
	Id *objectId = new Id(typeId.editor(),typeId.diagram(),typeId.element(),QUuid::createUuid().toString());

	QByteArray data;
	QMimeData *mimeData = new QMimeData();
	QDataStream stream(&data, QIODevice::WriteOnly);
	QString mimeType = QString("application/x-real-uml-data");
	QString uuid = objectId->toString();
	QString pathToItem = ROOT_ID.toString();
	QString name = "(anonymous something)";
	QPointF pos = QPointF(0, 0);
	stream << uuid;
	stream << pathToItem;
	stream << name;
	stream << pos;

	mimeData->setData(mimeType, data);
	createElement(mimeData, scenePos);
	delete mimeData;

	return objectId;
}

void EditorViewScene::createElement(const QMimeData *mimeData, QPointF scenePos)
{
	QByteArray itemData = mimeData->data("application/x-real-uml-data");
	QDataStream in_stream(&itemData, QIODevice::ReadOnly);

	QString uuid = "";
	QString pathToItem = "";
	QString name;
	QPointF pos;
	in_stream >> uuid;
	in_stream >> pathToItem;
	in_stream >> name;
	in_stream >> pos;

	QByteArray newItemData;
	QDataStream stream(&newItemData, QIODevice::WriteOnly);

	UML::Element *newParent = NULL;

	// TODO: make it simpler
	qReal::Id id = qReal::Id::loadFromString(uuid);
	UML::Element* e = mWindow->manager()->graphicalObject(id);

	// TODO: what is it??
	if (dynamic_cast<UML::NodeElement*>(e))
		newParent = getElemAt(scenePos);

	if (e)
		delete e;

	if( newParent ){
		if (!canBeContainedBy(newParent->uuid(), id)){
			QMessageBox::critical(0, "Error!", "[some text]");
			return;
		}

		//temporary solution for chaotic changes of coordinates of created elements with edge menu
		UML::EdgeElement* edge = dynamic_cast<UML::EdgeElement*>(newParent);
		if (edge)
			newParent = NULL;
	}

	stream << uuid;
	stream << pathToItem;
	stream << name;

	if (!newParent)
		stream << scenePos;
	else
		stream << newParent->mapToItem(newParent, newParent->mapFromScene(scenePos));

	QMimeData *newMimeData = new QMimeData;
	newMimeData->setData("application/x-real-uml-data", newItemData);
	QModelIndex parentIndex = newParent ? QModelIndex(newParent->index()) : mv_iface->rootIndex();
	mv_iface->model()->dropMimeData(newMimeData, Qt::CopyAction,
									mv_iface->model()->rowCount(parentIndex), 0, parentIndex);

	emit elementCreated(id);

	delete newMimeData;
}

void EditorViewScene::keyPressEvent(QKeyEvent *event)
{
	if (dynamic_cast<QGraphicsTextItem*>(focusItem())) {
		// Forward event to text editor
		QGraphicsScene::keyPressEvent(event);
	} else if (event->key() == Qt::Key_Delete) {
		// Delete selected elements from scene
		mainWindow()->deleteFromScene();
	} else
		QGraphicsScene::keyPressEvent(event);
}

void EditorViewScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	// Let scene update selection and perform other operations
	QGraphicsScene::mousePressEvent(event);

	if( event->button() == Qt::LeftButton ){
		QGraphicsItem *item = itemAt(event->scenePos());
		UML::ElementTitle *title = dynamic_cast < UML::ElementTitle * >(item);

		if (title) // check whether we accidently clicked on a title or not
			item = item->parentItem();

		if (item) {
			mPrevParent = item->parentItem();
			mPrevPosition = item->pos();
		}

	} else if (event->button() == Qt::RightButton) {
		UML::Element *e = getElemAt(event->scenePos());
		if (e && !e->isSelected()) {
			clearSelection();
			e->setSelected(true);
		}
	}
	if (mNeedDrawGrid)
		invalidate();
}

void EditorViewScene::initContextMenu(UML::Element *e, const QPointF &pos)
{
	QMenu menu;
	menu.addAction(mWindow->ui.actionDeleteFromDiagram);
	QList<UML::ContextMenuAction*> elementActions = e->contextMenuActions();

	if (!elementActions.isEmpty())
		menu.addSeparator();

	foreach (UML::ContextMenuAction* action, elementActions) {
		action->setEventPos(e->mapFromScene(pos));
		menu.addAction(action);

		connect(action, SIGNAL(triggered()), mActionSignalMapper, SLOT(map()),
				Qt::UniqueConnection);
		mActionSignalMapper->setMapping(action, action->text() + "###" + e->uuid().toString());
	}
	menu.addSeparator();

	menu.exec(QCursor::pos());
}

void EditorViewScene::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mouseReleaseEvent(event);

	UML::Element* element = getElemAt(event->scenePos());

	if (event->button() == Qt::RightButton)
	{
		if (element)
			initContextMenu(element, event->scenePos());
		return;
	}

	if (!element)
		return;
	UML::Element *parent;
	parent = getElemByModelIndex(element->index().parent());
	if (parent) {
		if (!canBeContainedBy(parent->uuid(), element->uuid())){
			QMessageBox::critical(0, "Ololo", "can't drop it here!111");
			// fail, reparenting the element as it was before
			foreach (QGraphicsItem *item, items(event->scenePos())) {
				UML::Element * elem = dynamic_cast < UML::Element * >(item);
				if (elem && elem->uuid() == element->uuid()) {
					QModelIndex ind = mv_iface->rootIndex();
					UML::Element * prevParent = dynamic_cast < UML::Element * >(mPrevParent);
					if (prevParent)
						ind = prevParent->index();
					if (model())
						model()->changeParent(element->index(), ind, mPrevPosition);
				}
			}
		}
	}
	if (mNeedDrawGrid)
		invalidate();
}

void EditorViewScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseMoveEvent(event);
}


void EditorViewScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		// Double click on a title activates it
		if (UML::ElementTitle *title = dynamic_cast<UML::ElementTitle*>(itemAt(event->scenePos()))) {
			if (!title->hasFocus()) {  // Do not activate already activated item
				event->accept();
				title->startTextInteraction();
				return;
			}
		}
		else if (UML::NodeElement *element = dynamic_cast<UML::NodeElement*>(itemAt(event->scenePos()))) {
			event->accept();

			IdList outgoingLinks = model()->api().outgoingConnections(element->uuid());

			if (outgoingLinks.size() > 0)
				mainWindow()->activateItemOrDiagram(outgoingLinks[0]);
			else {
				IdList diagrams = model()->assistApi().diagramsAbleToBeConnectedTo(element->uuid());
				if (!diagrams.isEmpty()) {
					Id diagramType = model()->assistApi().editorManager().findElementByType(diagrams[0].element());
					model()->assistApi().createConnected(element->uuid(), diagramType);
				}
			}

			// Now scene is changed from outside. Being a mere mortal I do not
			// know whether it is good or not, but what is the destiny of
			// workflow after this return?
			return;
		}
	}

	QGraphicsScene::mouseDoubleClickEvent(event);
}

UML::Element* EditorViewScene::getElemAt(QPointF const &position)
{
	foreach (QGraphicsItem *item, items(position)) {
		UML::Element* e = dynamic_cast<UML::Element*>(item);
		if (e)
			return e;
	}
	return NULL;
}

QPersistentModelIndex EditorViewScene::rootItem()
{
	return mv_iface->rootIndex();
}

void EditorViewScene::setMainWindow(qReal::MainWindow *mainWindow)
{
	mWindow = mainWindow;
	connect(this, SIGNAL(elementCreated(qReal::Id)), mainWindow->listenerManager(), SIGNAL(objectCreated(qReal::Id)));
	connect(mActionSignalMapper, SIGNAL(mapped(QString)), mainWindow->listenerManager(), SIGNAL(contextMenuActionTriggered(QString)));
}

qReal::MainWindow *EditorViewScene::mainWindow() const
{
	return mWindow;
}

qReal::model::Model *EditorViewScene::model() const
{
	return dynamic_cast<qReal::model::Model *>(mv_iface->model());
}

void EditorViewScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	if (mNeedDrawGrid) {
		QSettings settings("SPbSU", "QReal");
		mWidthOfGrid = (settings.value("GridWidth", 10).toDouble()) / 100;
		painter->setPen(QPen(Qt::black, mWidthOfGrid));
		drawGrid(painter, rect);
	}
}

void EditorViewScene::setNeedDrawGrid(bool show)
{
	mNeedDrawGrid = show;
}

bool EditorViewScene::getNeedDrawGrid()
{
	return mNeedDrawGrid;
}

