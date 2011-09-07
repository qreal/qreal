#pragma once
#include "text.h"

class TextPicture : public Text
{
public:
	TextPicture();
	TextPicture(int x, int y, QString const &text = "text");
	TextPicture(TextPicture const &other);
	virtual Item* clone();
	void setTextName(const QString &name);
	void setFontFamily(const QFont& font);
	void setFontPixelSize(int size);
	void setFontColor(const QString & text);
	void setFontItalic(bool isChecked);
	void setFontBold(bool isChecked);
	void setFontUnderline(bool isChecked);
	void setPoint(QPoint const &point);
	QFont font() const;
	QString name() const;
	virtual void setIsDynamicText(bool isDynamic);
	void drawForPictureText(QPainter* painter, QRectF rect);
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

	QDomElement setFontToDoc(QDomDocument &document, QString const &domName);
	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document, QPoint const &topLeftPicture);
	void readFont(QDomElement const &docItem);

private:
	QFont mFont;
};
