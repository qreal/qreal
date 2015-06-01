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

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsView>
#include <QtCore/QList>

#include <qrutils/graphicsUtils/abstractScene.h>
#include <qrkernel/settingsManager.h>

#include "mainWindow/shapeEdit/view.h"
#include "mainWindow/shapeEdit/item.h"
#include "mainWindow/shapeEdit/arch.h"
#include "mainWindow/shapeEdit/line.h"
#include "mainWindow/shapeEdit/ellipse.h"
#include "mainWindow/shapeEdit/rectangle.h"
#include "mainWindow/shapeEdit/text.h"
#include "mainWindow/shapeEdit/textPicture.h"
#include "mainWindow/shapeEdit/pointPort.h"
#include "mainWindow/shapeEdit/linePort.h"
#include "mainWindow/shapeEdit/stylus.h"
#include "mainWindow/shapeEdit/path.h"
#include "mainWindow/shapeEdit/curve.h"
#include "mainWindow/shapeEdit/image.h"

const int sizeEmptyRectX = 680;
const int sizeEmptyRectY = 580;

class Scene : public graphicsUtils::AbstractScene
{
	Q_OBJECT
public:
	Scene(graphicsUtils::AbstractView *view, QObject *parent = 0);
	QPoint centerEmpty();
	void changeTextName(const QString &name);
	void setZValue(Item* item);
	void addImage(const QString &fileName);

	void drawLine(bool checked);
	void drawEllipse(bool checked);
	void drawCurve(bool checked);
	void drawRectangle(bool checked);
	void addText(bool checked);
	void addDynamicText(bool checked);
	void addTextPicture(bool checked);
	void addPointPort(bool checked);
	void addLinePort(bool checked);
	void addStylus(bool checked);
	void addNone(bool checked);

	QList<Item *> selectedSceneItems();

signals:
	void noSelectedItems();
	void existSelectedItems(const QPen &penItem, const QBrush &brushItem);
	void noSelectedTextPictureItems();
	void existSelectedTextPictureItems(const QPen &penItem, const QFont &fontItem, const QString &name);
	void noSelectedPortItems();
	void existSelectedPortItems(const QString &type);
	void resetHighlightAllButtons();

private slots:
	void changePenStyle(const QString &text);
	void changePenWidth(int width);
	void changePenColor(const QString &text);
	void changeBrushStyle(const QString &text);
	void changeBrushColor(const QString &text);
	void changePortsType(const QString &type);

	void changePalette();
	void changeFontPalette();
	void changePortsComboBox();

	void changeFontFamily(const QFont& font);
	void changeFontPixelSize(int size);
	void changeFontColor(const QString &text);
	void changeFontItalic(bool isChecked);
	void changeFontBold(bool isChecked);
	void changeFontUnderline(bool isChecked);

	void deleteItem();
	void clearScene();

private:
	enum ItemTypes {
		none
		, line
		, ellipse
		, rectangle
		, text
		, dynamicText
		, textPicture
		, pointPort
		, linePort
		, stylus
		, curve
		, image
	};

	enum CopyPasteType {
		nonePaste
		, copy
		, cut
	};

	int mZValue;
	ItemTypes mItemType;
	bool mWaitMove;
	int mCount;
	Line *mLine;
	QRealEllipse *mEllipse;
	QRealRectangle *mRectangle;
	Text *mText;
	TextPicture *mTextPicture;
	PointPort *mPointPort;
	LinePort *mLinePort;
	Stylus *mStylus;
	Curve* mCurve;
	Image* mImage;
	QString mFileName;
	QPointF mC1;
	CopyPasteType mCopyPaste;
	QList<Item *> mListSelectedItemsForPaste;
	QList<QGraphicsItem *> mListSelectedItems;
	QList<TextPicture *> mListSelectedTextPictureItems;
	TextPicture *mSelectedTextPicture;
	QPair<bool, Item *> mNeedResize;
	QString mPortType;

	void initListSelectedItemsForPaste();
	QRectF selectedItemsBoundingRect() const;
	QList<TextPicture *> selectedTextPictureItems();
	QPointF setCXandCY(QGraphicsSceneMouseEvent *event);
	void reshapeLine(QGraphicsSceneMouseEvent *event);
	void reshapeLinePort(QGraphicsSceneMouseEvent *event);
	void reshapeEllipse(QGraphicsSceneMouseEvent *event);
	void reshapeRectangle(QGraphicsSceneMouseEvent *event);
	void reshapeStylus(QGraphicsSceneMouseEvent *event);
	void reshapeCurveFirst(QGraphicsSceneMouseEvent *event);
	void reshapeCurveSecond(QGraphicsSceneMouseEvent *event);

	void setZValueSelectedItems();
	void setNullZValueItems();
	QPair<bool, Item *> checkOnResize(qreal x, qreal y);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	virtual void keyPressEvent(QKeyEvent *keyEvent);
};
