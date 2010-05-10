/** @file editorviewscene.cpp
 * 	@brief Сцена для отрисовки объектов
 * */
#include "editorviewscene.h"

#include <QGraphicsTextItem>
#include <QtGui>
#include <QtCore/QDebug>

#include "editorviewmviface.h"
#include "editorview.h"
#include "mainwindow.h"
#include "../mainwindow/mainwindow.h"
#include "../model/model.h"

using namespace qReal;

EditorViewScene::EditorViewScene(QObject * parent)
		:  QGraphicsScene(parent), mWindow(NULL), mPrevParent(0)
{
	setItemIndexMethod(NoIndex);
	setEnabled(false);
	mDraw = false;
	connect(this, SIGNAL(sceneRectChanged(const QRectF &rect )), this, SLOT(sceneRectChangedHandler(const QRectF &rect)));
}

/*Рисуем сетку*/
void EditorViewScene::drawGrid()
{
	int startX = sceneRect().center().x() - widthLineX / 2;
	int endX = sceneRect().center().x() + widthLineX / 2;
	int startY = sceneRect().center().y() - widthLineY / 2;
	int endY = sceneRect().center().y() + widthLineY / 2;
	qDebug() << startX << " " << endX << " " << startY << " " << endY;
	for (int i = startX; i <= endX; i = i + indexGrid) {
		QLineF line(i, startY, i, endY);
		addLine(line, QPen(Qt::black, 0.25, Qt::DashLine));
	}
	for (int i = startY; i <= endY; i = i + indexGrid) {
		QLineF line(startX, i, endX, i);
		addLine(line, QPen(Qt::black, 0.25, Qt::DashLine));
	}
	mDraw = true;
}

void EditorViewScene::setEnabled(bool enabled)
{
	foreach (QGraphicsView *view, views())
		view->setEnabled(enabled);
}

void EditorViewScene::clearScene()
{
	foreach (QGraphicsItem *item, items())
		// Выглядит довольно безумно, но некоторые элементы
		// оказываются уже удалены, потому как был удалён их родитель.
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
	Q_UNUSED(candidate); // задолбали ворнинги. убрать, когда пофиксят хмли
	// Типизированные контейнеры временно закомментированы, надо сначала поправить xml-ки.
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
	Q_ASSERT(mWindow);// Значение mWindow должно быть инициализировано
	// отдельно, через конструктор это делать нехорошо,
	// поскольку сцена создаётся в сгенерённом ui-шнике.

	// если нет ни одной диаграммы, то ничего не создаем.
	if (mv_iface->model()->rowCount(QModelIndex()) == 0)
		return;

	createElement(event->mimeData(),event->scenePos());
}

bool EditorViewScene::launchEdgeMenu(UML::EdgeElement *edge, QPointF scenePos)
{
	bool edgeDeleted = false;
	qDebug() << "---launchEdgeMenu() begin";
	edge->setSelected(true);

	QList<QObject*> toDelete;

	QMenu *edgeMenu = new QMenu();
	toDelete.append(edgeMenu);
	edgeMenu->addAction(mWindow->ui.actionDeleteFromDiagram);

	const QString title = "Create new element";
	QMenu *createMenu = new QMenu(title, edgeMenu);
	toDelete.append(createMenu);
	edgeMenu->addMenu(createMenu);

	QSignalMapper *menuSignalMapper = new QSignalMapper(this);
	toDelete.append(menuSignalMapper);
	foreach(Id editorId, mWindow->manager()->editors())
	{
		qDebug() << editorId.editor();
		QMenu *editor = new QMenu(editorId.editor(), createMenu);
		toDelete.append(editor);
		foreach(Id diagramId, mWindow->manager()->diagrams(editorId))
		{
			QMenu *diagram = new QMenu(diagramId.diagram(), editor);
			toDelete.append(diagram);
			foreach(Id elementId, mWindow->manager()->elements(diagramId))
			{
				QAction *element = new QAction(elementId.element(), diagram);
				diagram->addAction(element);
				toDelete.append(element);

				QObject::connect(element,SIGNAL(triggered()), menuSignalMapper,SLOT(map()));
				menuSignalMapper->setMapping(element, elementId.toString());
			}
			editor->addMenu(diagram);
		}
		createMenu->addMenu(editor);
	}

	mCreatePoint = scenePos;
	QObject::connect(menuSignalMapper,SIGNAL(mapped(const QString &)),this,SLOT(createElement(const QString &)));

	if (edgeMenu->exec(QCursor::pos()) == mWindow->ui.actionDeleteFromDiagram)
		edgeDeleted = true;

//	Чистка памяти.
//	foreach(QObject *object, toDelete)
//		delete object;

	qDebug() << "---launchEdgeMenu() end";
	return edgeDeleted;
}



