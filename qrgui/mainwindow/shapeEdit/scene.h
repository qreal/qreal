#pragma once

#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QGraphicsView>
#include <QtCore/QList>

#include "view.h"
#include "item.h"
#include "arch.h"
#include "line.h"
#include "ellipse.h"
#include "rectangle.h"
#include "text.h"
#include "textPicture.h"
#include "pointPort.h"
#include "linePort.h"
#include "stylus.h"
#include "path.h"
#include "curve.h"
#include "image.h"
#include "../umllib/elementTitle.h"
#include "../../../qrkernel/settingsManager.h"
#include "../../../qrutils/graphicsUtils/abstractScene.h"

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
	void addImage(QString const &fileName);

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

signals:
	void noSelectedItems();
	void existSelectedItems(QPen const &penItem, QBrush const &brushItem);
	void noSelectedTextPictureItems();
	void existSelectedTextPictureItems(QPen const &penItem, QFont const &fontItem, QString const &name);
	void resetHighlightAllButtons();

private slots:
	void changePenStyle(const QString &text);
	void changePenWidth(int width);
	void changePenColor(const QString &text);
	void changeBrushStyle(const QString &text);
	void changeBrushColor(const QString &text);
	void changePalette();
	void changeFontPalette();

	void changeFontFamily(const QFont& font);
	void changeFontPixelSize(int size);
	void changeFontColor(const QString & text);
	void changeFontItalic(bool isChecked);
	void changeFontBold(bool isChecked);
	void changeFontUnderline(bool isChecked);

	void deleteItem();
	void clearScene();

private:
	enum ItemTypes {
		none,
		line,
		ellipse,
		rectangle,
		text,
		dynamicText,
		textPicture,
		pointPort,
		linePort,
		stylus,
		curve,
		image
	};
	enum CopyPasteType {
		nonePaste,
		copy,
		cut
	};

	int mZValue;
	ItemTypes mItemType;
	bool mWaitMove;
	int mCount;
	Line *mLine;
	Ellipse *mEllipse;
	Rectangle *mRectangle;
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

	void initListSelectedItemsForPaste();
	QRectF selectedItemsBoundingRect() const;
	QList<Item *> selectedSceneItems();
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
