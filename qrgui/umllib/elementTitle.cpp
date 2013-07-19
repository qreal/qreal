#include <QtGui/QTextCursor>

#include "elementTitle.h"
#include "nodeElement.h"
#include "edgeElement.h"
#include "private/fontCache.h"

using namespace qReal;

const QString separator = "»";
const int minTextRect = 28;
const int reductCoeff = 3;

ElementTitle::ElementTitle(qreal x, qreal y, QString const &text, qreal rotation)
		: mFocusIn(false), mReadOnly(true), mScalingX(false), mScalingY(false), mRotation(rotation)
		, mPoint(x, y), mBinding(""), mBackground(Qt::transparent), mIsHard(false), isStretched(false)
{
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable);
	setTitleFont();
	setPos(x, y);
	setPlainText(text);
}

ElementTitle::ElementTitle(qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation)
		: mFocusIn(false), mReadOnly(readOnly), mScalingX(false), mScalingY(false), mRotation(rotation)
		, mPoint(x, y), mBinding(binding), mBackground(Qt::transparent), mIsHard(false)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemIgnoresTransformations);
	setTitleFont();
	setPos(x, y);
}

QRectF ElementTitle::boundingRect() const
{
	return mContents;
}

QPainterPath ElementTitle::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());

	return path;
}

QString ElementTitle::createTextForRepo() const
{
	return (toPlainText()
			+ separator
			+ QString::number(pos().x())
			+ " "
			+ QString::number(pos().y())
			+ separator
			+ QString::number(mContents.width())
			+ " "
			+ QString::number(mContents.height()));
}

void ElementTitle::setTextFromRepo(QString const& text)
{
	if (!text.count(separator))
	{
		updateData();
		return;
	}

	QStringList prList = text.split(separator);

	QStringList coordinates = prList[coordinate].split(" ");
	qreal x = coordinates[0].toDouble();
	qreal y = coordinates[1].toDouble();

	QStringList rectProperties = prList[rectProp].split(" ");
	qreal width = rectProperties[0].toDouble();
	qreal height = rectProperties[1].toDouble();

	setProperties(x, y, width, height, prList[propertyText]);
}

void ElementTitle::setProperties(qreal x, qreal y, qreal width, qreal height, QString const &text)
{
	updateRect(width, height);

	setPlainText(text);

	setPos(x, y);
}

void ElementTitle::updateData()
{
	QString value = createTextForRepo();
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
	if (hasFocus()) {
		ElementTitleInterface::mousePressEvent(event);
		event->accept();
		return;
	}

	if ((event->pos().x() >= boundingRect().right() - minTextRect / 2)
			&& (event->pos().y() >= boundingRect().bottom() - minTextRect / 2)) {
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
			updateRect(cursorPoint.x(), cursorPoint.y());
			return;
		}
		ElementTitleInterface::mouseMoveEvent(event);
		event->accept();
	}
}

void ElementTitle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (!hasFocus()) {
		updateData();
	}

	ElementTitleInterface::mouseReleaseEvent(event);
}

void ElementTitle::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	if (hasFocus()) {
		return;
	}

	if ((event->pos().x() >= boundingRect().right() - minTextRect / 2)
			&& (event->pos().y() >= boundingRect().bottom() - minTextRect / 2)) {
		setCursor(Qt::SizeAllCursor);
	} else {
		unsetCursor();
	}
}

void ElementTitle::init(QRectF const &contents)
{
	mContents.setWidth(contents.width() / reductCoeff);
	mContents.setHeight(contents.height() / reductCoeff);

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

//	QString const htmlNormalizedText = toHtml().remove("\n", Qt::CaseInsensitive);

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
		updateData();
	}
//	setPlainText()

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
	if (isSelected() || parentItem()->isSelected())
	{
		QColor color;
		QPen pen;

		pen.setWidth(1);
		pen.setStyle(Qt::DashLine);

		color.setNamedColor("#f8f8ff");
		painter->save();
		painter->setPen(pen);
		painter->setBrush(QBrush(color));
		painter->drawRect(boundingRect());
		painter->restore();
	}

	if (isSelected() && !hasFocus())
	{

	}

	if (!toPlainText().isEmpty()) {
		painter->save();
		painter->setBrush(QBrush(mBackground));
		painter->setPen(QPen(Qt::transparent));
		painter->drawRect(boundingRect());
		painter->restore();
	}

	QStyleOptionGraphicsItem myOption(*option);
	myOption.state &= ~QStyle::State_Selected;
	myOption.state &= ~QStyle::State_HasFocus;

	QGraphicsTextItem::paint(painter, &myOption, widget);
}

void ElementTitle::updateRect(qreal width, qreal height)
{
	if (width < minTextRect) {
		width = minTextRect;
	}

	if (height < minTextRect) {
		height = minTextRect;
	}

	mContents.setBottomRight(QPointF(width, height));
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
