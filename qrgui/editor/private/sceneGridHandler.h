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

#pragma once

#include <QtCore/QList>
#include <QtWidgets/QGraphicsLineItem>
#include <QtGui/QPen>

namespace qReal {
namespace gui {
namespace editor {

class NodeElement;

class SceneGridHandler
{
public:
	explicit SceneGridHandler(NodeElement *mNode);
	~SceneGridHandler();

	void deleteGuides();

	/// aligning the coordinates of a point to grid
	qreal makeGridAlignment(qreal coord);

	void setGridMode(bool mode);
	void setAlignmentMode(bool mode);
	void setShowAlignmentMode(bool mode);

	/// Align node to grid
	void alignToGrid();
	/// Draw alignment lines
	void drawGuides();

	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

	/// return coordinate aligned to grid
	static qreal alignedCoordinate(qreal coord, int coef, int indexGrid);

private:

	/// returns list of elements lying in the same layer
	QList<QGraphicsItem *> getAdjancedNodes() const;

	/** @brief drawing a horizontal line */
	void drawLineY(qreal pointY, const QRectF &sceneRect);
	/** @brief drawing a vertical line */
	void drawLineX(qreal pointX, const QRectF &sceneRect);

	bool makeJumpX(qreal deltaX, qreal pointX);
	bool makeJumpY(qreal deltaY, qreal pointY);

	void buildLineX(qreal deltaX, qreal pointX, qreal correctionX
			, qreal &myX1, qreal &myX2, const QRectF &sceneRect);
	void buildLineY(qreal deltaY, qreal pointY, qreal correctionY
			, qreal &myY1, qreal &myY2, const QRectF &sceneRect);

	/// move element vertically according to the grid; coef - index of cell in the grid
	void makeGridMovingX(qreal myX, int coef, int indexGrid);

	/// move element horizontally according to the grid
	void makeGridMovingY(qreal myY, int coef, int indexGrid);

	qreal recalculateX1() const;
	qreal recalculateX2(qreal myX1) const;
	qreal recalculateY1() const;
	qreal recalculateY2(qreal myY1) const;

	NodeElement *mNode;
	/// Lines of guides
	QList<QGraphicsLineItem *> mGuides;

	bool mShowAlignment;
	/** @brief if true, the object will be aligned to indexGrid */
	bool mSwitchGrid;
	bool mSwitchAlignment;

	QPen mGuidesPen;

	/** @brief radius of guides appearing */
	static const int radius = 20;
	/** @brief minimum distance to approach to jump to the guide */
	static const int radiusJump = 10;
	/** @brief spacing between element and guide */
	static const int spacing = 0;
	/** @brief guides that have distance equal or less will be represented by only one of them */
	static const int indistinguishabilitySpace = 10;
};

}
}
}
