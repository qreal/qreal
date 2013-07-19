#include <QtGui/QTextCursor>

#include "elementTitle.h"
#include "nodeElement.h"
#include "edgeElement.h"
#include "private/fontCache.h"

using namespace qReal;
using namespace enums;

ElementTitle::ElementTitle(qreal x, qreal y, QString const &text, qreal rotation)
		: mFocusIn(false), mReadOnly(true), mScalingX(false), mScalingY(false), mRotation(rotation)
		, mPoint(x, y), mBinding(""), mBackground(Qt::transparent), mIsStretched(false), mIsHard(false)
		, mParentIsSelected(false), mWasMoved(false)
{
	setFlags(ItemIsSelectable | ItemIsMovable);
	setTitleFont();
	setPos(x, y);
	setText(text);
	setRotation(mRotation);
}

ElementTitle::ElementTitle(qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation)
		: mFocusIn(false), mReadOnly(readOnly), mScalingX(false), mScalingY(false), mRotation(rotation)
		, mPoint(x, y), mBinding(binding), mBackground(Qt::transparent), mIsHard(false)
{
	setFlags(ItemIsSelectable | ItemIsMovable);
	setTitleFont();
	setPos(x, y);
	setRotation(mRotation);
}

QString ElementTitle::createTextForRepo() const
{
	return (toPlainText()
			+ propertiesSeparator + QString::number(pos().x()) + " " + QString::number(pos().y())
			+ propertiesSeparator + QString::number(mContents.width()));
}

void ElementTitle::moveToParentCenter()
{
	if (mWasMoved) {
		// now it has user defined position, don't center automatically
		return;
	}

	if (orientation() == OrientationType::horizontal) {
		qreal parentCenter = mParentContents.x() + mParentContents.width() / 2;
		qreal titleCenter = x() + mContents.width() / 2;
		qreal diff = parentCenter - titleCenter;

		setX(x() + diff);
	} else if (orientation() == OrientationType::vertical) {
		qreal parentCenter = mParentContents.y() + mParentContents.height() / 2;
		qreal titleCenter = y() - mContents.width() / 2;
		qreal diff = parentCenter - titleCenter;

		qDebug() << "parent: "<< parentCenter;
		qDebug() << "title : "<< titleCenter;
		qDebug() << diff;

		setY(y() + diff);
	}
}

OrientationType::OrientationType ElementTitle::orientation()
{
	if (abs(rotation()) == 90) {
		return OrientationType::vertical;
	}
	return OrientationType::horizontal;
}

void ElementTitle::setText(const QString &text)
{
	setPlainText(text);
	moveToParentCenter();
}

void ElementTitle::setTextFromRepo(QString const& text)
{
	if (!text.contains(propertiesSeparator)) {
		setHtml(text); // need this to load old saves with html markup
		setText(toPlainText());
		updateData();
		return;
	}

	QStringList const prList = text.split(propertiesSeparator);

	QStringList const coordinates = prList[coordinate].split(" ");
	qreal x = coordinates.at(0).toDouble();
	qreal y = coordinates.at(1).toDouble();

	setProperties(x, y, prList[textWidth].toDouble(), prList[propertyText]);
}

void ElementTitle::setParentSelected(bool isSelected)
{
	mParentIsSelected = isSelected;
}

void ElementTitle::setParentContents(QRectF contents)
{
	mParentContents = contents;
	moveToParentCenter();
}

void ElementTitle::setProperties(qreal x, qreal y, qreal width, QString const &text)
{
	mContents.setWidth(width);
	setTextWidth(mContents.width());

	setText(text);

	setPos(x, y);
}

void ElementTitle::updateData()
{
	QString const value = createTextForRepo();
	if (mBinding == "name") {
		static_cast<NodeElement*>(parentItem())->setName(value);
	} else {
		static_cast<NodeElement*>(parentItem())->setLogicalProperty(mBinding, value);
	}
}

void ElementTitle::setTitleFont()
{
	setFont(FontCache::instance()->titlesFont());
}

void ElementTitle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	mIsStretched = ((event->pos().x() >= boundingRect().right() - 10)
			&& (event->pos().y() >= boundingRect().bottom() - 10));
	ElementTitleInterface::mousePressEvent(event);
	event->accept();
}

void ElementTitle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (!isSelected()) {
		return;
	}

	QPointF cursorPoint = mapToItem(this, event->pos());

	if (mIsStretched) {
		updateRect(cursorPoint);
		return;
	}

	mWasMoved = true;
	ElementTitleInterface::mouseMoveEvent(event);
	event->accept();
}

void ElementTitle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (mIsStretched) {
		moveToParentCenter();
	}
	updateData();

	ElementTitleInterface::mouseReleaseEvent(event);
}


void ElementTitle::init(QRectF const &contents)
{
	mContents = contents;
	mParentContents = contents;
	if (orientation() == OrientationType::horizontal) {
		mContents.setWidth(mContents.width() / 2);
	} else if (orientation() == OrientationType::vertical) {
		mContents.setWidth(mContents.height() * 3 / 4);
	}

	setTextWidth(mContents.width());

	qreal const x = mPoint.x() * mContents.width();
	qreal const y = mPoint.y() * mContents.height();
	setPos(x, y);

	moveToParentCenter();
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
	setTextInteractionFlags(Qt::NoTextInteraction);

	// Clear selection
	QTextCursor cursor = textCursor();
	cursor.clearSelection();
	setTextCursor(cursor);

	unsetCursor();

	if (mReadOnly) {
		return;
	}

	if (mOldText != toPlainText()) {
		updateData();
	}
}

void ElementTitle::keyPressEvent(QKeyEvent *event)
{
	int const keyEvent = event->key();
	if (keyEvent == Qt::Key_Escape) {
		// Restore previous text and loose focus
		setText(mOldText);
		clearFocus();
		return;
	}
	if ((event->modifiers() & Qt::ShiftModifier) && (event->key() == Qt::Key_Return)) {
		// Line feed
		QTextCursor const cursor = textCursor();
		QString const currentText = toPlainText();
		setText(currentText + "\n");
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
	QString text = toPlainText();

	if (text.isEmpty() && !mParentIsSelected && !isSelected()) {
		return;
	}

	painter->save();
	painter->setBrush(QBrush(mBackground));

	if ((mParentIsSelected && toPlainText().isEmpty()) || isSelected()) {
		painter->setPen(QPen(Qt::DotLine));
	} else if (!toPlainText().isEmpty()) {
		painter->setPen(QPen(Qt::transparent));
	}

	painter->drawRect(boundingRect());
	painter->restore();

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

