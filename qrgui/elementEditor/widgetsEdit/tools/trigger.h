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
	Q_ENUMS(State)
	Q_PROPERTY(Action action READ action WRITE setAction USER true DESIGNABLE true)
	Q_PROPERTY(State initialState READ initialState WRITE setInitialState USER true DESIGNABLE true)
	Q_PROPERTY(QString widget1 READ widget1Xml WRITE setWidget1Xml USER true DESIGNABLE false)
	Q_PROPERTY(QString widget2 READ widget2Xml WRITE setWidget2Xml USER true DESIGNABLE false)

public:
	Trigger(ToolController *controller);
	virtual ~Trigger() {}

	bool isUnderConstruction() const;

	virtual void generateXml(QDomElement &element, QDomDocument &document);
	virtual void deserializeWidget(QWidget *parent, QDomElement const &element);

	enum Action {
		OnFocused = 0// States switched when corresponding widget is focused
		, OnMouseOver // States switched on mouse over
		, OnF2Pressed // States switched in rename-style
		, OnMouseClick // State switched to active when mouse pressed
		// TODO: Add actions here
	};

	// TODO: use it in TriggerWidget
	enum State {
		Inactive = 0
		, Active
	};

private slots:
	void stateChanged(int const state);
	void produceSubtools();

private:
	Widget *produceSubtool() const;

	void onConstructionEnabled();
	void onConstructionDisabled();

	Action action() const;
	QString widget1Xml() const;
	QString widget2Xml() const;
	State initialState() const;

	void setAction(Action const action);
	void setWidget1Xml(QString const &xml);
	void setWidget2Xml(QString const &xml);
	void setInitialState(State const state);

	int mCurrentState;
	Action mAction;
	Widget *mState1Tool;
	Widget *mState2Tool;
	QGraphicsProxyWidget *mStateBoxTool;
	QGraphicsLinearLayout *mLayout;
	QString mWidget1Xml;
	QString mWidget2Xml;
	State mInitialState;
};

}
}
