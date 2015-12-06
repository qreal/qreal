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

#include "scene.h"

#include <limits>
#include <QtCore/QPointF>
#include <QtGui/QKeyEvent>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtWidgets/QApplication>

#include <qrgui/controller/commands/doNothingCommand.h>

#include "mainWindow/shapeEdit/commands/simpleTemplateCommand.h"
#include "mainWindow/shapeEdit/commands/resizeItemCommand.h"
#include "mainWindow/shapeEdit/commands/moveItemCommand.h"
#include "mainWindow/shapeEdit/commands/addItemCommand.h"
#include "mainWindow/shapeEdit/commands/removeItemCommand.h"
#include "mainWindow/shapeEdit/item/typedEntity.h"

using namespace qReal;
using namespace qReal::shapeEdit;
using namespace qReal::commands;

Scene::Scene(graphicsUtils::AbstractView *view, Controller *controller, QObject *parent)
    : AbstractScene(view, parent)
    , mController(controller)
    , mNewItem(nullptr)
	, mWaitMove(false)
    , mIsAddingFinished(false)
    , mResizeCommand(nullptr)
    , mMoveCommand(nullptr)
	, mSelectedTextPicture(nullptr)
{
	mSizeEmptyRectX = sizeEmptyRectX;
	mSizeEmptyRectY = sizeEmptyRectY;
	setItemIndexMethod(NoIndex);
	setEmptyRect(0, 0, mSizeEmptyRectX, mSizeEmptyRectY);
	setEmptyPenBrushItems();
	mCopyPaste = nonePaste;
	mPortType = "NonTyped";

	connect(this, SIGNAL(selectionChanged()), this, SLOT(changePalette()));
	connect(this, SIGNAL(selectionChanged()), this, SLOT(changeFontPalette()));
	connect(this, SIGNAL(selectionChanged()), this, SLOT(changePortsComboBox()));

	mZValue = 0;
}

QRectF Scene::selectedItemsBoundingRect() const
{
	QRectF resBoundRect;
	QList<Item *> list = mListSelectedItemsForPaste;
	foreach (Item *graphicsItem, list)
		resBoundRect |= graphicsItem->sceneBoundingRect();
	return resBoundRect;
}

QPoint Scene::centerEmpty()
{
	return QPoint(sizeEmptyRectX / 2, sizeEmptyRectY / 2);
}

void Scene::setZValue(Item* item)
{
	item->setItemZValue(mZValue);
	mZValue++;
}


void Scene::setPenBrushForItem(Item *item)
{
    item->setPenBrush(mPenStyleItems, mPenWidthItems
                      , mPenColorItems, mBrushStyleItems, mBrushColorItems);
}

void Scene::removeMoveFlagForItem(QGraphicsSceneMouseEvent *event, Item *item)
{
    removeMoveFlag(event, item);
}

void Scene::setIsAddingFinished(bool isFinished)
{
    mIsAddingFinished = isFinished;
}

void Scene::setWaitMove(bool isWait)
{
    mWaitMove = isWait;
}

bool Scene::getWaitMove()
{
    return mWaitMove;
}

QString Scene::getPortType()
{
    return mPortType;
}

QString Scene::getFileName()
{
    return mFileName;
}


void Scene::setZValueSelectedItems()
{
	mListSelectedItems = selectedItems();
	foreach (QGraphicsItem *graphicsItem, mListSelectedItems) {
        Item *item = dynamic_cast<Item *>(graphicsItem);
		item->setZValue(mZValue);
		mZValue++;
	}
}

void Scene::setNullZValueItems()
{
	foreach (QGraphicsItem *graphicsItem, mListSelectedItems) {
		Item* item = dynamic_cast<Item*>(graphicsItem);
		item->setZValue(item->itemZValue());
	}
	mListSelectedItems.clear();
}

QPair<bool, Item *> Scene::checkOnResize(qreal x, qreal y)
{
	QList<Item *> list = selectedSceneItems();
	foreach (Item *item, list) {
		item->changeDragState(x, y);
		item->changeScalingPointState(x, y);
		if (item->dragState() != Item::None)  {
			mView->setDragMode(QGraphicsView::NoDrag);
			return QPair<bool, Item *>(true, item);
		}
	}
	return QPair<bool, Item *>(false, nullptr);
}


