/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "editorViewMVIface.h"

#include <qrkernel/definitions.h>
#include <metaMetaModel/elementType.h>

#include "editor/editorView.h"
#include "editor/editorViewScene.h"
#include "editor/element.h"
#include "plugins/pluginManager/editorManagerInterface.h"

using namespace qReal;
using namespace qReal::gui::editor;

EditorViewMViface::EditorViewMViface(EditorView *view, EditorViewScene *scene)
	: QAbstractItemView(0)
	, mScene(scene)
	, mView(view)
	, mGraphicalAssistApi(nullptr)
	, mLogicalAssistApi(nullptr)
	, mExploser(nullptr)
{
	connect(this, &EditorViewMViface::rootElementRemoved, mView, &EditorView::rootElementRemoved);
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
		if (curr.parent() == ancestor) {
			return true;
		}
		prev = curr;
		curr = curr.parent();
	} while (curr != prev);
	return false;
}

QModelIndex EditorViewMViface::moveCursor(QAbstractItemView::CursorAction
		, Qt::KeyboardModifiers)
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

	if (model() && model()->rowCount(QModelIndex()) == 0) {
		mScene->setEnabled(false);
	}

	// Our diagram would be nicer
	QGraphicsRectItem *rect = mScene->addRect(QRect(-1000, -1000, 2000, 2000));
	mScene->removeItem(rect);
	delete rect;

	if (model()) {
		if (rootIndex().isValid()) {
			rowsInserted(rootIndex(), 0, model()->rowCount(rootIndex()) - 1);
		} else {
			mScene->setEnabled(true);
		}
	}
}

void EditorViewMViface::setRootIndex(const QModelIndex &index)
{
	if (index == rootIndex()) {
		return;
	}
	QAbstractItemView::setRootIndex(index);
	reset();
}

Id EditorViewMViface::rootId() const
{
	return mGraphicalAssistApi ? mGraphicalAssistApi->idByIndex(rootIndex()) : Id();
}

void EditorViewMViface::rowsInserted(const QModelIndex &parent, int start, int end)
{
	mScene->setEnabled(true);
	for (int row = start; row <= end; ++row) {

		QPersistentModelIndex current = model()->index(row, 0, parent);
		if (!isDescendentOf(current, rootIndex())) {
			continue;
		}

		Id currentId = current.data(roles::idRole).value<Id>();
		if (currentId == Id::rootId()) {
			continue;
		}

		Id parentUuid;
		if (parent != rootIndex()) {
			parentUuid = parent.data(roles::idRole).value<Id>();
		}

		if (!parent.isValid()) {
			setRootIndex(current);
			continue;
		}

		const ElementType &elementType = mLogicalAssistApi->editorManagerInterface().elementType(currentId);
		Element *elem = elementType.isNode()
				? static_cast<Element *>(new NodeElement(elementType.toNode(), currentId, mScene->models()))
				: static_cast<Element *>(new EdgeElement(elementType.toEdge(), currentId, mScene->models()));

		elem->setController(&mScene->controller());

		QPointF ePos = model()->data(current, roles::positionRole).toPointF();
		bool needToProcessChildren = true;

		if (elem) {
			// setting position before parent definition 'itemChange' to work correctly
			elem->setPos(ePos);

			if (NodeElement * const node = dynamic_cast<NodeElement *>(elem)) {
				node->setGeometry(mGraphicalAssistApi->configuration(elem->id()).boundingRect()
						.translated(ePos.toPoint()));
			}

			if (item(parent)) {
				elem->setParentItem(item(parent));
				QModelIndex next = current.sibling(current.row() + 1, 0);
				if(next.isValid() && item(next) != nullptr) {
					elem->stackBefore(item(next));
				}
			} else {
				mScene->addItem(elem);
			}

			setItem(current, elem);
			elem->updateData();
			elem->connectToPort();
			elem->checkConnectionsToPort();
			elem->initPossibleEdges();
			elem->initTitles();
			mView->setFocus();
			// TODO: brush up init~()

			if (dynamic_cast<NodeElement *>(elem) && currentId.element() == "Class" &&
					mGraphicalAssistApi->children(currentId).empty())
			{
				needToProcessChildren = false;
				for (int i = 0; i < 2; i++) {
					QString curChildElementType = (i == 0) ? "MethodsContainer" : "FieldsContainer";
					Id newUuid = Id("Kernel_metamodel", "Kernel", curChildElementType, QUuid::createUuid().toString());
					mGraphicalAssistApi->createElement(currentId, newUuid
							, false,  "(anonymous something)", QPointF(0, 0));
				}
			}

			if (EdgeElement * const edgeElem = dynamic_cast<EdgeElement *>(elem)) {
				edgeElem->layOut();
			}
		}

		if (needToProcessChildren && model()->hasChildren(current)) {
			rowsInserted(current, 0, model()->rowCount(current) - 1);
		}

		NodeElement * nodeElement = dynamic_cast<NodeElement*>(elem);
		if (nodeElement) {
			nodeElement->alignToGrid();
		}
	}

	QAbstractItemView::rowsInserted(parent, start, end);
}

