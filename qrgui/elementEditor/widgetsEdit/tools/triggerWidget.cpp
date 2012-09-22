#include <QtCore/QEvent>
#include <QtGui/QFocusEvent>

#include "triggerWidget.h"

using namespace qReal::widgetsEdit;

TriggerWidget::TriggerWidget()
	: QWidget(NULL), mState(0)
	, mAction(Trigger::OnFocused)
	, mState1Widget(NULL), mState2Widget(NULL)
{
	mLayout = new QStackedLayout;
	setLayout(mLayout);
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
	mLayout->addWidget(mState1Widget);
	mLayout->addWidget(mState2Widget);
}

void TriggerWidget::changeState(int const state)
{
	mState = state;
	mLayout->setCurrentIndex(state);
}

void TriggerWidget::enterEvent(QEvent *event)
{
	event->accept();
	if (Trigger::OnMouseOver == mAction) {
		changeState(0);
	}
}

void TriggerWidget::leaveEvent(QEvent *event)
{
	event->accept();
	if (Trigger::OnMouseOver == mAction) {
		changeState(1);
	}
}

void TriggerWidget::focusInEvent(QFocusEvent *event)
{
	event->accept();
	if (Trigger::OnFocused == mAction) {
		changeState(0);
	}
}

void TriggerWidget::focusOutEvent(QFocusEvent *event)
{
	event->accept();
	if (Trigger::OnFocused == mAction) {
		changeState(1);
	}
}
