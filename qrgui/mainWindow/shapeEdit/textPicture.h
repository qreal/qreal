/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include "mainWindow/shapeEdit/text.h"

class TextPicture : public Text
{
public:
	TextPicture();
	TextPicture(int x, int y, const QString &text = "text");
	TextPicture(const TextPicture &other);
	virtual Item* clone();
	void setTextName(const QString &name);
	void setFontFamily(const QFont& font);
	void setFontPixelSize(int size);
	void setFontColor(const QString &text);
	void setFontItalic(bool isChecked);
	void setFontBold(bool isChecked);
	void setFontUnderline(bool isChecked);
	void setPoint(const QPoint &point);
	QFont font() const;
	QString name() const;
	virtual void setIsDynamicText(bool isDynamic);
	void drawForPictureText(QPainter* painter, QRectF rect);
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

	QDomElement setFontToDoc(QDomDocument &document, const QString &domName);
	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document
			, const QPoint &topLeftPicture);

	void readFont(const QDomElement &docItem);

private:
	QFont mFont;
};
