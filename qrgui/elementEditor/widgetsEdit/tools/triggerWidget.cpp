#include <QtCore/QEvent>
#include <QtGui/QFocusEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>

#include "triggerWidget.h"

using namespace qReal::widgetsEdit;

TriggerWidget::TriggerWidget()
	: QFrame(NULL), mState(Trigger::Inactive)
	, mAction(Trigger::OnFocused)
	, mState1Widget(NULL), mState2Widget(NULL)
{
	init();
}

TriggerWidget::TriggerWidget(QWidget *widget1, QWidget *widget2)
	: QFrame(NULL), mState(Trigger::Inactive)
	, mAction(Trigger::OnFocused)
	, mState1Widget(NULL), mState2Widget(NULL)
{
	init();
	setWidgets(widget1, widget2);
	changeState(Trigger::Inactive);
}

void TriggerWidget::init()
{
	mLayout = new QStackedLayout;
	setLayout(mLayout);
	setFocusPolicy(Qt::StrongFocus);
	setFrameShadow(QFrame::Raised);
	setFrameShape(QFrame::Panel);
}

Trigger::Action TriggerWidget::action() const
{
	return mAction;
}

QWidget *TriggerWidget::state1Widget() const
{
	return mState1Widget;
}

QWidget *TriggerWidget::state2Widget() const
{
	return mState2Widget;
}

void TriggerWidget::setAction(Trigger::Action const action)
{
	mAction = action;
}

void TriggerWidget::setWidgets(QWidget *state1widget, QWidget *state2Widget)
{
	if (mState1Widget) {
		mLayout->removeWidget(mState1Widget);
	}
	if (mState2Widget) {
		mLayout->removeWidget(mState2Widget);
	}
	mState1Widget = state1widget;
	mState2Widget = state2Widget;
	mState1Widget->setParent(this);
	mState2Widget->setParent(this);
	mLayout->insertWidget(0, mState1Widget);
	mLayout->insertWidget(1, mState2Widget);
}

void TriggerWidget::changeState(const Trigger::State state)
{
	mState = state;
	mLayout->setCurrentIndex(Trigger::Active == state ? 0 : 1);
}

void TriggerWidget::enterEvent(QEvent *event)
{
	event->accept();
	if (Trigger::OnMouseOver == mAction) {
		changeState(Trigger::Active);
	}
}

void TriggerWidget::leaveEvent(QEvent *event)
{
	event->accept();
	if (Trigger::OnMouseOver == mAction) {
		changeState(Trigger::Inactive);
	}
}

void TriggerWidget::focusInEvent(QFocusEvent *event)
{
	qDebug() << "FOCUS IN!!!";
	event->accept();
	if (Trigger::OnFocused == mAction) {
		changeState(Trigger::Active);
	}
}

void TriggerWidget::focusOutEvent(QFocusEvent *event)
{
	qDebug() << "FOCUS OUT!!!";
	event->accept();
	if (Trigger::OnFocused == mAction) {
		changeState(Trigger::Inactive);
	}
}

void TriggerWidget::keyPressEvent(QKeyEvent *event)
{
	event->accept();
	if (Trigger::OnF2Pressed == mAction
		&& Qt::Key_F2 == event->key()
		&& Qt::NoModifier == event->modifiers()) {

		changeState(Trigger::Active);
	}
}

void TriggerWidget::mousePressEvent(QMouseEvent *event)
{
	event->accept();
	if (Trigger::OnMouseClick == mAction) {
		changeState(Trigger::Active);
	}
}

void TriggerWidget::mouseReleaseEvent(QMouseEvent *event)
{
	event->accept();
}
