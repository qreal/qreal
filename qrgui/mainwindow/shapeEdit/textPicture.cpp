#include "textPicture.h"

TextPicture::TextPicture()
	: Text(false)
{
	mDomElementType = pictureType;
	mFont.setPixelSize(15);
	mBoundingRect = QRectF(mX1, mY1, 0, 0);
	mIsDynamicText = false;
}

TextPicture::TextPicture(int x, int y, QString const &text)
	: Text()
{
	mDomElementType = pictureType;
	mFont.setPixelSize(15);
	mBoundingRect = QRectF(x, y, 0, 0);
	mIsDynamicText = false;
	init(x, y, text);
}

TextPicture::TextPicture(TextPicture const &other)
	:Text()
{
	mNeedScalingRect = other.mNeedScalingRect ;
	mPen = other.mPen;
	mBrush = other.mBrush;
	mDomElementType = pictureType;
	mX1 = other.mX1;
	mY1 = other.mY1;
	mText.setPos(other.mText.x(), other.mText.y());
	mText.setFlags(other.mText.flags());
	mText.setTextInteractionFlags(Qt::TextEditorInteraction);
	mText.setPlainText(other.mText.toPlainText());
	mText.setParentItem(other.parentItem());
	mFont = other.mFont;
	mBoundingRect = other.mBoundingRect;
	mListScalePoint = other.mListScalePoint;
	setPos(other.x(), other.y());
}

Item* TextPicture::clone()
{
	TextPicture* item = new TextPicture(*this);
	return item;
}

void TextPicture::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->setFont(mFont);
	painter->setPen(mPen);
	mText.setVisible(false);
	qreal x = mX1;
	qreal y = mY1;
	mBoundingRect = QRectF();
	QString str = mText.toPlainText();
	str += "\n";
	int countStr = 0;
	while (str.contains('\n'))
	{
		int i = str.indexOf('\n');
		QString temp = str.left(i);
		str.remove(0, i + 1);
		QRectF rect;
		painter->drawText(QRectF(static_cast<int>(x), static_cast<int>(y), 500, 500), Qt::AlignLeft | Qt::AlignTop, temp, &rect);
		y += painter->font().pixelSize();
		mBoundingRect |= rect;
		countStr++;
	}
	drawForPictureText(painter, mBoundingRect);
}

void TextPicture::drawForPictureText(QPainter* painter, QRectF rect)
{
	QPen pen(Qt::gray);
	pen.setWidth(1);
	pen.setStyle(Qt::DashDotLine);
	painter->setPen(pen);
	painter->drawRect(rect);
}

void TextPicture::setIsDynamicText(bool isDynamic)
{
	Q_UNUSED(isDynamic);
	mIsDynamicText = false;
}

QFont TextPicture::font() const
{
	return mFont;
}

QString TextPicture::name() const
{
	return mText.toPlainText();
}

void TextPicture::setPoint(QPoint const &point)
{
	setPos(point);
}

void TextPicture::setTextName(const QString &name)
{
	mText.setPlainText(name);
}

void TextPicture::setFontFamily(const QFont& font)
{
	mFont.setFamily(font.family());
}

void TextPicture::setFontPixelSize(int size)
{
	mFont.setPixelSize(size);
}

void TextPicture::setFontColor(const QString & text)
{
	mPen.setColor(QColor(text));
}

void TextPicture::setFontItalic(bool isChecked)
{
	mFont.setItalic(isChecked);
}

void TextPicture::setFontBold(bool isChecked)
{
	mFont.setBold(isChecked);
}

void TextPicture::setFontUnderline(bool isChecked)
{
	mFont.setUnderline(isChecked);
}

void TextPicture::readFont(QDomElement const &docItem)
{
	QDomElement dom = docItem;
	if(!dom.isNull())
	{
		if (dom.hasAttribute("font-fill"))
		{
			QColor color = dom.attribute("font-fill");
			mPen.setColor(color);
		}

		if (dom.hasAttribute("font-size"))
		{
			QString fontsize = dom.attribute("font-size");
			if (fontsize.endsWith("%"))
			{
				fontsize.chop(1);
				mFont.setPixelSize(fontsize.toInt() / 100);
			}
			else if (fontsize.endsWith("a"))
			{
				fontsize.chop(1);
				mFont.setPixelSize(fontsize.toInt());
			}
			else
				mFont.setPixelSize(fontsize.toInt());
		}

		if (dom.hasAttribute("font-name"))
		{
			mFont.setFamily(dom.attribute("font-name"));
		}

		if (dom.hasAttribute("b"))
		{
			mFont.setBold(dom.attribute("b").toInt());
		}

		if (dom.hasAttribute("i"))
		{
			mFont.setItalic(dom.attribute("i").toInt());
		}

		if (dom.hasAttribute("u"))
		{
			mFont.setUnderline(dom.attribute("u").toInt());
		}
	}
}

QDomElement TextPicture::setFontToDoc(QDomDocument &document, QString const &domName)
{
	QDomElement dom = document.createElement(domName);
	dom.setAttribute("font-fill", mPen.color().name());
	dom.setAttribute("font-size", mFont.pixelSize());
	dom.setAttribute("font-name", mFont.family());
	dom.setAttribute("i", mFont.italic() ? "1" : "0");
	dom.setAttribute("u", mFont.underline() ? "1" : "0");
	dom.setAttribute("b", mFont.bold() ? "1" : "0");

	return dom;
}

QPair<QDomElement, Item::DomElementTypes> TextPicture::generateItem(QDomDocument &document, QPoint const &topLeftPicture)
{
	QDomElement text = setFontToDoc(document, "text");
	int const x1 = static_cast<int>(mapToScene(mBoundingRect).boundingRect().left() - topLeftPicture.x());
	int const y1 = static_cast<int>(mapToScene(mBoundingRect).boundingRect().top() - topLeftPicture.y());
	text.setAttribute("y1", setSingleScaleForDoc(4, x1, y1));
	text.setAttribute("x1", setSingleScaleForDoc(0, x1, y1));

	QDomText str = document.createTextNode(mText.toPlainText());
	text.appendChild(str);

	return QPair<QDomElement, Item::DomElementTypes>(text, mDomElementType);
}
