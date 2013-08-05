#pragma once

#include <QtWidgets/QGraphicsTextItem>

#include "../editorPluginInterface/labelInterface.h"

namespace enums {
namespace orientationType {
enum OrientationType {
	horizontal
	, vertical
};
}
}

class Label : public QGraphicsTextItem, public LabelInterface
{
	Q_OBJECT

public:
	Label(qreal x, qreal y, QString const &text, qreal rotation);
	Label(qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation);
	virtual ~Label();

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

	void setShouldCenter(bool shouldCenter);

	virtual void setFlags(GraphicsItemFlags flags);
	virtual void setTextInteractionFlags(Qt::TextInteractionFlags flags);
	virtual void setHtml(QString const &html);
	virtual void setPlainText(QString const &text);

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
	void updateData(bool withUndoRedo = false);
	void updateRect(QPointF newBottomRightPoint);
	void setProperties(qreal x, qreal y, qreal width, QString const &text);
	QString createTextForRepo() const;
	void setText(QString const &text);
	void moveToParentCenter();
	enums::orientationType::OrientationType orientation();

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
