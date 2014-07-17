#pragma once

#include <QtWidgets/QGraphicsTextItem>

#include "editorPluginInterface/labelInterface.h"
#include "models/graphicalModelAssistApi.h"

namespace qReal {

class Label : public QGraphicsTextItem, public LabelInterface
{
	Q_OBJECT

public:
	Label(models::GraphicalModelAssistApi &graphicalAssistApi, Id const &elementId
			, int index, qreal x, qreal y, QString const &text, qreal rotation);

	Label(models::GraphicalModelAssistApi &graphicalAssistApi, Id const &elementId
			, int index, qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation);

	~Label() override;

	void init(QRectF const &contents);
	void setBackground(QColor const &background);
	void setScaling(bool scalingX, bool scalingY);

	bool isHard() const;
	virtual void setHard(bool hard);

	void startTextInteraction();
	void setTitleFont();

	void setTextFromRepo(QString const& text);

	void setParentSelected(bool isSelected);
	void setParentContents(QRectF const &contents);

	void setShouldCenter(bool shouldCenter);
	void scaleCoordinates(QRectF const &contents);

	void clearMoveFlag();

	virtual void setFlags(GraphicsItemFlags flags);
	virtual void setTextInteractionFlags(Qt::TextInteractionFlags flags);
	virtual void setHtml(QString const &html);
	virtual void setPlainText(QString const &text);

protected:
	enum InterpreterPropertyType
	{
		propertyText
		, coordinate
		, textWidth
	};

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	virtual void focusOutEvent(QFocusEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = NULL);

private:
	void init();
	void updateData(bool withUndoRedo = false);
	void updateRect(QPointF newBottomRightPoint);
	void setText(QString const &text);
	void moveToParentCenter();
	Qt::Orientation orientation();
	QRectF labelMovingRect() const;

	bool mFocusIn;
	bool mReadOnly;
	bool mScalingX;
	bool mScalingY;
	QMap<QString, QString> mEnumValues;
	QRectF mContents;
	QRectF mParentContents;
	qreal mRotation;
	QPointF mPoint;
	QString mOldText;
	QString mBinding;
	QColor mBackground;
	bool mIsStretched;
	bool mIsHard;
	bool mParentIsSelected;
	bool mWasMoved;
	bool mShouldMove;
	int const mIndex;
	Id const mId;
	models::GraphicalModelAssistApi &mGraphicalModelAssistApi;
};

}
