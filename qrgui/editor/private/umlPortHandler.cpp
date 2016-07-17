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

#include "umlPortHandler.h"

#include "editor/nodeElement.h"
#include "editor/private/borderChecker.h"

using namespace qReal::gui::editor;

UmlPortHandler::UmlPortHandler(NodeElement * const node)
		: mNode(node), mBelongsToHorizontalBorders(true)
{
}

void UmlPortHandler::handleMoveEvent(const bool leftPressed
		, QPointF &pos, const QPointF &scenePos
		, NodeElement *&parentNode)
{
	if (!mNode || !leftPressed) {
		return;
	}

	if (pos == QPointF(0,0)) {
		pos = mNode->pos();
	}

	QGraphicsItem* const item = mNode->scene()->items(scenePos).value(1);
	NodeElement* const actionItem = dynamic_cast<NodeElement * const>(item);
	const BorderChecker actionItemBorderChecker(actionItem);
	QPointF posInItem = QPointF(0, 0);
	if (actionItem && ((actionItem == parentNode) || (!parentNode))) {
		posInItem = actionItem->mapFromScene(scenePos);

		const bool checkLowerBorder = actionItemBorderChecker.checkLowerBorder(posInItem);
		const bool checkUpperBorder = actionItemBorderChecker.checkUpperBorder(posInItem);
		const bool checkRightBorder = actionItemBorderChecker.checkRightBorder(posInItem);
		const bool checkLeftBorder = actionItemBorderChecker.checkLeftBorder(posInItem);
		const bool checkBorders = checkLowerBorder || checkUpperBorder
			|| checkRightBorder || checkLeftBorder;

		if (checkBorders) {
			mNode->setParentItem(actionItem);
			parentNode = actionItem;
			pos = mNode->pos();
			mBelongsToHorizontalBorders = checkLowerBorder || checkUpperBorder;
		} else {
			handleHorizontalBorders(actionItem, parentNode, pos, posInItem);
		}
	} else if (parentNode) {
			mNode->setPos(pos);
			if (parentNode) {
				posInItem = parentNode->mapFromScene(scenePos);
				handleHorizontalBorders(parentNode, parentNode, pos, posInItem);
			}
	}
}

void UmlPortHandler::handleHorizontalBorders(
		const NodeElement * const tmpNode, const NodeElement * const parentNode
		, const QPointF &pos, const QPointF &posInItem) const
{
	QPointF newPos = pos;
	const BorderChecker parentNodeBorderChecker(parentNode);
	if (mBelongsToHorizontalBorders) {
		// tmpNode->borderValues()[0] == xHor of tmpNode. (mXHor field of BorderChecker(tmpNode))
		if (parentNodeBorderChecker.checkNoBorderY(posInItem, tmpNode->borderValues()[0])) {
			newPos.setX(posInItem.x());
		}
	} else {
		// tmpNode->borderValues()[3] == yVert of tmpNode. (mYVert field of BorderChecker(tmpNode))
		if (parentNodeBorderChecker.checkNoBorderX(posInItem, tmpNode->borderValues()[3])) {
			newPos.setY(posInItem.y());
		}
	}
	mNode->setPos(newPos);
}
