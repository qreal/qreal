#include <QtGui>

#include "editorviewmviface.h"
#include "editorview.h"
#include "editorviewscene.h"
#include "../kernel/definitions.h"
#include "../umllib/uml_element.h"
#include "../editorManager/editorManager.h"
#include "../mainwindow/mainwindow.h"

using namespace qReal;

EditorViewMViface::EditorViewMViface(EditorView *view, EditorViewScene *scene)
	: QAbstractItemView(0)
{
	mView = view;
	mScene = scene;

	mScene->mv_iface = this;
	mScene->view = mView;
}

EditorViewMViface::~EditorViewMViface()
{
	clearItems();
}

QRect EditorViewMViface::visualRect(const QModelIndex &) const
{
	return QRect();
}

void EditorViewMViface::scrollTo(const QModelIndex &, ScrollHint)
{
}

QModelIndex EditorViewMViface::indexAt(const QPoint &) const
{
	return QModelIndex();
}

QModelIndex EditorViewMViface::moveCursor(QAbstractItemView::CursorAction,
		Qt::KeyboardModifiers)
{
	return QModelIndex();
}

int EditorViewMViface::horizontalOffset() const
{
	return 0;
}

int EditorViewMViface::verticalOffset() const
{
	return 0;
}

bool EditorViewMViface::isIndexHidden(const QModelIndex &) const
{
	return false;
}

void EditorViewMViface::setSelection(const QRect&, QItemSelectionModel::SelectionFlags )
{
}

QRegion EditorViewMViface::visualRegionForSelection(const QItemSelection &) const
{
	return QRegion();
}

void EditorViewMViface::reset()
{
	mScene->clearScene();
	clearItems();

	if (model() && model()->rowCount(QModelIndex()) == 0)
		mScene->setEnabled(false);

	// so that our diagram be nicer
	QGraphicsRectItem *rect = mScene->addRect(QRect(-1000, -1000, 2000, 2000));
	mScene->removeItem(rect);
	delete rect;

	if (model())
		rowsInserted(rootIndex(), 0, model()->rowCount(rootIndex()) - 1);
}

void EditorViewMViface::setRootIndex(const QModelIndex &index)
{
	QAbstractItemView::setRootIndex(index);
	reset();
}

void EditorViewMViface::rowsInserted(QModelIndex const &parent, int start, int end)
{
	for (int row = start; row <= end; ++row) {
		QPersistentModelIndex current = model()->index(row, 0, parent);
		Id uuid = current.data(roles::idRole).value<Id>();

		Id parent_uuid;
		if (parent != rootIndex())
			parent_uuid = parent.data(roles::idRole).value<Id>();

		mScene->setEnabled(true);

		// if we add a diagram as root element
		if (!parent.isValid()) {
			setRootIndex(current);
			continue;
		}

		if (uuid == ROOT_ID)
			continue;

		UML::Element* e = mScene->mainWindow()->manager()->graphicalObject(uuid);
		QPointF ePos = model()->data(current, roles::positionRole).toPointF();
		bool needToProcessChildren = true;
		if (e) {
			e->setPos(ePos);
			//задаем позицию до определения родителя для того, чтобы правильно отработал itemChange

			e->setIndex(current);
			if (parent_uuid != Id() && item(parent) != NULL)
				e->setParentItem(item(parent));
			else
				mScene->addItem(e);
			setItem(current, e);
			e->updateData();
			e->connectToPort();
			e->initPossibleEdges();

			UML::NodeElement* nodeE = dynamic_cast<UML::NodeElement*>(e);
		
			qDebug() << "Uuid ___" << uuid.toString();

			if (nodeE && uuid.element() == "Class") {
				needToProcessChildren = false;
				
				for (int i = 0; i < 2; i++) {
					QString curChildElementType;
					if (i == 0)
						curChildElementType = "MethodsContainer";
					else
						curChildElementType = "FieldsContainer";

					Id newUuid = Id("Kernel_metamodel", "Kernel", 
							curChildElementType, QUuid::createUuid().toString());

					QByteArray data;
					QMimeData *mimeData = new QMimeData();
					QDataStream stream(&data, QIODevice::WriteOnly);
					QString mimeType = QString("application/x-real-uml-data");
					QString newElemUuid = newUuid.toString(); //TODO: normal string
					QString pathToItem = ROOT_ID.toString();
					QString name = "(anonymous something)";
					QPointF pos = QPointF(0, 0);
					stream << newElemUuid;
					stream << pathToItem;
					stream << name;
					stream << pos;

					mimeData->setData(mimeType, data);

					model()->dropMimeData(mimeData, Qt::CopyAction, model()->rowCount(current), 0, current);

					delete mimeData;
				}
			}
		}

		qDebug() << "Before model children process" << uuid.element();

		if (needToProcessChildren && model()->hasChildren(current)) {
			rowsInserted(current, 0, model()->rowCount(current) - 1);
		}
		
		qDebug() << "After model children process" << uuid.element();
	}
	QAbstractItemView::rowsInserted(parent, start, end);
}

