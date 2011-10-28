#include "elementTitle.h"

#include <QtGui/QTextCursor>
#include <QFontDatabase>

#include "nodeElement.h"
#include "edgeElement.h"
ElementTitle::ElementTitle(qreal x, qreal y, QString const &text)
	: mFocusIn(false), mReadOnly(true), mScalingX(false), mScalingY(false), mPoint(x, y), mBinding(""), mBackground(Qt::transparent)
{
	setTitleFont();
	setPos(x, y);
	setHtml(text);

}

ElementTitle::ElementTitle(qreal x, qreal y, QString const &binding, bool readOnly)
	: mFocusIn(false), mReadOnly(readOnly), mScalingX(false), mScalingY(false), mPoint(x, y), mBinding(binding), mBackground(Qt::transparent)
{
	setTitleFont();
	setPos(x, y);
}

void ElementTitle::setTitleFont() {
	if (SettingsManager::value("CustomFont", true).toBool()) {
		QFont font;
		font.fromString(SettingsManager::value("CurrentFont", "ololo").toString());
		setFont(font);
	} else {
		int const fontId = QFontDatabase::addApplicationFont(QDir::currentPath() + "/DejaVuSansCondensed.ttf");
		if (fontId != -1)
			setFont(QFont(QFontDatabase::applicationFontFamilies(fontId).at(0), 7));
	}
}

void ElementTitle::init(QRectF const& contents)
{
	mContents = contents;

	qreal x = mPoint.x() * mContents.width();
	qreal y = mPoint.y() * mContents.height();

	setPos(x, y);
}

void ElementTitle::setScaling(bool scalingX, bool scalingY)
{
	mScalingX = scalingX;
	mScalingY = scalingY;
}

void ElementTitle::setBackground(Qt::GlobalColor const &background)
{
	mBackground = background;
}

void ElementTitle::focusOutEvent(QFocusEvent *event)
{
	QGraphicsTextItem::focusOutEvent(event);

	QString htmlNormalizedText = toHtml().remove("\n", Qt::CaseInsensitive);

	setTextInteractionFlags(Qt::NoTextInteraction);

	parentItem()->setSelected(true);

	// Clear selection
	QTextCursor cursor = textCursor();
	cursor.clearSelection();
	setTextCursor(cursor);

	unsetCursor();

	if (mReadOnly)
		return;

	if (mOldText != toPlainText()) {
		QString value = toPlainText();
		if (mBinding == "name")
			static_cast<NodeElement*>(parentItem())->setName(value);
		else
			static_cast<NodeElement*>(parentItem())->setLogicalProperty(mBinding, value);
	}
	setHtml(htmlNormalizedText);
}

void ElementTitle::keyPressEvent(QKeyEvent *event)
{
	/*if (event->key() == Qt::Key_Escape)
	{
		// Restore previous text and loose focus
		setPlainText(mOldText);
		clearFocus();
		return;
	}
	if (event->key() == Qt::Key_Enter ||
		event->key() == Qt::Key_Return)
	{
		// Loose focus: new name will be applied in focusOutEvent
		clearFocus();
		return;
	}*/
	QGraphicsTextItem::keyPressEvent(event);
}

void ElementTitle::startTextInteraction()
{
	parentItem()->setSelected(true);

	// Already interacting?
	if (hasFocus())
		return;

	mOldText = toPlainText();

	// Clear scene selection
	//if (!(event->modifiers() & Qt::ControlModifier)) - was here.
	scene()->clearSelection();

	if (mReadOnly)
		setTextInteractionFlags(Qt::TextBrowserInteraction);
	else
		setTextInteractionFlags(Qt::TextEditorInteraction);
	setFocus(Qt::OtherFocusReason);

	// Set full text selection
	QTextCursor cursor = QTextCursor(document());
	cursor.select(QTextCursor::Document);
	setTextCursor(cursor);
	setCursor(Qt::IBeamCursor);
}

void ElementTitle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	// if text is not empty, draw it's background
	if (!toPlainText().isEmpty()) {
		painter->save();
		painter->setBrush(QBrush(mBackground));
		painter->setPen(QPen(Qt::transparent));
		painter->drawRect(boundingRect());
		painter->restore();
	}

	QGraphicsTextItem::paint(painter, option, widget);
}

ElementTitleInterface *ElementTitleFactory::createTitle(qreal x, qreal y, QString const &text)
{
	return new ElementTitle(x, y, text);
}

ElementTitleInterface *ElementTitleFactory::createTitle(qreal x, qreal y,QString const &binding, bool readOnly)
{
	return new ElementTitle(x, y, binding, readOnly);
}

void ElementTitle::transform(QRectF const& contents)
{
	qreal x = 0;
	qreal y = 0;

	if (mScalingX)
		x = mPoint.x() * mContents.width();
	else
		x = mPoint.x() * contents.width();

	if (mScalingY)
		y = mPoint.y() * mContents.height();
	else
		y = mPoint.y() * contents.height();

	setPos(x, y);
}