qReal::Id *EditorViewScene::createElement(const QString &str)
{
	return createElement(str, mCreatePoint);
}

qReal::Id *EditorViewScene::createElement(const QString &str, QPointF scenePos)
{
	qDebug() << "createElement(..)" << str;
	Id typeId = Id::loadFromString(str);
	Id *objectId = new Id(typeId.editor(),typeId.diagram(),typeId.element(),QUuid::createUuid().toString());

	QByteArray data;
	QMimeData *mimeData = new QMimeData();
	QDataStream stream(&data, QIODevice::WriteOnly);
	QString mimeType = QString("application/x-real-uml-data");
	QString uuid = objectId->toString();
	QString pathToItem = ROOT_ID.toString();
	QString name = "(anonymous something)";
	QPointF pos = QPointF(0,0);
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
	qDebug() << "---createElement() begin";

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

	qDebug() << "---uuid:       " << uuid;
	qDebug() << "---pathToItem: " << pathToItem;
	qDebug() << "---name:       " << name;
	qDebug() << "---pos:        " << pos;

	QByteArray newItemData;
	QDataStream stream(&newItemData, QIODevice::WriteOnly);

	UML::Element *newParent = NULL;

	// TODO: возможно, это можно сделать проще
	qReal::Id id = qReal::Id::loadFromString(uuid);
	UML::Element *e = mWindow->manager()->graphicalObject(id);

	qDebug() << "new element uuid: " << e->uuid().toString();

	if (dynamic_cast<UML::NodeElement*>(e))
		newParent = getElemAt(scenePos);

	if (e)
		delete e;

	if( newParent ){
		if (!canBeContainedBy(newParent->uuid(), id)){
			QMessageBox::critical(0, "Error!", "[some text]");
			return;
		}
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

	delete newMimeData;
	qDebug() << "---createElement() end";
}

void EditorViewScene::keyPressEvent(QKeyEvent *event)
{
	if (dynamic_cast<QGraphicsTextItem*>(this->focusItem())) {
		// Forward event to text editor
		QGraphicsScene::keyPressEvent(event);
	} else if (event->key() == Qt::Key_Delete) {
		// Delete selected elements from scene
		mainWindow()->deleteFromScene();
	} else
		QGraphicsScene::keyPressEvent(event);
}

void EditorViewScene::createGoToSubmenu(QMenu * const goToMenu, QString const &name, qReal::IdList const &ids) const
{
	QMenu *menu = goToMenu->addMenu(name);
	foreach (Id element, ids) {
		QAction *action = menu->addAction(model()->api().name(element));
		connect(action, SIGNAL(triggered()), SLOT(goToActionTriggered()));
		action->setData(element.toVariant());
	}
}

void EditorViewScene::createAddConnectionMenu(UML::Element const * const element
	, QMenu &contextMenu, QString const &menuName
	, IdList const &connectableTypes, IdList const &alreadyConnectedElements
	, IdList const &connectableDiagrams, const char *slot) const
{
	QMenu *addConnectionMenu = contextMenu.addMenu(menuName);

	foreach (Id type, connectableTypes) {
		foreach (Id elementId, model()->api().elements(type)) {
			if (alreadyConnectedElements.contains(elementId))
				continue;
			QAction *action = addConnectionMenu->addAction(model()->api().name(elementId));
			connect(action, SIGNAL(triggered()), slot);
			QList<QVariant> tag;
			tag << element->uuid().toVariant() << elementId.toVariant();
			action->setData(tag);
		}
	}

	foreach (Id diagram, connectableDiagrams) {
		Id diagramType = model()->assistApi().editorManager().findElementByType(diagram.element());
		QString name = model()->assistApi().editorManager().friendlyName(diagramType);
		QString editorName = model()->assistApi().editorManager().friendlyName(Id(diagramType.editor()));
		QAction *action = addConnectionMenu->addAction("New " + editorName + "/" + name);
		connect(action, SIGNAL(triggered()), slot);
		QList<QVariant> tag;
		tag << element->uuid().toVariant() << diagramType.toVariant();
		action->setData(tag);
	}
}

void EditorViewScene::createDisconnectMenu(UML::Element const * const element
	, QMenu &contextMenu, QString const &menuName
	, IdList const &outgoingConnections, IdList const &incomingConnections
	, const char *slot) const
{
	QMenu *disconnectMenu = contextMenu.addMenu(menuName);
	IdList list = outgoingConnections;
	list.append(incomingConnections);

	foreach (Id elementId, list) {
		QAction *action = disconnectMenu->addAction(model()->api().name(elementId));
		connect(action, SIGNAL(triggered()), slot);
		QList<QVariant> tag;
		tag << element->uuid().toVariant() << elementId.toVariant();
		action->setData(tag);
	}
}

void EditorViewScene::createConnectionSubmenus(QMenu &contextMenu, UML::Element const * const element) const
{
	// Пункты меню, отвечающие за провязку, "привязать к".
	// TODO: Перенести это в элементы, они лучше знают, что они такое, а тут
	// сцене модель и апи приходится спрашивать.
	createAddConnectionMenu(element, contextMenu, tr("Add connection")
		, mWindow->manager()->getConnectedTypes(element->uuid().type())
		, model()->api().outgoingConnections(element->uuid())
		, model()->assistApi().diagramsAbleToBeConnectedTo(element->uuid())
		, SLOT(connectActionTriggered())
	);

	createDisconnectMenu(element, contextMenu, tr("Disconnect")
		, model()->api().outgoingConnections(element->uuid())
		, model()->api().incomingConnections(element->uuid())
		, SLOT(disconnectActionTriggered())
	);

	createAddConnectionMenu(element, contextMenu, tr("Add usage")
		, mWindow->manager()->getUsedTypes(element->uuid().type())
		, model()->api().outgoingUsages(element->uuid())
		, model()->assistApi().diagramsAbleToBeUsedIn(element->uuid())
		, SLOT(addUsageActionTriggered())
	);

	createDisconnectMenu(element, contextMenu, tr("Delete usage")
		, model()->api().outgoingUsages(element->uuid())
		, model()->api().incomingUsages(element->uuid())
		, SLOT(deleteUsageActionTriggered())
	);

	QMenu *goToMenu = contextMenu.addMenu(tr("Go to"));

	createGoToSubmenu(goToMenu, tr("Forward connection"), model()->api().outgoingConnections(element->uuid()));
	createGoToSubmenu(goToMenu, tr("Backward connection"), model()->api().incomingConnections(element->uuid()));
	createGoToSubmenu(goToMenu, tr("Uses"), model()->api().outgoingUsages(element->uuid()));
	createGoToSubmenu(goToMenu, tr("Used in"), model()->api().incomingUsages(element->uuid()));
}

void EditorViewScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (!mDraw)
		drawGrid();
	// Let scene update selection and perform other operations
	QGraphicsScene::mousePressEvent(event);

	if( event->button() == Qt::LeftButton ){
		QGraphicsItem *item = itemAt(event->scenePos());
		qDebug() << "item: " << item;
		UML::ElementTitle *title = dynamic_cast < UML::ElementTitle * >(item);

		if (title)  // проверяем, а не зацепились ли мы случайно за надпись, когда начали тащить элемент
			item = item->parentItem();

		if (item) {
			mPrevParent = item->parentItem();
			mPrevPosition = item->pos();
			qDebug() << "NEW mPrevParent: " << mPrevParent;
			qDebug() << "NEW pos: " << mPrevPosition;
		}

	} else if (event->button() == Qt::RightButton) {

		UML::Element *e = getElemAt(event->scenePos());
		if (!e)
			return;
		if (!e->isSelected()) {
			clearSelection();
			e->setSelected(true);
		}

		// Menu belongs to scene handler because it can delete elements.
		// We cannot allow elements to commit suicide.

		QMenu menu;
		menu.addAction(mWindow->ui.actionDeleteFromDiagram);
		QList<UML::ContextMenuAction*> elementActions = e->contextMenuActions();

		if (!elementActions.isEmpty())
			menu.addSeparator();

		foreach (UML::ContextMenuAction* action, elementActions) {
			action->setEventPos(e->mapFromScene(event->scenePos()));
			menu.addAction(action);
		}

		createConnectionSubmenus(menu, e);

		menu.exec(QCursor::pos());
	}
}

