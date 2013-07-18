#include <QtGui/QTextCursor>

#include "elementTitle.h"
#include "nodeElement.h"
#include "edgeElement.h"
#include "private/fontCache.h"

using namespace qReal;

ElementTitle::ElementTitle(qreal x, qreal y, QString const &text, qreal rotation)
		: mFocusIn(false), mReadOnly(true), mScalingX(false), mScalingY(false), mRotation(rotation)
		, mPoint(x, y), mBinding(""), mBackground(Qt::transparent), mIsHard(false), isStretched(false)
{
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable);
	setTitleFont();
	setPos(x, y);
	setHtml(text);
}

ElementTitle::ElementTitle(qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation)
		: mFocusIn(false), mReadOnly(readOnly), mScalingX(false), mScalingY(false), mRotation(rotation)
		, mPoint(x, y), mBinding(binding), mBackground(Qt::transparent), mIsHard(false)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemIgnoresTransformations);
	setTitleFont();
	setPos(x, y);
}

void ElementTitle::setTitleFont()
{
	setFont(FontCache::instance()->titlesFont());
}

void ElementTitle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if ((event->pos().x() >= boundingRect().right() - 10)
			&& (event->pos().y() >= boundingRect().bottom() - 10)) {
		isStretched = true;
	} else {
		isStretched = false;
	}
	ElementTitleInterface::mousePressEvent(event);
	event->accept();

}

void ElementTitle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (isSelected()) {
		QPointF cursorPoint = mapToItem(this, event->pos());
		if (isStretched) {
			updateRect(cursorPoint);
			return;
		}
		ElementTitleInterface::mouseMoveEvent(event);
		event->accept();
	}
}

void ElementTitle::init(QRectF const &contents)
{
	mContents = contents;

	setTextWidth(mContents.width());

	qreal const x = mPoint.x() * mContents.width();
	qreal const y = mPoint.y() * mContents.height();
	setPos(x, y);
}

void ElementTitle::setScaling(bool scalingX, bool scalingY)
{
	mScalingX = scalingX;
	mScalingY = scalingY;
}

void ElementTitle::setBackground(QColor const &background)
{
	mBackground = background;
}

bool ElementTitle::isHard() const
{
	return mIsHard;
}

void ElementTitle::setHard(bool hard)
{
	mIsHard = hard;
}

void ElementTitle::focusOutEvent(QFocusEvent *event)
{
	QGraphicsTextItem::focusOutEvent(event);

	QString const htmlNormalizedText = toHtml().remove("\n", Qt::CaseInsensitive);

	setTextInteractionFlags(Qt::NoTextInteraction);

//	parentItem()->setSelected(true);

	// Clear selection
	QTextCursor cursor = textCursor();
	cursor.clearSelection();
	setTextCursor(cursor);

	unsetCursor();

	if (mReadOnly) {
		return;
	}

	if (mOldText != toPlainText()) {
		QString value = toPlainText();
		if (mBinding == "name") {
			static_cast<NodeElement*>(parentItem())->setName(value);
		} else {
			static_cast<NodeElement*>(parentItem())->setLogicalProperty(mBinding, value);
		}
	}
	setHtml(htmlNormalizedText);

}

void ElementTitle::keyPressEvent(QKeyEvent *event)
{
	int const keyEvent = event->key();
	if (keyEvent == Qt::Key_Escape) {
		// Restore previous text and loose focus
		setPlainText(mOldText);
		clearFocus();
		return;
	}
	if ((event->modifiers() & Qt::ShiftModifier) && (event->key() == Qt::Key_Return)) {
		// Line feed
		QTextCursor const cursor = textCursor();
		QString const currentText = toPlainText();
		setPlainText(currentText + "\n");
		setTextCursor(cursor);
		return;
	}
	if (keyEvent == Qt::Key_Enter || keyEvent == Qt::Key_Return) {
		// Loose focus: new name will be applied in focusOutEvent
		clearFocus();
		return;
	}
	QGraphicsTextItem::keyPressEvent(event);
}

void ElementTitle::startTextInteraction()
{
//	parentItem()->setSelected(true);

	// Already interacting?
	if (hasFocus()) {
		return;
	}

	mOldText = toPlainText();

	setTextInteractionFlags(mReadOnly ? Qt::TextBrowserInteraction : Qt::TextEditorInteraction);
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

void ElementTitle::updateRect(QPointF newBottomRightPoint)
{
	mContents.setBottomRight(newBottomRightPoint);
	setTextWidth(mContents.width());
}


ElementTitleInterface *ElementTitleFactory::createTitle(qreal x, qreal y, QString const &text, qreal rotation)
{
	return new ElementTitle(x, y, text, rotation);
}

ElementTitleInterface *ElementTitleFactory::createTitle(qreal x, qreal y,QString const &binding, bool readOnly, qreal rotation)
{
	return new ElementTitle(x, y, binding, readOnly, rotation);
}

void ElementTitle::transform(QRectF const& contents)
{
	ElementTitleInterface::transform();
}

