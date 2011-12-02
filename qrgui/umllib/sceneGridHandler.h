#pragma once

#include <QtCore/QList>

#include <QtGui/QGraphicsLineItem>
#include <QtGui/QPen>

class NodeElement;

class SceneGridHandler
{
public:
	explicit SceneGridHandler(NodeElement *mNode);

	void delUnusedLines();

	// aligning the coordinates of a point to the grid
	qreal makeGridAlignmentX(qreal myX);
	qreal makeGridAlignmentY(qreal myY);

	void setGridMode(bool mode);
	void setAlignmentMode(bool mode);
	void setShowAlignmentMode(bool mode);

	void mouseMoveEvent();

private:
	/** @brief drawing a horizontal line */
	void drawLineY(qreal pointY, qreal myX);
	/** @brief drawing a vertical line */
	void drawLineX(qreal pointX, qreal myY);

	bool makeJumpX(qreal deltaX, qreal pointX);
	bool makeJumpY(qreal deltaY, qreal pointY);

	void buildLineX(qreal deltaX, qreal pointX, qreal correctionX
			, qreal &myX1, qreal &myX2, qreal myY);
	void buildLineY(qreal deltaY, qreal pointY, qreal correctionY
			, qreal &myY1, qreal &myY2, qreal myX);

	void makeGridMovingX(qreal myX, int koef, int indexGrid);
	void makeGridMovingY(qreal myY, int koef, int indexGrid);

	qreal recalculateX1();
	qreal recalculateX2(qreal myX1);
	qreal recalculateY1();
	qreal recalculateY2(qreal myY1);

	QList<QGraphicsLineItem*> mLines;
	NodeElement *mNode;

	bool mShowAlignment;
	/** @brief if true, the object will be aligned to indexGrid */
	bool mSwitchGrid;
	bool mSwitchAlignment;

	QPen mGuidesPen;

	/** @brief radius of guides appearing */
	static qreal const radius = 20;
	/** @brief minimum distance to approach to jump to the guide */
	static qreal const radiusJump = 10;
	/** @brief spacing between element and guide */
	static qreal const spacing = 0;
	/** @brief guides that have distance equal or less will be represented by only one of them */
	static qreal const indistinguishabilitySpace = 10;
};
