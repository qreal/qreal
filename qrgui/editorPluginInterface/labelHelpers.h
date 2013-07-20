#pragma once
#include <QtCore/QString>
#include <QtWidgets/QGraphicsTextItem>

class LabelInterface : public QGraphicsTextItem
{
public:
	virtual void setBackground(QColor const &background) = 0;
	virtual void setScaling(bool scalingX, bool scalingY) = 0;
	virtual void setHard(bool hard) = 0;
	virtual void setTextFromRepo(QString const& text) = 0;
};

class LabelFactoryInterface
{
public:
	virtual LabelInterface *createTitle(qreal x, qreal y, QString const &text, qreal rotation) = 0;
	virtual LabelInterface *createTitle(qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation) = 0;
};
