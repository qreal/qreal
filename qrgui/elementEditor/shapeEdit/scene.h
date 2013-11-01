#pragma once

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsView>
#include <QtCore/QList>

#include <qrutils/graphicsUtils/abstractScene.h>
#include <qrkernel/settingsManager.h>

#include "elementEditor/shapeEdit/view.h"
#include "elementEditor/shapeEdit/item.h"
#include "elementEditor/shapeEdit/arch.h"
#include "elementEditor/shapeEdit/line.h"
#include "elementEditor/shapeEdit/ellipse.h"
#include "elementEditor/shapeEdit/rectangle.h"
#include "elementEditor/shapeEdit/text.h"
#include "elementEditor/shapeEdit/textPicture.h"
#include "elementEditor/shapeEdit/pointPort.h"
#include "elementEditor/shapeEdit/linePort.h"
#include "elementEditor/shapeEdit/stylus.h"
#include "elementEditor/shapeEdit/path.h"
#include "elementEditor/shapeEdit/curve.h"
#include "elementEditor/shapeEdit/image.h"
#include "umllib/label.h"

int const sizeEmptyRectX = 680;
int const sizeEmptyRectY = 580;

class Scene : public graphicsUtils::AbstractScene
{
	Q_OBJECT
public:
	Scene(graphicsUtils::AbstractView *view, QObject *parent = 0);
	QPoint centerEmpty();
	void changeTextName(QString const &name);
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

	QList<Item *> selectedSceneItems();

public slots:
	void clearScene();

signals:
	void noSelectedItems();
	void existSelectedItems(QPen const &penItem, QBrush const &brushItem);
	void noSelectedTextPictureItems();
	void existSelectedTextPictureItems(QPen const &penItem, QFont const &fontItem, QString const &name);
	void noSelectedPortItems();
	void existSelectedPortItems(QString const &type);
	void resetHighlightAllButtons();

private slots:
	void changePenStyle(QString const &text);
	void changePenWidth(int width);
	void changePenColor(QString const &text);
	void changeBrushStyle(QString const &text);
	void changeBrushColor(QString const &text);
	void changePortsType(QString const &type);

	void changePalette();
	void changeFontPalette();
	void changePortsComboBox();

	void changeFontFamily(const QFont& font);
	void changeFontPixelSize(int size);
	void changeFontColor(QString const &text);
	void changeFontItalic(bool isChecked);
	void changeFontBold(bool isChecked);
	void changeFontUnderline(bool isChecked);

	void deleteItem();

private:
	enum ItemTypes {
		none = 0
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
		nonePaste = 0
		, copy
		, cut
	};

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
};