void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);

    if (mNewItem) {
        setDragMode(QGraphicsView::NoDrag);
        AbstractCommand *command = mNewItem->mousePressEvent(event, this);
        if (command) {
            mController->execute(command);
        }
    } else {
        setDragMode(QGraphicsView::RubberBandDrag);

        forPressResize(event);
        mChangingItem = dynamic_cast<Item *>(mGraphicsItem);

        if (mChangingItem) {
            mChangingItem->changeScalingPointState(mX1, mY1);
            if (mChangingItem->getScalingPointState() != Item::noneScale) {
                //changing scalePointState
                mChangingItem->setScalingPointColor();
                update();
            }

            if (mGraphicsItem->dragState() != graphicsUtils::AbstractItem::None) {
                //resizing
                mResizeCommand = new ResizeItemCommand(this, mChangingItem);
                mResizeCommand->startTracking();
            } else {
                //moving
                mMoveCommand = new MoveItemCommand(this, selectedItems());
                mMoveCommand->startTracking();
            }
        }
        setZValueSelectedItems();
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);

    if (mNewItem) {
        setDragMode(QGraphicsView::NoDrag);
        AbstractCommand *command = mNewItem->mouseMoveEvent(event, this);
        if (command) {
            mController->execute(command);
        }
        update();
    } else {
        setDragMode(QGraphicsView::RubberBandDrag);
        forMoveResize(event);
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    AbstractCommand *command = nullptr;

    if (mNewItem) {
        setDragMode(QGraphicsView::NoDrag);
        command = mNewItem->mouseReleaseEvent(event, this);
        if (command) {
            mController->execute(command);
        }
    } else {
        setDragMode(QGraphicsView::RubberBandDrag);
        forReleaseResize(event);

        if (mMoveCommand) {
            mMoveCommand->stopTracking();
            if (mMoveCommand->isMoved()) {
                mController->execute(mMoveCommand);
            } else {
                delete mMoveCommand;
            }
            mMoveCommand = nullptr; // Controller takes ownership
        } else if (mResizeCommand) {
            mResizeCommand->stopTracking();
            mController->execute(mResizeCommand);
            mResizeCommand = nullptr;
        }
    }

    mWaitMove = false;
    setMoveFlag(event);
    setNullZValueItems();
    resetItemCreating();
}

void Scene::resetItemCreating()
{
    if (mIsAddingFinished) {
        emit resetHighlightAllButtons();
        mNewItem = nullptr;
        mIsAddingFinished = false;
    }
}


void Scene::initPasteItemsBuffer()
{
    for (auto item : mListSelectedItemsForPaste) {
        delete item;
    }
    mListSelectedItemsForPaste.clear();

    for (auto item : selectedSceneItems()) {
        mListSelectedItemsForPaste.push_back(item->clone());
    }
}

void Scene::keyPressEvent(QKeyEvent *keyEvent)
{
    QGraphicsScene::keyPressEvent(keyEvent);

    if (keyEvent->matches(QKeySequence::Cut)) {
        initPasteItemsBuffer();
        mCopyPaste = cut;

        auto metaRemove = new DoNothingCommand();
        foreach(auto item, selectedItems()) {
            metaRemove->addPostAction(new RemoveItemCommand(this, item));
        }
        mController->execute(metaRemove);

    } else if (keyEvent->matches(QKeySequence::Copy)) {
        initPasteItemsBuffer();
        mCopyPaste = copy;

    } else if (keyEvent->matches(QKeySequence::Paste)) {
        QPointF posCursor(mView->mapFromGlobal(QCursor::pos()));
        posCursor = mView->mapToScene(posCursor.toPoint());
        QPointF topLeftSelection(selectedItemsBoundingRect().topLeft());
        clearSelection();

        auto metaAdd = new DoNothingCommand();
        foreach(Item *item, mListSelectedItemsForPaste) {
            auto newItem = item->clone();
            newItem->setSelected(true);
            newItem->setPos(posCursor - topLeftSelection + newItem->scenePos());
            setZValue(newItem);
            metaAdd->addPostAction(new AddItemCommand(this, newItem));
        }
        mController->execute(metaAdd);
    }
}

void Scene::addShapeEditItem(bool checked, Item *item)
{
    if (checked) {
        mNewItem = item;
    }
}

void Scene::addNone(bool checked)
{
    addShapeEditItem(checked, nullptr);
}

void Scene::addImage(const QString &fileName)
{
	mFileName = fileName;

	QString workingDirName = QFileInfo(QApplication::applicationFilePath()).absoluteDir().absolutePath();
	QDir dir(workingDirName);
	dir.mkdir("images");
	mFileName = workingDirName + "/images/" + fileName.section('/', -1);
	QFile::copy(fileName, mFileName);
}

