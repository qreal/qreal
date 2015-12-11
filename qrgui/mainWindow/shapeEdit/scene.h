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

#include <tr1/functional>

#include <qrutils/graphicsUtils/abstractScene.h>
#include <qrutils/graphicsUtils/gridDrawer.h>
#include <qrkernel/settingsManager.h>
#include <qrgui/controller/controller.h>

#include "mainWindow/shapeEdit/item/item.h"

#include "mainWindow/shapeEdit/item/text.h"
#include "mainWindow/shapeEdit/item/textPicture.h"
#include "mainWindow/shapeEdit/item/pointPort.h"
#include "mainWindow/shapeEdit/item/linePort.h"
#include "mainWindow/shapeEdit/item/image.h"

namespace qReal {
namespace shapeEdit {

class ResizeItemCommand;
class MoveItemCommand;

const int sizeEmptyRectX = 680;
const int sizeEmptyRectY = 580;

class Scene : public graphicsUtils::AbstractScene
{
	Q_OBJECT

public:
    Scene(graphicsUtils::AbstractView *view, Controller *controller, QObject *parent = 0);
	QPoint centerEmpty();
	void changeTextName(const QString &name);
	void setZValue(Item* item);

    // methods for Item
    void setPenBrushForItem(Item *item);
    void removeMoveFlagForItem(QGraphicsSceneMouseEvent *event, Item *item);
    void setIsAddingFinished(bool isFinished);
    void setWaitMove(bool isWait);
    bool getWaitMove();
    QString getPortType();
    QString getFileName();

    QList<Item *> selectedShapeEditItems();

public slots:
    void redraw();
    void setNeedDrawGrid(bool show);
    void addShapeEditItem(bool checked, Item* item);
    void addImage(const QString &fileName);
    void addNone();

signals:
	void noSelectedItems();
	void existSelectedItems(const QPen &penItem, const QBrush &brushItem);
    void noSelectedTextPictureItems();
    void existSelectedTextPictureItems(const QPen &penItem, const QFont &fontItem, const QString &name);
    void noSelectedPortItems();
    void existSelectedPortItems(const QString &type);
	void resetHighlightAllButtons();

protected:
    virtual void drawBackground(QPainter *painter, const QRectF &rect);

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

    void deleteItems();
    void clearScene();
    void resetItemCreating();

private:
	enum CopyPasteType {
		nonePaste
		, copy
		, cut
	};

    Controller *mController;

    bool mNeedDrawGrid; // if true, the grid will be shown (as scene's background)
    graphicsUtils::GridDrawer mGridDrawer;

	int mZValue;
    Item *mNewItem;
    Item *mChangingItem;
	bool mWaitMove;
    bool mIsAddingFinished;
    ResizeItemCommand *mResizeCommand;
    MoveItemCommand *mMoveCommand;

    QString mPortType;
    QString mFileName;
	CopyPasteType mCopyPaste;
	QList<Item *> mListSelectedItemsForPaste;

    void initContextMenuActions();

	void initPasteItemsBuffer();
	QRectF selectedItemsBoundingRect() const;
    QList<TextPicture *> selectedTextPictureItems();

	void setZValueSelectedItems();
	void setNullZValueItems();
	QPair<bool, Item *> checkOnResize(qreal x, qreal y);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    virtual void keyPressEvent(QKeyEvent *keyEvent);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

}
}
