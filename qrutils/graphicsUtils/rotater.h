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

#include <qrutils/graphicsUtils/abstractItem.h>
#include <qrutils/graphicsUtils/lineImpl.h>

namespace graphicsUtils {

class RotateItem;

const int length = 30;
const int addAngle = 30; // Degrees; for arrow pike drawing

/// Class that represents line that allows to rotate QGraphicItems
class QRUTILS_EXPORT Rotater : public AbstractItem
{
public:
	Rotater();

	QRectF boundingRect() const;

	void drawItem(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget* widget = 0) override;
	void drawExtractionForItem(QPainter* painter) override;
	void drawFieldForResizeItem(QPainter* painter) override;
	void setPenBrushForExtraction(QPainter* painter, const QStyleOptionGraphicsItem* option) override;
	void resizeItem(QGraphicsSceneMouseEvent *event) override;
	void calcResizeItem(QGraphicsSceneMouseEvent *event) override;

	void setMasterItem(RotateItem *masterItem);

private:
	void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

	int mLength;
	int mDrift;
	int mResizeDrift;
	graphicsUtils::RotateItem *mMaster;
	graphicsUtils::LineImpl mLineImpl;
};

}
