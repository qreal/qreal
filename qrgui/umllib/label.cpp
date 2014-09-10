#include "label.h"

#include <QtGui/QTextCursor>

#include "umllib/nodeElement.h"
#include "umllib/edgeElement.h"
#include "brandManager/brandManager.h"

using namespace qReal;

Label::Label(models::GraphicalModelAssistApi &graphicalAssistApi, Id const &elementId
		, int index, qreal x, qreal y, QString const &text, qreal rotation)
	: mFocusIn(false), mReadOnly(true), mScalingX(false), mScalingY(false), mRotation(rotation)
	, mPoint(x, y), mBinding(""), mBackground(Qt::transparent), mIsStretched(false), mIsHard(false)
	, mParentIsSelected(false), mWasMoved(false), mShouldMove(false)
	, mIndex(index)
	, mId(elementId)
	, mGraphicalModelAssistApi(graphicalAssistApi)
{
	setText(text);
	init();
}

Label::Label(models::GraphicalModelAssistApi &graphicalAssistApi, Id const &elementId
		, int index, qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation)
	: mFocusIn(false), mReadOnly(readOnly), mScalingX(false), mScalingY(false), mRotation(rotation)
	, mPoint(x, y), mBinding(binding), mBackground(Qt::transparent), mIsStretched(false), mIsHard(false)
	, mParentIsSelected(false), mWasMoved(false), mShouldMove(false)
	, mIndex(index)
	, mId(elementId)
	, mGraphicalModelAssistApi(graphicalAssistApi)
{
	init();
}

Label::~Label()
{
}

void Label::init()
{
	QGraphicsTextItem::setFlags(ItemIsSelectable);
	QGraphicsTextItem::setFlag(ItemIsMovable, SettingsManager::value("MoveLabels", true).toBool());

	setTitleFont();
	setRotation(mRotation);
	if (!mBinding.isEmpty()) {
		QList<QPair<QString, QString>> const values = mGraphicalModelAssistApi
				.editorManagerInterface().enumValues(mId, mBinding);
		for (QPair<QString, QString> const &pair : values) {
			mEnumValues[pair.first] = pair.second;
		}
	}
}

void Label::moveToParentCenter()
{
	if (mWasMoved) {
		// now it has user defined position, don't center automatically
		return;
	}

	if (orientation() == Qt::Horizontal) {
		qreal parentCenter = mParentContents.x() + mParentContents.width() / 2;
		qreal titleCenter = x() + mContents.width() / 2;
		qreal diff = parentCenter - titleCenter;

		setX(x() + diff);
	} else if (orientation() == Qt::Vertical) {
		qreal parentCenter = mParentContents.y() + mParentContents.height() / 2;
		qreal titleCenter = y() - mContents.width() / 2;
		qreal diff = parentCenter - titleCenter;

		setY(y() + diff);
	}
}

Qt::Orientation Label::orientation()
{
	if (abs(rotation()) == 90) {
		return Qt::Vertical;
	}

	return Qt::Horizontal;
}

void Label::setText(QString const &text)
{
	setPlainText(text);
}

void Label::setTextFromRepo(QString const &text)
{
	QString const friendlyText = mEnumValues.isEmpty() ? text : enumText(text);
	if (friendlyText != toPlainText()) {
		QGraphicsTextItem::setPlainText(friendlyText);
		setText(toPlainText());
		updateData();
	}
}

void Label::setParentSelected(bool isSelected)
{
	mParentIsSelected = isSelected;
}

void Label::setParentContents(QRectF const &contents)
{
	mParentContents = contents;
	scaleCoordinates(contents);
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

	qreal const x = mPoint.x() * (!mScalingX ? mContents.width() : contents.width());
	qreal const y = mPoint.y() * (!mScalingX ? mContents.height() : contents.height());

	setPos(x, y);
}

void Label::setFlags(GraphicsItemFlags flags)
{
	QGraphicsTextItem::setFlags(flags);
}

