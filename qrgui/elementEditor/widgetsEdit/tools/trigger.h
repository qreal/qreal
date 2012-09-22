#pragma once

#include <QtGui/QWidget>
#include <QtGui/QGraphicsLinearLayout>

#include "tool.h"
#include "layoutTool.h"
#include "widget.h"

namespace qReal
{
namespace widgetsEdit
{

int const TRIGGER_DEFAULT_WIDTH = 150;
int const TRIGGER_DEFAULT_HEIGHT = 100;

class TriggerWidget;

class Trigger : public Tool
{
	Q_OBJECT

	Q_ENUMS(Action)
	Q_PROPERTY(Action action READ action WRITE setAction USER true)

public:
	Trigger(ToolController *controller);
	virtual ~Trigger() {}

	bool isUnderConstruction() const;

	virtual void deserializeWidget(QWidget *parent, QDomElement const &element);

	enum Action {
		OnFocused = 0// States switched when corresponding widget is focused
		, OnMouseOver // States switched on mouse over
		// TODO: Add action here
	};

private slots:
	void stateChanged(int const state);
	void produceSubtools();

private:
	Widget *produceSubtool() const;

	Action action() const;
	void setAction(Action action);

	void onConstructionEnabled();
	void onConstructionDisabled();

	int mCurrentState;
	Action mAction;
	TriggerWidget *mWidget;
	Widget *mState1Tool;
	Widget *mState2Tool;
	QWidget *mState1Widget;
	QWidget *mState2Widget;
	QGraphicsProxyWidget *mStateBoxTool;
	QGraphicsLinearLayout *mLayout;
};

}
}
