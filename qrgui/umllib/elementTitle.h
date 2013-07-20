#pragma once

#include <QtWidgets/QGraphicsTextItem>
#include "../editorPluginInterface/elementTitleHelpers.h"

class ElementTitle;

class ElementTitleFactory : public ElementTitleFactoryInterface
{
public:
	ElementTitleInterface *createTitle(qreal x, qreal y, QString const &text, qreal rotation);
	ElementTitleInterface *createTitle(qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation);
};

class ElementTitle : public ElementTitleInterface
{
	Q_OBJECT
public:
	ElementTitle(qreal x, qreal y, QString const &text, qreal rotation);
	ElementTitle(qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation);
	virtual ~ElementTitle() {}

	virtual QRectF boundingRect() const;
	QPainterPath shape() const;

	void init(QRectF const& contents);
	void setBackground(QColor const &background);
	void setScaling(bool scalingX, bool scalingY);

	bool isHard() const;
	virtual void setHard(bool hard);

	void startTextInteraction();
	void transform(QRectF const& contents);
	void setTitleFont();

	void setTextFromRepo(QString const& text);

protected:
	enum InterpriterPropertyType
	{
		propertyText,
		coordinate,
		rectProp
	};

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

	virtual void focusOutEvent(QFocusEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = NULL);
private:
	bool isStretched;
	bool mFocusIn;
	bool mReadOnly;
	bool mScalingX;
	bool mScalingY;
	QRectF mContents;
	qreal mRotation;
	QPointF mPoint;
	QString mOldText;
	QString mBinding;
	QColor mBackground;
	bool mIsHard;

	void updateData();
	void updateRect(qreal width, qreal height);
	void setProperties(qreal x, qreal y, qreal width, qreal height, QString const &text);
	QString createTextForRepo() const;
};
