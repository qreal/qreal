#pragma once

#include <QtCore/QList>
#include <QtGui/QGraphicsLineItem>

namespace UML {
class NodeElement;

class SceneGridHandler
{
public:
	SceneGridHandler(NodeElement *mNode);

	void delUnusedLines();

	void setGridMode(bool mode);
	void setAlignmentMode(bool mode);
	void setShowAlignmentMode(bool mode);

	void mouseMoveEvent();

private:
	void drawLineY(qreal pointY, qreal myX);
	void drawLineX(qreal pointX, qreal myY);

	bool makeJumpX(qreal deltaX, qreal radiusJump, qreal pointX);
	bool makeJumpY(qreal deltaY, qreal radiusJump, qreal pointY);

	void buildLineX(qreal deltaX, qreal radius, bool doAlways, qreal radiusJump,
			qreal pointX, qreal correctionX	, qreal &myX1, qreal &myX2, qreal myY);
	void buildLineY(qreal deltaY, qreal radius, bool doAlways, qreal radiusJump,
			qreal pointY, qreal correctionY, qreal &myY1, qreal &myY2, qreal myX);

	void makeGridMovingX(qreal myX, int koef, int indexGrid);
	void makeGridMovingY(qreal myY, int koef, int indexGrid);

	qreal recalculateX1();
	qreal recalculateX2(qreal myX1);
	qreal recalculateY1();
	qreal recalculateY2(qreal myY1);

	QList<QGraphicsLineItem*> mLines;
	NodeElement *mNode;
	QGraphicsScene *mScene;

	bool mShowAlignment;
	bool mSwitchGrid;  //if true, the object will be aligned to indexGrid
	bool mSwitchAlignment;
};
}