void EditorViewMViface::rowsAboutToBeRemoved(QModelIndex  const &parent, int start, int end)
{
	for (int row = start; row <= end; ++row) {
		QModelIndex curr = model()->index(row, 0, parent);
		if( item(curr) ){
			mScene->removeItem(item(curr));
			delete item(curr);
		}
		removeItem(curr);
	}

	// elements from model are deleted after GUI ones
	if (parent == QModelIndex() && model()->rowCount(parent) == start - end + 1)
		mScene->setEnabled(false);

	QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}

void EditorViewMViface::rowsAboutToBeMoved(QModelIndex const &sourceParent, int sourceStart, int sourceEnd, QModelIndex const &destinationParent, int destinationRow)
{
	Q_UNUSED(sourceEnd);
	Q_UNUSED(destinationRow);
	Q_ASSERT(sourceStart == sourceEnd);  // only one element is permitted to be moved
	QPersistentModelIndex movedElementIndex = sourceParent.child(sourceStart, 0);

	if (!item(movedElementIndex)) {
		// there's no such element on the scene already
		// TODO: add element on the scene if there's no such element here, but there's in the model
		// ignoring there cases now
		qDebug() << "Trying to move element that already does not exist on a current scene, that's strange";
		return;
	}

	UML::Element* movedElement = item(movedElementIndex);

	if (!item(destinationParent)) {
		// no parent element on the scene, so it should be root element
		movedElement->setParentItem(NULL);
		return;
	}

	UML::Element* newParent = item(destinationParent);
	movedElement->setParentItem(newParent);
}

void EditorViewMViface::rowsMoved(QModelIndex const &sourceParent, int sourceStart, int sourceEnd, QModelIndex const &destinationParent, int destinationRow)
{
	Q_UNUSED(sourceParent);
	Q_UNUSED(sourceStart);
	Q_UNUSED(sourceEnd);
	Q_ASSERT(sourceStart == sourceEnd);
	QPersistentModelIndex movedElementIndex = destinationParent.child(destinationRow, 0);

	Q_ASSERT(movedElementIndex.isValid());
	if (!item(movedElementIndex)) {
		// no element on the scene, forget about it
		return;
	}

	UML::Element* movedElement = item(movedElementIndex);
	movedElement->updateData();
}

void EditorViewMViface::dataChanged(const QModelIndex &topLeft,
	const QModelIndex &bottomRight)
{
	for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
		QModelIndex curr = topLeft.sibling(row, 0);
		if (item(curr))
			item(curr)->updateData();
	}
}

EditorViewScene *EditorViewMViface::scene() const
{
	return mScene;
}

void EditorViewMViface::clearItems()
{

	QList<QGraphicsItem *> toRemove;
	foreach (IndexElementPair pair, mItems)
		if (!pair.second->parentItem())
			toRemove.append(pair.second);
	foreach (QGraphicsItem *item, toRemove)
		delete item;
	mItems.clear();
}

UML::Element *EditorViewMViface::item(QPersistentModelIndex const &index) const
{
	foreach (IndexElementPair pair, mItems) {
		if (pair.first == index)
			return pair.second;
	}
	return NULL;
}

void EditorViewMViface::setItem(QPersistentModelIndex const &index, UML::Element *item)
{
	IndexElementPair pair(index, item);
	if (!mItems.contains(pair))
		mItems.insert(pair);
}

void EditorViewMViface::removeItem(QPersistentModelIndex const &index)
{
	foreach (IndexElementPair pair, mItems) {
		if (pair.first == index)
			mItems.remove(pair);
	}
}
