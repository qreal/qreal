#include <QtGui/QTextCursor>

#include "label.h"
#include "nodeElement.h"
#include "edgeElement.h"
#include "private/fontCache.h"

using namespace qReal;
using namespace enums;

Label::Label(qreal x, qreal y, QString const &text, qreal rotation)
		: mFocusIn(false), mReadOnly(true), mScalingX(false), mScalingY(false), mRotation(rotation)
		, mPoint(x, y), mBinding(""), mBackground(Qt::transparent), mIsStretched(false), mIsHard(false)
		, mParentIsSelected(false), mWasMoved(false), mShouldMove(false)
{
	setFlags(ItemIsSelectable | ItemIsMovable);
	setTitleFont();
	setPos(x, y);
	setText(text);
	setRotation(mRotation);
}

Label::Label(qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation)
		: mFocusIn(false), mReadOnly(readOnly), mScalingX(false), mScalingY(false), mRotation(rotation)
		, mPoint(x, y), mBinding(binding), mBackground(Qt::transparent), mIsHard(false)
		, mParentIsSelected(false), mWasMoved(false), mShouldMove(false)
{
	setFlags(ItemIsSelectable | ItemIsMovable);
	setTitleFont();
	setPos(x, y);
	setRotation(mRotation);
}

QString Label::createTextForRepo() const
{
	return (toPlainText()
			+ propertiesSeparator + QString::number(pos().x()) + " " + QString::number(pos().y())
			+ propertiesSeparator + QString::number(mContents.width()));
}

void Label::moveToParentCenter()
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

		setY(y() + diff);
	}
}

OrientationType::OrientationType Label::orientation()
{
	if (abs(rotation()) == 90) {
		return OrientationType::vertical;
	}
	return OrientationType::horizontal;
}

void Label::setText(const QString &text)
{
	setPlainText(text);
	moveToParentCenter();
}

void Label::setTextFromRepo(QString const& text)
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

void Label::setParentSelected(bool isSelected)
{
	mParentIsSelected = isSelected;
}

void Label::setParentContents(QRectF contents)
{
	mParentContents = contents;
	scaleCoordinates(contents);
	moveToParentCenter();
}

void Label::setShouldCenter(bool shouldCenter)
{
	mWasMoved = !shouldCenter;
}

void Label::scaleCoordinates(QRectF const &contents)
{
	if (mWasMoved) {
		return;
	}

	qreal x = 0;
	qreal y = 0;

	if (!mScalingX) {
		x = mPoint.x() * mContents.width();
	} else {
		x = mPoint.x() * contents.width();
	}

	if (!mScalingY) {
		y = mPoint.y() * mContents.height();
	} else {
		y = mPoint.y() * contents.height();
	}

	setPos(x, y);
}

void Label::setProperties(qreal x, qreal y, qreal width, QString const &text)
{
	mContents.setWidth(width);
	setTextWidth(mContents.width());

	setText(text);

	setPos(x, y);
}

void Label::updateData()
{
	QString const value = createTextForRepo();
	if (mBinding == "name") {
		static_cast<NodeElement*>(parentItem())->setName(value);
	} else {
		static_cast<NodeElement*>(parentItem())->setLogicalProperty(mBinding, value);
	}
}

void Label::setTitleFont()
{
	setFont(FontCache::instance()->titlesFont());
}

void Label::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (!mShouldMove) {
		LabelInterface::mousePressEvent(event);
		event->accept();
		return;
	}

	mIsStretched = ((event->pos().x() >= boundingRect().right() - 10)
			&& (event->pos().y() >= boundingRect().bottom() - 10));
	LabelInterface::mousePressEvent(event);
	event->accept();
}

void Label::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (!mShouldMove) {
		setSelected(false);
		parentItem()->grabMouse();
		return;
	}

	QPointF cursorPoint = mapToItem(this, event->pos());

	if (mIsStretched  && SettingsManager::value("ResizeLabels", true).toBool()) {
		updateRect(cursorPoint);
		return;
	}

	if (!SettingsManager::value("MoveLabels", true).toBool()) {
		event->ignore();
		return;
	}

	if (!labelMovingRect().contains(event->pos())) {
		event->ignore();
		return;
	}

	mWasMoved = true;
	LabelInterface::mouseMoveEvent(event);
	event->accept();
}

void Label::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	mShouldMove = true;

	if (mIsStretched) {
		moveToParentCenter();
	}
	updateData();

	LabelInterface::mouseReleaseEvent(event);
}


void Label::init(QRectF const &contents)
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

void Label::setScaling(bool scalingX, bool scalingY)
{
	mScalingX = scalingX;
	mScalingY = scalingY;
}

void Label::setBackground(QColor const &background)
{
	mBackground = background;
}

bool Label::isHard() const
{
	return mIsHard;
}

void Label::setHard(bool hard)
{
	mIsHard = hard;
}

void Label::focusOutEvent(QFocusEvent *event)
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

void Label::keyPressEvent(QKeyEvent *event)
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

void Label::startTextInteraction()
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

void Label::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

void Label::updateRect(QPointF newBottomRightPoint)
{
	mContents.setBottomRight(newBottomRightPoint);
	setTextWidth(mContents.width());
}

void Label::clearMoveFlag()
{
	mShouldMove = false;
}

QRectF Label::labelMovingRect() const
{
	int distance = 100;
	return mapFromItem(parentItem(), parentItem()->boundingRect()).boundingRect()
			.adjusted(-distance, -distance, distance, distance);
}

LabelInterface *LabelFactory::createTitle(qreal x, qreal y, QString const &text, qreal rotation)
{
	return new Label(x, y, text, rotation);
}

LabelInterface *LabelFactory::createTitle(qreal x, qreal y,QString const &binding, bool readOnly, qreal rotation)
{
	return new Label(x, y, binding, readOnly, rotation);
}

