#pragma once

#include <QtGui/QWidget>
#include <QtGui/QComboBox>
#include <QtGui/QStackedLayout>

#include "trigger.h"

namespace qReal
{
namespace widgetsEdit
{

class TriggerWidget : public QWidget
{
	Q_OBJECT

public:
	TriggerWidget();

	Trigger::Action action() const;
	QWidget *state1Widget() const;
	QWidget *state2Widget() const;
	void setAction(Trigger::Action const action);
	void setWidgets(QWidget *state1widget, QWidget *state2Widget);

private:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void focusInEvent(QFocusEvent *event);
	void focusOutEvent(QFocusEvent *event);

	void changeState(int const state);

	int mState;
	Trigger::Action mAction;
	QWidget *mState1Widget;
	QWidget *mState2Widget;
	QStackedLayout *mLayout;
};

}
}
