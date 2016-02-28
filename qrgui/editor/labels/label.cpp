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

#include "label.h"

#include <QtGui/QTextCursor>

#include "editor/nodeElement.h"
#include "editor/edgeElement.h"
#include "brandManager/brandManager.h"

using namespace qReal;
using namespace qReal::gui::editor;

Label::Label(models::GraphicalModelAssistApi &graphicalAssistApi
		, const Id &elementId
		, const LabelProperties &properties)
	: mIsStretched(false)
	, mWasMoved(false)
	, mShouldMove(false)
	, mId(elementId)
	, mGraphicalModelAssistApi(graphicalAssistApi)
	, mProperties(properties)
{
	setTextInteractionFlags(Qt::NoTextInteraction);
	if (properties.isStatic()) {
		setText(properties.text());
	}

	init();
}

Label::~Label()
{
}

void Label::init()
{
	QGraphicsTextItem::setFlags(ItemIsSelectable);
	QGraphicsTextItem::setFlag(ItemIsMovable, SettingsManager::value("MoveLabels", true).toBool());

	reinitFont();
	setRotation(mProperties.rotation());
	if (!mProperties.isStatic()) {
		QList<QPair<QString, QString>> const values = mGraphicalModelAssistApi
				.editorManagerInterface().enumValues(mId, mProperties.binding());
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
	if (qAbs(rotation()) == 90) {
		return Qt::Vertical;
	}

	return Qt::Horizontal;
}

void Label::setText(const QString &text)
{
	setPlainText(text);
}

void Label::setTextFromRepo(const QString &text)
{
	const QString friendlyText = mEnumValues.isEmpty()
			? text
			: mEnumValues.contains(text) ? mEnumValues[text] : enumText(text);
	if (friendlyText != toPlainText()) {
		QGraphicsTextItem::setPlainText(friendlyText);
		setText(toPlainText());
		updateData();
	}
}

void Label::setParentContents(const QRectF &contents)
{
	mParentContents = contents;
	scaleCoordinates(contents);
}

void Label::setShouldCenter(bool shouldCenter)
{
	mWasMoved = !shouldCenter;
}

void Label::scaleCoordinates(const QRectF &contents)
{
	if (mWasMoved) {
		return;
	}

	const qreal x = mProperties.x() * (!mProperties.scalingX() ? mContents.width() : contents.width());
	const qreal y = mProperties.y() * (!mProperties.scalingY() ? mContents.height() : contents.height());

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

void Label::setHtml(const QString &html)
{
	QGraphicsTextItem::setHtml(html);
}

void Label::setPlainText(const QString &text)
{
	QGraphicsTextItem::setPlainText(text);
}

void Label::setPrefix(const QString &text)
{
	mProperties.setPrefix(text);
}

void Label::setSuffix(const QString &text)
{
	mProperties.setSuffix(text);
}

void Label::updateData(bool withUndoRedo)
{
	const QString value = toPlainText();
	NodeElement * const parent = static_cast<NodeElement *>(parentItem());
	if (mProperties.binding() == "name") {
		parent->setName(value, withUndoRedo);
	} else if (mEnumValues.isEmpty()) {
		parent->setLogicalProperty(mProperties.binding(), value, withUndoRedo);
	} else {
		const QString repoValue = mEnumValues.values().contains(value)
				? mEnumValues.key(value)
				: enumText(value);
		parent->setLogicalProperty(mProperties.binding(), repoValue, withUndoRedo);
	}

	mGraphicalModelAssistApi.setLabelPosition(mId, mProperties.index(), pos());
	mGraphicalModelAssistApi.setLabelSize(mId, mProperties.index(), boundingRect().size());
}

void Label::reinitFont()
{
	setFont(BrandManager::fonts()->sceneLabelsFont());
}

void Label::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (dynamic_cast<EdgeElement *>(parentItem())) {
		// Passing event to edge because users usially want to edit its property when clicking on it.
		QGraphicsItem::mousePressEvent(event);
		return;
	}

	mIsStretched = (event->pos().x() >= boundingRect().right() - 10
			&& event->pos().y() >= boundingRect().bottom() - 10);

	QGraphicsTextItem::mousePressEvent(event);
	parentItem()->setSelected(true);
	event->accept();
	setSelected(true);
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
	parentItem()->setSelected(true);
	setSelected(true);
}

void Label::init(const QRectF &contents)
{
	mContents = contents;
	mParentContents = contents;

	if (mGraphicalModelAssistApi.hasLabel(mId, mProperties.index())) {
		const QPointF currentPos = mGraphicalModelAssistApi.labelPosition(mId, mProperties.index());
		mProperties.setX(currentPos.x() / mContents.width());
		mProperties.setY(currentPos.y() / mContents.height());
		setPos(currentPos);
	} else {
		const qreal x = mProperties.x() * mContents.width();
		const qreal y = mProperties.y() * mContents.height();
		setPos(x, y);
		mGraphicalModelAssistApi.createLabel(mId, mProperties.index(), QPointF(x, y), boundingRect().size());
	}
}

void Label::setScaling(bool scalingX, bool scalingY)
{
	mProperties.setScalingX(scalingX);
	mProperties.setScalingY(scalingY);
}

void Label::setBackground(const QColor &background)
{
	mProperties.setBackground(background);
}

bool Label::isHard() const
{
	return mProperties.isHard();
}

void Label::setHard(bool hard)
{
	mProperties.setHard(hard);
}

bool Label::isReadOnly() const
{
	return mProperties.isReadOnly();
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

	if (isReadOnly()) {
		return;
	}

	if (mOldText != toPlainText()) {
		updateData(true);
	}
}

void Label::keyPressEvent(QKeyEvent *event)
{
	const int keyEvent = event->key();
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
		const int oldPos = cursor.position();
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

	setTextInteractionFlags(isReadOnly() ? Qt::TextBrowserInteraction : Qt::TextEditorInteraction);
	setFocus(Qt::OtherFocusReason);

	// Set full text selection
	QTextCursor cursor = QTextCursor(document());
	cursor.select(QTextCursor::Document);
	setTextCursor(cursor);
	setCursor(Qt::IBeamCursor);
}

void Label::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (toPlainText().isEmpty() && !parentItem()->isSelected()
			&& !isSelected() && dynamic_cast<EdgeElement *>(parentItem())) {
		/// @todo: Why label decides it? Why not edge element itself?
		return;
	}

	painter->save();
	painter->setBrush(mProperties.background());

	if (isSelected()) {
		painter->setPen(QPen(Qt::DashLine));
	} else {
		painter->setPen(QPen(Qt::lightGray, 1, Qt::DotLine));
	}

	painter->drawRoundedRect(QGraphicsTextItem::boundingRect(), 2, 2);
	painter->restore();

	painter->save();
	painter->setFont(font());
	drawText(painter, prefixRect(), mProperties.prefix());
	drawText(painter, suffixRect(), mProperties.suffix());
	painter->restore();

	// Default dashed frame is drawn arround the whole bounding rect (arround prefix and suffix too). Disabling it.
	const_cast<QStyleOptionGraphicsItem *>(option)->state &= ~QStyle::State_Selected & ~QStyle::State_HasFocus;
	QGraphicsTextItem::paint(painter, option, widget);
}

