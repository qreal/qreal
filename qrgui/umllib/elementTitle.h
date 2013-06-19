#pragma once

#include <QtWidgets/QGraphicsTextItem>
#include "../editorPluginInterface/elementTitleHelpers.h"

class ElementTitle;

class ElementTitleFactory : public ElementTitleFactoryInterface
{
public:
	ElementTitleInterface *createTitle(qreal x, qreal y, QString const &text);
	ElementTitleInterface *createTitle(qreal x, qreal y, QString const &binding, bool readOnly);
};

class ElementTitle : public ElementTitleInterface
{
	Q_OBJECT
public:
	ElementTitle(qreal x, qreal y, QString const &text);
	ElementTitle(qreal x, qreal y, QString const &binding, bool readOnly);
	virtual ~ElementTitle() {}

	void init(QRectF const& contents);
	void setBackground(QColor const &background);
	void setScaling(bool scalingX, bool scalingY);

	bool isHard() const;
	virtual void setHard(bool hard);

	void startTextInteraction();
	void transform(QRectF const& contents);
	void setTitleFont();

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

	virtual void focusOutEvent(QFocusEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = NULL);
private:
	bool mFocusIn;
	bool mReadOnly;
	bool mScalingX;
	bool mScalingY;
	QRectF mContents;
	QPointF mPoint;
	QString mOldText;
	QString mBinding;
	QColor mBackground;
	bool mIsHard;
};