void Scene::deleteItem()
{
    auto metaRemove = new DoNothingCommand();
    foreach(auto item, selectedItems()) {
        metaRemove->addPostAction(new RemoveItemCommand(this, item));
    }
    mController->execute(metaRemove);
}

void Scene::clearScene()
{
    auto removeAll = new DoNothingCommand();
    foreach(auto item, items()) {
        removeAll->addPostAction(new RemoveItemCommand(this, item));
    }
    mController->execute(removeAll);
	mEmptyRect = addRect(0, 0, sizeEmptyRectX, sizeEmptyRectY, QPen(Qt::white));
}

QList<Item *> Scene::selectedSceneItems()
{
	QList<Item *> resList;
	mListSelectedItems = selectedItems();
	foreach (QGraphicsItem *graphicsItem, mListSelectedItems) {
        Item *item = dynamic_cast<Item *>(graphicsItem);
		if (item != nullptr)
			resList.push_back(item);
	}
	qSort(resList.begin(), resList.end(), compareItems);
	return resList;
}

QList<TextPicture *> Scene::selectedTextPictureItems()
{
	QList<TextPicture *> resList;
	mListSelectedItems = selectedItems();
	foreach (QGraphicsItem *graphicsItem, mListSelectedItems) {
		TextPicture* item = dynamic_cast<TextPicture*>(graphicsItem);
		if (item != nullptr)
			resList.push_back(item);
	}
	return resList;
}

void Scene::changePenStyle(const QString &text)
{
    auto action = [&] (const QString &text, QList<Item *> targets) {
        clearSelection();

        mPenStyleItems = text;
        foreach (Item *item, targets) {
            item->setPenStyle(text);
            item->setSelected(true);
        }

        if (!targets.isEmpty()) {
            changePalette();
        }
        update();
    };

    auto targets = selectedSceneItems();
    if (!targets.isEmpty()) {
        foreach (auto item, targets) {
            auto old = item->pen().style();
            item->setPenStyle(text);

            if (item->pen().style() != old) {
                mController->execute(new SimpleTemplateCommand<QString, QList<Item *>>(
                                         action, targets, mPenStyleItems, text));
                break;
            }
        }
    } else {
        mPenStyleItems = text;
    }
}

void Scene::changePenWidth(int width)
{
    auto action = [&] (int width, QList<Item *> targets) {
        clearSelection();

        mPenWidthItems = width;
        foreach (Item *item, targets) {
            item->setPenWidth(width);
            item->setSelected(true);
        }

        if (!targets.isEmpty()) {
            changePalette();
        }
        update();
    };

    auto targets = selectedSceneItems();
    if (!targets.isEmpty()) {
        foreach (auto item, targets) {
            if (item->pen().width() != width) {
                mController->execute(new SimpleTemplateCommand<int, QList<Item *>>(
                                         action, targets, mPenWidthItems, width));
                break;
            }
        }
    } else {
        mPenWidthItems = width;
    }
}

void Scene::changePenColor(const QString &text)
{
    auto action = [&] (const QString &text, QList<Item *> targets) {
        clearSelection();

        mPenColorItems = text;
        foreach (Item *item, targets) {
            item->setPenColor(text);
            item->setSelected(true);
        }

        if (!targets.isEmpty()) {
            changePalette();
        }
        update();
    };

    auto targets = selectedSceneItems();
    if (!targets.isEmpty()) {
        foreach (auto item, targets) {
            auto old = item->pen().color();
            item->setPenColor(text);

            if (item->pen().color() != old) {
                mController->execute(new SimpleTemplateCommand<QString, QList<Item *>>(
                                         action, targets, mPenColorItems, text));
                break;
            }
        }
    } else {
        mPenColorItems = text;
    }
}

void Scene::changeBrushStyle(const QString &text)
{
    auto action = [&] (const QString &text, QList<Item *> targets) {
        clearSelection();

        mBrushStyleItems = text;
        foreach (Item *item, targets) {
            item->setBrushStyle(text);
            item->setSelected(true);
        }

        if (!targets.isEmpty()) {
            changePalette();
        }
        update();
    };

    auto targets = selectedSceneItems();
    if (!targets.isEmpty()) {
        foreach (auto item, targets) {
            auto old = item->brush().style();
            item->setBrushStyle(text);

            if (item->brush().style() != old) {
                mController->execute(new SimpleTemplateCommand<QString, QList<Item *>>(
                                         action, targets, mBrushStyleItems, text));
                break;
            }
        }
    } else {
        mBrushStyleItems = text;
    }
}