void Label::drawText(QPainter *painter, const QRectF &rect, const QString &text)
{
	painter->drawText(rect, Qt::AlignCenter, text);
}

QRectF Label::prefixRect() const
{
	const QRectF thisRect = QGraphicsTextItem::boundingRect();
	QRectF textRect = this->textRect(mProperties.prefix());
	const qreal x = thisRect.left() - textRect.width();
	const qreal y = thisRect.top() + (thisRect.height() - textRect.height()) / 2;
	textRect.moveTo(x, y);
	return textRect;
}

QRectF Label::suffixRect() const
{
	const QRectF thisRect = QGraphicsTextItem::boundingRect();
	QRectF textRect = this->textRect(mProperties.suffix());
	const qreal x = thisRect.right();
	const qreal y = thisRect.top() + (thisRect.height() - textRect.height()) / 2;
	textRect.moveTo(x, y);
	return textRect;
}

QRectF Label::textRect(const QString &text) const
{
	return QFontMetrics(font()).boundingRect(text).adjusted(-3, 0, 3, 0);
}

QRectF Label::boundingRect() const
{
	return QGraphicsTextItem::boundingRect().united(prefixRect()).united(suffixRect());
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
	const int distance = SettingsManager::value("LabelsDistance").toInt();
	return mapFromItem(parentItem(), parentItem()->boundingRect()).boundingRect()
			.adjusted(-distance, -distance, distance, distance);
}

QString Label::enumText(const QString &enumValue) const
{
	return mGraphicalModelAssistApi.editorManagerInterface().isEnumEditable(mId, mProperties.binding())
			? enumValue
			: QString();
}