void EditorViewMViface::rowsAboutToBeRemoved(QModelIndex  const &parent, int start, int end)
{
	for (int row = start; row <= end; ++row) {
		QModelIndex curr = model()->index(row, 0, parent);
		if (curr == rootIndex()) {
			// Root id was removed, time to close current tab.
			emit rootElementRemoved(curr);
			// Now we will be deleted, nipping off...
			return;
		}

		if (Element *element = item(curr)) {
			mScene->onElementDeleted(element);
			mScene->removeItem(element);
			delete element;
		}

		removeItem(curr);
	}

	// elements from model are deleted after GUI ones
	if (parent == QModelIndex() && model()->rowCount(parent) == start - end + 1) {
		mScene->setEnabled(false);
	}

	QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}

void EditorViewMViface::rowsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd
		, const QModelIndex &destinationParent, int destinationRow)
{
	Q_UNUSED(sourceEnd);
	Q_ASSERT(sourceStart == sourceEnd);  // only one element is permitted to be moved
	QPersistentModelIndex movedElementIndex = sourceParent.child(sourceStart, 0);
	QPersistentModelIndex newSiblingIndex = destinationParent.child(destinationRow, 0);

	Element *movedElement = item(movedElementIndex);
	Element *sibling = item(newSiblingIndex);
	if (!movedElement) {
		// there's no such element on the scene already
		// TODO: add element on the scene if there's no such element here, but there's in the model
		// ignoring there cases now
		qDebug() << "Trying to move element that already does not exist on a current scene, that's strange";
		return;
	}

	Element *newParent = item(destinationParent);
	if (newParent == nullptr) {
		// no parent element on the scene, so it should be root element
		movedElement->setParentItem(nullptr);
		return;
	}

	movedElement->setParentItem(newParent);
	if (sibling != nullptr) {
		movedElement->stackBefore(sibling);
	}
}

void EditorViewMViface::rowsMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd
		, const QModelIndex &destinationParent, int destinationRow)
{
	Q_UNUSED(sourceParent);
	Q_UNUSED(sourceStart);
	Q_UNUSED(sourceEnd);
	Q_ASSERT(sourceStart == sourceEnd);
	QPersistentModelIndex movedElementIndex = destinationParent.child(destinationRow, 0);

	Q_ASSERT(movedElementIndex.isValid());
	Element *movedElement = item(movedElementIndex);

	if (movedElement == nullptr) {
		// no element on the scene, forget about it
		return;
	}

	movedElement->updateData();
}

void EditorViewMViface::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight
		, const QVector<int> &roles)
{
	Q_UNUSED(roles)
	for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
		const QModelIndex curr = topLeft.sibling(row, 0);
		Element *element = item(curr);
		if (element) {
			element->updateData();
		}
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
	mItems.clear();
}

Element *EditorViewMViface::item(const QPersistentModelIndex &index) const
{
	for (const IndexElementPair &pair : mItems) {
		if (pair.first == index) {
			return pair.second;
		}
	}

	return nullptr;
}

void EditorViewMViface::setItem(const QPersistentModelIndex &index, Element *item)
{
	IndexElementPair pair(index, item);
	if (!mItems.contains(pair)) {
		mItems.insert(pair);
	}
}

void EditorViewMViface::removeItem(const QPersistentModelIndex &index)
{
	QList<IndexElementPair> itemsForRemoving;
	for (const IndexElementPair &pair : mItems) {
		if (pair.first == index) {
			itemsForRemoving.append(pair);
		}
	}

	for (const auto &element : itemsForRemoving) {
		mItems.remove(element);
	}
}

void EditorViewMViface::configure(models::GraphicalModelAssistApi &graphicalAssistApi
		, models::LogicalModelAssistApi &logicalAssistApi
		, models::Exploser &exploser)
{
	mGraphicalAssistApi = &graphicalAssistApi;
	mLogicalAssistApi = &logicalAssistApi;
	mExploser = &exploser;
}

/// @todo: set logical model in constructor
void EditorViewMViface::setLogicalModel(QAbstractItemModel * const logicalModel)
{
	connect(logicalModel, SIGNAL(dataChanged(QModelIndex, QModelIndex))
			, this, SLOT(logicalDataChanged(QModelIndex, QModelIndex)));
}

void EditorViewMViface::logicalDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
	for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
		const QModelIndex curr = topLeft.sibling(row, 0);
		const Id logicalId = curr.data(roles::idRole).value<Id>();
		const IdList graphicalIds = mGraphicalAssistApi->graphicalIdsByLogicalId(logicalId);
		for (const Id &graphicalId : graphicalIds) {
			const QModelIndex graphicalIndex = mGraphicalAssistApi->indexById(graphicalId);
			Element *graphicalItem = item(graphicalIndex);
			if (graphicalItem) {
				graphicalItem->updateData();
			}
		}
	}
}
