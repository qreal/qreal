#pragma once

#include <QtCore/QList>

#include <QtGui/QGraphicsLineItem>
#include <QtGui/QPen>

class NodeElement;

class SceneGridHandler
{
public:
	explicit SceneGridHandler(NodeElement *mNode);
	~SceneGridHandler();

	void delUnusedLines();

	void setGridMode(bool mode);
	void setAlignmentMode(bool mode);
	void setShowAlignmentMode(bool mode);

	/// Align node to grid
	void alignToGrid();
	/// Draw alignment lines
	void drawGuides();

	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:

	/// returns list of elements lying in the same layer
	QList<QGraphicsItem *> getAdjancedNodes() const;

	/** @brief drawing a horizontal line */
	void drawLineY(qreal pointY);
	/** @brief drawing a vertical line */
	void drawLineX(qreal pointX);

	bool makeJumpX(qreal deltaX, qreal pointX);
	bool makeJumpY(qreal deltaY, qreal pointY);

	void buildLineX(qreal deltaX, qreal pointX, qreal correctionX
			, qreal &myX1, qreal &myX2);
	void buildLineY(qreal deltaY, qreal pointY, qreal correctionY
			, qreal &myY1, qreal &myY2);

	void makeGridMovingX(qreal myX, int koef, int indexGrid);
	void makeGridMovingY(qreal myY, int koef, int indexGrid);

	qreal recalculateX1() const;
	qreal recalculateX2(qreal myX1) const;
	qreal recalculateY1() const;
	qreal recalculateY2(qreal myY1) const;

	NodeElement *mNode;
	/// Lines of guides
	QVector<QLineF> mLines;
	/// Pixmap of guides lines
	QPixmap *mGuidesPixmap;

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