void EditorViewScene::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	// Let scene update selection and perform other operations
	QGraphicsScene::mouseReleaseEvent(event);

	UML::Element *element = getElemAt(event->scenePos());
	if (!element)
		return;

	qDebug() << "element" << element->uuid().toString();
	UML::Element *parent = getElemByModelIndex(element->index().parent());


	if (parent){
	qDebug() << "parent: " << parent->uuid().toString();

		if (!canBeContainedBy(parent->uuid(), element->uuid())){
			QMessageBox::critical(0, "Ololo", "can't drop it here!111");
			// фэйл, репарентим элемент обратно
			foreach (QGraphicsItem *item, items(event->scenePos())) {
				UML::Element * elem = dynamic_cast < UML::Element * >(item);
				if (elem && elem->uuid() == element->uuid()) {
					QModelIndex ind = mv_iface->rootIndex();
					UML::Element * prevParent = dynamic_cast < UML::Element * >(mPrevParent);
					qDebug() << "prev parent:  " << mPrevParent;

					if (prevParent)
						ind = prevParent->index();

					if (model())
						model()->changeParent(element->index(), ind, mPrevPosition);

//					elem->setParentItem(mPrevParent);
//					elem->setPos(mPrevPosition);
					qDebug() << "new pos: " << elem->scenePos() << elem->pos();
				}
			}
		}
	}
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
		UML::Element *e = dynamic_cast<UML::Element *>(item);
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
}

