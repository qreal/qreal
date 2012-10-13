#pragma once

#include <QtGui/QWidget>
#include <QtGui/QComboBox>
#include <QtGui/QStackedLayout>

#include "trigger.h"

namespace qReal
{
namespace widgetsEdit
{

class TriggerWidget : public QFrame
{
	Q_OBJECT

public:
	TriggerWidget();
	TriggerWidget(QWidget *widget1, QWidget *widget2);

	Trigger::Action action() const;
	QWidget *state1Widget() const;
	QWidget *state2Widget() const;
	void setAction(Trigger::Action const action);
	void setWidgets(QWidget *widget1, QWidget *widget2);

	void changeState(Trigger::State const state);

private:
	void init();

	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void focusInEvent(QFocusEvent *event);
	void focusOutEvent(QFocusEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	Trigger::State mState;
	Trigger::Action mAction;
	QWidget *mState1Widget;
	QWidget *mState2Widget;
	QStackedLayout *mLayout;
};

}
}
