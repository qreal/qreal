#include <QtCore/QDebug>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>

#include "trigger.h"
#include "triggerWidget.h"
#include "../private/toolFactory.h"

using namespace qReal::widgetsEdit;

Trigger::Trigger(ToolController *controller)
	: Tool(new QFrame, controller)
{
	mIcon = QIcon(":/icons/widgetsEditor/trigger.png");
	mTitle = tr("Trigger");

	widget()->setGeometry(0, 0
		, TRIGGER_DEFAULT_WIDTH
		, TRIGGER_DEFAULT_HEIGHT);

	// Qt, WHF??? Without this line it DOES NOT WORK!!!! AHTUNGGG!!
	widget()->setMinimumSize(2, 2);

	connect(this, SIGNAL(loaded()), this, SLOT(produceSubtools()));

	QComboBox *stateBox = new QComboBox;
	stateBox->addItem(tr("State1"));
	stateBox->addItem(tr("State2"));
	connect(stateBox, SIGNAL(activated(int)), this, SLOT(stateChanged(int)));
	mStateBoxTool = new QGraphicsProxyWidget;
	mStateBoxTool->setWidget(stateBox);

	mLayout = new QGraphicsLinearLayout(Qt::Vertical);
	mLayout->addItem(mStateBoxTool);
	setLayout(mLayout);
}

void Trigger::produceSubtools()
{
	mState1Tool = produceSubtool();
	mState2Tool = produceSubtool();
	mState1Widget = mState1Tool->widget();
	mState2Widget = mState2Tool->widget();

	stateChanged(0);
}

Widget *Trigger::produceSubtool() const
{
	// TODO: Understand why calling ToolFactory::instance() causes an infinite loop.
	// Use ToolFactory::instance()->makeItem(...) instead of next two lines
	Widget *result = new Widget(mController);
	result->onLoaded();
	result->setMovable(false);
	result->setResizable(false);
	if (mController) {
		mController->addChild(result);
	}

	return result;
}

Trigger::Action Trigger::action() const
{
	return mAction;
}

void Trigger::setAction(Action action)
{
	mAction = action;
}

void Trigger::stateChanged(int const state)
{
	if (mCurrentState == state) {
		return;
	}
	Widget *oldWidget = (mCurrentState == 0) ? mState1Tool : mState2Tool;
	mCurrentState = state;
	Widget *currentWidget = (mCurrentState == 0) ? mState1Tool : mState2Tool;
	if (mLayout->count() > 1) {
		mLayout->removeAt(1);
	}
	oldWidget->setParent(NULL);
	oldWidget->setParentItem(NULL);
	mLayout->removeItem(oldWidget);
	oldWidget->hide();
	currentWidget->setParent(this);
	currentWidget->setParentItem(this);
	mLayout->insertItem(1, currentWidget);
	currentWidget->show();
}

void Trigger::deserializeWidget(QWidget *parent, QDomElement const &element)
{
	setWidget(new TriggerWidget);
	Tool::deserializeWidget(parent, element);
}