void Scene::changeBrushColor(const QString &text)
{
    auto action = [&] (const QString &text, QList<Item *> targets) {
        clearSelection();

        mBrushColorItems = text;
        foreach (Item *item, targets) {
            item->setBrushColor(text);
            item->setSelected(true);
        }

        if (!targets.isEmpty()) {
            changePalette();
        }
        update();
    };

    auto targets = selectedSceneItems();
    if (!targets.isEmpty()) {
        foreach (auto item, targets) {
            auto old = item->brush().color();
            item->setBrushColor(text);

            if (item->brush().color() != old) {
                mController->execute(new SimpleTemplateCommand<QString, QList<Item *>>(
                                         action, targets, mBrushColorItems, text));
                break;
            }
        }
    } else {
        mBrushColorItems = text;
    }
}

void Scene::changePortsType(const QString &type)
{
    auto action = [&] (const QString &type, QList<QGraphicsItem *> targets) {
        clearSelection();

        mPortType = type;
        foreach (auto item, targets) {
            TypedEntity *typedItem = dynamic_cast<TypedEntity *>(item);
            if (typedItem) {
                typedItem->setType(type);
                item->setSelected(true);
            }
        }

        if (!targets.isEmpty()) {
            changePortsComboBox();
        }
        update();
    };

    auto targets = selectedItems();
    if (!targets.isEmpty()) {
        foreach (auto item, targets) {
            TypedEntity *typedItem = dynamic_cast<TypedEntity *>(item);
            if (typedItem) {
                if (typedItem->getType() != type) {
                    mController->execute(new SimpleTemplateCommand<QString, QList<QGraphicsItem *>>(
                                             action, targets, mPortType, type));
                    break;
                }
            }
        }
    } else {
        mPortType = type;
    }
}

void Scene::changePalette()
{
    if(!mNewItem) {
		mListSelectedItems = selectedItems();
		if (mListSelectedItems.isEmpty()) {
			emit noSelectedItems();
			setEmptyPenBrushItems();
        } else {
            Item *item = dynamic_cast<Item *>(mListSelectedItems.back());
            if (item) {
				QPen penItem = item->pen();
				QBrush brushItem = item->brush();
				emit existSelectedItems(penItem, brushItem);
				setPenBrushItems(penItem, brushItem);
			}
		}
	}
}

void Scene::changeFontPalette()
{
    QList<TextPicture *> textPictureItems = selectedTextPictureItems();
    if (textPictureItems.isEmpty())
		emit noSelectedTextPictureItems();
	else {
        TextPicture* item = dynamic_cast<TextPicture*>(textPictureItems.back());
		if (item != nullptr) {
			QPen penItem = item->pen();
			QFont fontItem = item->font();
			emit existSelectedTextPictureItems(penItem, fontItem, item->name());
		}
	}
}

void Scene::changePortsComboBox()
{
    foreach (QGraphicsItem *item, selectedItems()) {
        TypedEntity *typedItem = dynamic_cast<TypedEntity *>(item);
        if (typedItem) {
            emit existSelectedPortItems(typedItem->getType());
            return;
        }
    }
	emit noSelectedPortItems();
}

void Scene::changeFontFamily(const QFont& font)
{
	foreach (TextPicture *item, selectedTextPictureItems())
		item->setFontFamily(font);
	update();
}

void Scene::changeFontPixelSize(int size)
{
	foreach (TextPicture *item, selectedTextPictureItems())
		item->setFontPixelSize(size);
	update();
}

void Scene::changeFontColor(const QString &text)
{
	foreach (TextPicture *item, selectedTextPictureItems())
		item->setFontColor(text);
	update();
}

void Scene::changeFontItalic(bool isChecked)
{
	foreach (TextPicture *item, selectedTextPictureItems())
		item->setFontItalic(isChecked);
	update();
}

void Scene::changeFontBold(bool isChecked)
{
	foreach (TextPicture *item, selectedTextPictureItems())
		item->setFontBold(isChecked);
	update();
}

void Scene::changeFontUnderline(bool isChecked)
{
	foreach (TextPicture *item, selectedTextPictureItems())
		item->setFontUnderline(isChecked);
	update();
}

void Scene::changeTextName(const QString &name)
{
    foreach (TextPicture *item, selectedTextPictureItems())
        item->setTextName(name);
    update();
}