qReal::MainWindow *EditorViewScene::mainWindow() const
{
	return mWindow;
}

void EditorViewScene::connectActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	QList<QVariant> connection = action->data().toList();
	Id source = connection[0].value<Id>();
	Id destination = connection[1].value<Id>();
	if (!action->text().startsWith("New ")) {
		model()->assistApi().connect(source, destination);
	} else {
		model()->assistApi().createConnected(source, destination);
	}
}

void EditorViewScene::addUsageActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	QList<QVariant> connection = action->data().toList();
	Id source = connection[0].value<Id>();
	Id destination = connection[1].value<Id>();
	if (!action->text().startsWith("New ")) {
		model()->assistApi().addUsage(source, destination);
	} else {
		model()->assistApi().createUsed(source, destination);
	}
}

void EditorViewScene::goToActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	Id target = action->data().value<Id>();
	mainWindow()->activateItemOrDiagram(target);
	return;
}

void EditorViewScene::disconnectActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	QList<QVariant> connection = action->data().toList();
	Id source = connection[0].value<Id>();
	Id destination = connection[1].value<Id>();
	model()->assistApi().disconnect(source, destination);
}

void EditorViewScene::deleteUsageActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	QList<QVariant> connection = action->data().toList();
	Id source = connection[0].value<Id>();
	Id destination = connection[1].value<Id>();
	model()->assistApi().deleteUsage(source, destination);
}

qReal::model::Model *EditorViewScene::model() const
{
	return dynamic_cast<qReal::model::Model *>(mv_iface->model());
}

void EditorViewScene::sceneRectChangedHandler(const QRectF & rect)
{
	mDraw = true;
}
