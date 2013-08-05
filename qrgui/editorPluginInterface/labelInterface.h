#pragma once

#include <QtCore/QString>
#include <QtGui/QColor>
#include <QtWidgets/QGraphicsItem>

class LabelInterface
{
public:
	/// Sets label background.
	virtual void setBackground(QColor const &background) = 0;

	/// Does nothing.
	virtual void setScaling(bool scalingX, bool scalingY) = 0;

	virtual void setHard(bool hard) = 0;
	virtual void setTextFromRepo(QString const& text) = 0;
	virtual void setFlags(QGraphicsItem::GraphicsItemFlags flags) = 0;
	virtual void setTextInteractionFlags(Qt::TextInteractionFlags flags) = 0;
	virtual void setHtml(QString const &html) = 0;
	virtual void setPlainText(QString const &text) = 0;
};
