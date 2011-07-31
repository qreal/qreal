#include <QtGui>
#include <QtCore/QDebug>

#include "editorViewMVIface.h"
#include "editorView.h"
#include "editorViewScene.h"
#include "../kernel/definitions.h"
#include "../umllib/element.h"
#include "../editorManager/editorManager.h"
#include "../mainwindow/mainWindow.h"

using namespace qReal;

EditorViewMViface::EditorViewMViface(EditorView *view, EditorViewScene *scene)
	: QAbstractItemView(0)
	, mScene(scene)
	, mView(view)
	, mGraphicalAssistApi(NULL)
	, mLogicalAssistApi(NULL)
{
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

bool EditorViewMViface::isDescendentOf(const QModelIndex &descendent, const QModelIndex &ancestor)
{
	QModelIndex prev;
	QModelIndex curr = descendent;
	do {
		if (curr.parent() == ancestor)
			return true;
		prev = curr;
		curr = curr.parent();
	} while (curr != prev);
	return false;
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
	if (index == rootIndex())
		return;
	QAbstractItemView::setRootIndex(index);
	reset();
}

Id EditorViewMViface::rootId()
{
	return mGraphicalAssistApi->idByIndex(rootIndex());
}

void EditorViewMViface::rowsInserted(QModelIndex const &parent, int start, int end)
{
	for (int row = start; row <= end; ++row) {
		mScene->setEnabled(true);

		QPersistentModelIndex current = model()->index(row, 0, parent);
		if (!isDescendentOf(current, rootIndex()))
			continue;
		Id currentId = current.data(roles::idRole).value<Id>();
		if (currentId == Id::rootId())
			continue;
		Id parentUuid;
		if (parent != rootIndex())
			parentUuid = parent.data(roles::idRole).value<Id>();
		if (!parent.isValid()) {
			setRootIndex(current);
			continue;
		}

		Element* elem = mScene->mainWindow()->manager()->graphicalObject(currentId);
		elem->setAssistApi(mGraphicalAssistApi, mLogicalAssistApi);

		QPointF ePos = model()->data(current, roles::positionRole).toPointF();
		bool needToProcessChildren = true;
		if (elem) {
			elem->setPos(ePos);	//задаем позицию до определения родителя для того, чтобы правильно отработал itemChange
			elem->setId(currentId);

			NodeElement* nodeElement = dynamic_cast<NodeElement*>(elem);
			if (nodeElement)
				nodeElement->storeGeometry();

			if (item(parent) != NULL)
				elem->setParentItem(item(parent));
				QModelIndex next = current.sibling(current.row() + 1, 0);
				if(next.isValid()){
//					beforeId = next.data(roles::logicalIdRole).value<Id>();
					elem->stackBefore(item(next));
				}
			else {
				mScene->addItem(elem);
			}
			setItem(current, elem);
			elem->updateData();
			elem->connectToPort();
			elem->checkConnectionsToPort();
			elem->initPossibleEdges();
			elem->initTitles();
//			elem->initEmbeddedControls();
			//todo: нужно привести в порядок всякие init~()

			bool isEdgeFromEmbeddedLinker = false;
			QList<QGraphicsItem*> selectedItems = mScene->selectedItems();
			if (selectedItems.size() == 1) {
				NodeElement* master = dynamic_cast<NodeElement*>(selectedItems.at(0));
				if (master && master->connectionInProgress())
					isEdgeFromEmbeddedLinker = true;
			}

			if (!isEdgeFromEmbeddedLinker)
				mScene->clearSelection();
			elem->setSelected(true);

			NodeElement* nodeElem = dynamic_cast<NodeElement*>(elem);
			if (nodeElem && currentId.element() == "Class" &&
				mGraphicalAssistApi->children(currentId).empty())
			{
				needToProcessChildren = false;
				for (int i = 0; i < 2; i++) {
					QString curChildElementType;
					if (i == 0)
						curChildElementType = "MethodsContainer";
					else
						curChildElementType = "FieldsContainer";
					Id newUuid = Id("Kernel_metamodel", "Kernel",
							curChildElementType, QUuid::createUuid().toString());

					mGraphicalAssistApi->createElement(currentId, newUuid, false,  "(anonymous something)", QPointF(0, 0));
				}
			}
		}
		if (needToProcessChildren && model()->hasChildren(current))
			rowsInserted(current, 0, model()->rowCount(current) - 1);
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

	Element* movedElement = item(movedElementIndex);

	if (!item(destinationParent)) {
		// no parent element on the scene, so it should be root element
		movedElement->setParentItem(NULL);
		return;
	}

	Element* newParent = item(destinationParent);
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

	Element* movedElement = item(movedElementIndex);
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

models::GraphicalModelAssistApi *EditorViewMViface::graphicalAssistApi() const
{
	return mGraphicalAssistApi;
}

models::LogicalModelAssistApi *EditorViewMViface::logicalAssistApi() const
{
	return mLogicalAssistApi;
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

Element *EditorViewMViface::item(QPersistentModelIndex const &index) const
{
	foreach (IndexElementPair pair, mItems) {
		if (pair.first == index)
			return pair.second;
	}
	return NULL;
}

void EditorViewMViface::setItem(QPersistentModelIndex const &index, Element *item)
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

void EditorViewMViface::setAssistApi(models::GraphicalModelAssistApi &graphicalAssistApi, models::LogicalModelAssistApi &logicalAssistApi)
{
	mGraphicalAssistApi = &graphicalAssistApi;
	mLogicalAssistApi = &logicalAssistApi;
}

void EditorViewMViface::setLogicalModel(QAbstractItemModel * const logicalModel)
{
	connect(logicalModel, SIGNAL(dataChanged(QModelIndex, QModelIndex))
			, this, SLOT(logicalDataChanged(QModelIndex, QModelIndex)));
}

void EditorViewMViface::logicalDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
	for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
		QModelIndex const curr = topLeft.sibling(row, 0);
		Id const logicalId = curr.data(roles::idRole).value<Id>();
		IdList const graphicalIds = mGraphicalAssistApi->graphicalIdsByLogicalId(logicalId);
		foreach (Id const graphicalId, graphicalIds) {
			QModelIndex const graphicalIndex = mGraphicalAssistApi->indexById(graphicalId);
			Element *graphicalItem = item(graphicalIndex);
			if (graphicalItem)
				graphicalItem->updateData();
		}
	}
}
