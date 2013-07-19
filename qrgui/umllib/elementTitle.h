#pragma once

#include <QtWidgets/QGraphicsTextItem>
#include "../editorPluginInterface/elementTitleHelpers.h"

namespace enums {
namespace OrientationType {
enum OrientationType {
	horizontal,
	vertical
};
}
}

class LabelFactory : public LabelFactoryInterface
{
public:
	LabelInterface *createTitle(qreal x, qreal y, QString const &text, qreal rotation);
	LabelInterface *createTitle(qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation);
};

class Label : public LabelInterface
{
	Q_OBJECT
public:
	Label(qreal x, qreal y, QString const &text, qreal rotation);
	Label(qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation);
	virtual ~Label() {}

	void init(QRectF const& contents);
	void setBackground(QColor const &background);
	void setScaling(bool scalingX, bool scalingY);

	bool isHard() const;
	virtual void setHard(bool hard);

	void startTextInteraction();
	void setTitleFont();

	void setTextFromRepo(QString const& text);

	void setParentSelected(bool isSelected);
	void setParentContents(QRectF contents);

protected:
	enum InterpriterPropertyType
	{
		propertyText,
		coordinate,
		textWidth
	};

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	virtual void focusOutEvent(QFocusEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = NULL);
private:
	void updateData();
	void updateRect(QPointF newBottomRightPoint);
	void setProperties(qreal x, qreal y, qreal width, QString const &text);
	QString createTextForRepo() const;
	void setText(QString const &text);
	void moveToParentCenter();
	enums::OrientationType::OrientationType orientation();

	bool mFocusIn;
	bool mReadOnly;
	bool mScalingX;
	bool mScalingY;
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
};