void Label::setTextInteractionFlags(Qt::TextInteractionFlags flags)
{
	QGraphicsTextItem::setTextInteractionFlags(flags);
}

void Label::setHtml(QString const &html)
{
	QGraphicsTextItem::setHtml(html);
}

void Label::setPlainText(QString const &text)
{
	QGraphicsTextItem::setPlainText(text);
}

void Label::updateData(bool withUndoRedo)
{
	QString const value = toPlainText();
	NodeElement * const parent = static_cast<NodeElement*>(parentItem());
	if (mBinding == "name") {
		parent->setName(value, withUndoRedo);
	} else if (mEnumValues.isEmpty()) {
		parent->setLogicalProperty(mBinding, value, withUndoRedo);
	} else {
		parent->setLogicalProperty(mBinding, enumText(value), withUndoRedo);
	}

	mGraphicalModelAssistApi.setLabelPosition(mId, mIndex, pos());
	mGraphicalModelAssistApi.setLabelSize(mId, mIndex, this->boundingRect().size());
}

void Label::setTitleFont()
{
	setFont(BrandManager::fonts()->sceneLabelsFont());
}

void Label::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (!mShouldMove) {
		QGraphicsTextItem::mousePressEvent(event);
		event->ignore();
		return;
	}

	mIsStretched = (event->pos().x() >= boundingRect().right() - 10
			&& event->pos().y() >= boundingRect().bottom() - 10);

	QGraphicsTextItem::mousePressEvent(event);
	event->accept();
}

void Label::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (!mShouldMove) {
		setSelected(false);
		return;
	}

	QPointF cursorPoint = mapToItem(this, event->pos());

	if (mIsStretched && SettingsManager::value("ResizeLabels", true).toBool()) {
		updateRect(cursorPoint);
		return;
	}

	if (!SettingsManager::value("MoveLabels", true).toBool()) {
		event->ignore();
		return;
	}

	mWasMoved = true;

	if (!labelMovingRect().contains(event->pos())) {
		event->ignore();
		return;
	}

	QGraphicsTextItem::mouseMoveEvent(event);
	event->accept();
}

void Label::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	mShouldMove = true;

	updateData();

	QGraphicsTextItem::mouseReleaseEvent(event);
	setSelected(true);
}

void Label::init(QRectF const &contents)
{
	mContents = contents;
	mParentContents = contents;

	if (mGraphicalModelAssistApi.hasLabel(mId, mIndex)) {
		QPointF const currentPos = mGraphicalModelAssistApi.labelPosition(mId, mIndex);
		mPoint.setX(currentPos.x() / mContents.width());
		mPoint.setY(currentPos.y() / mContents.height());
		setPos(currentPos);
	} else {
		qreal const x = mPoint.x() * mContents.width();
		qreal const y = mPoint.y() * mContents.height();
		setPos(x, y);
		mGraphicalModelAssistApi.createLabel(mId, mIndex, QPointF(x, y), this->boundingRect().size());
	}
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

bool Label::isReadOnly() const
{
	return mReadOnly;
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
		updateData(true);
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
		QTextCursor cursor = textCursor();
		QString currentText = toPlainText();
		int const oldPos = cursor.position();
		currentText.insert(oldPos, "\n");
		setText(currentText);
		cursor.movePosition(QTextCursor::Start);
		cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, oldPos + 1);
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
	int const distance = SettingsManager::value("LabelsDistance").toInt();
	return mapFromItem(parentItem(), parentItem()->boundingRect()).boundingRect()
			.adjusted(-distance, -distance, distance, distance);
}

QString Label::enumText(QString const &enumValue) const
{
	return mEnumValues.contains(enumValue)
					? mEnumValues.key(enumValue)
					: mGraphicalModelAssistApi.editorManagerInterface().isEnumEditable(mId, mBinding)
							? enumValue
							: QString();
}
