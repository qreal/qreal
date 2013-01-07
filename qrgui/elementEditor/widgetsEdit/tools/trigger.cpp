#include <QtCore/QDebug>
#include <QtXml/QDomDocument>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>

#include "trigger.h"
#include "triggerWidget.h"
#include "../private/toolFactory.h"

using namespace qReal::widgetsEdit;

Trigger::Trigger(ToolController *controller)
	: Tool(new QFrame, controller)
{
	mTitle = tr("Trigger");
	mTag = "Trigger";
	mIcon = QIcon(":/icons/widgetsEditor/trigger.png");

	widget()->setGeometry(0, 0
		, TRIGGER_DEFAULT_WIDTH
		, TRIGGER_DEFAULT_HEIGHT);

	// Qt WTF??? Without this line it DOES NOT WORK!!!! AHTUNGGG!!
	widget()->setMinimumSize(2, 2);

	connect(this, SIGNAL(loaded()), this, SLOT(produceSubtools()));

	QComboBox *stateBox = new QComboBox;
	stateBox->addItem(tr("Active State"));
	stateBox->addItem(tr("Inactive State"));
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
	stateChanged(0);
}

Tool *Trigger::produceSubtool() const
{
	// TODO: Understand why calling ToolFactory::instance() causes an infinite loop.
	// Use ToolFactory::instance()->makeItem(...) instead of next two lines
	Tool *result = new Widget(mController);
	setUpSubtool(result);

	return result;
}

void Trigger::setUpSubtool(Tool *tool) const
{
	tool->onLoaded();
	tool->setMovable(false);
	tool->setResizable(false);
	if (mController) {
		mController->addChild(tool);
	}
}

Trigger::Action Trigger::action() const
{
	return mAction;
}

void Trigger::setAction(Action const action)
{
	mAction = action;
}

void Trigger::stateChanged(int const state)
{
	if (mCurrentState == state) {
		return;
	}
	Tool *oldWidget = (mCurrentState == 0) ? mState1Tool : mState2Tool;
	mCurrentState = state;
	Tool *currentWidget = (mCurrentState == 0) ? mState1Tool : mState2Tool;
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

QString Trigger::widget1Xml() const
{
	return mWidget1Xml;
}

QString Trigger::widget2Xml() const
{
	return mWidget2Xml;
}

Trigger::State Trigger::initialState() const
{
	return mInitialState;
}

void Trigger::setWidget1Xml(QString const &xml)
{
	mWidget1Xml = xml;
}

void Trigger::setWidget2Xml(QString const &xml)
{
	mWidget2Xml = xml;
}

void Trigger::setInitialState(State const state)
{
	mInitialState = state;
}

void Trigger::generateXml(QDomElement &element, QDomDocument &document)
{
	QDomDocument widget1Doc;
	QDomDocument widget2Doc;
	QDomElement widget1Element = widget1Doc.createElement(mState1Tool->title());
	QDomElement widget2Element = widget2Doc.createElement(mState2Tool->title());
	mState1Tool->generateXml(widget1Element, widget1Doc);
	mState2Tool->generateXml(widget2Element, widget2Doc);
	widget1Doc.appendChild(widget1Element);
	widget2Doc.appendChild(widget2Element);
	setWidget1Xml(widget1Doc.toString(4));
	setWidget2Xml(widget2Doc.toString(4));
	// Temporary removing children (else they will be serialized)
	childItems().removeOne(mState1Tool);
	childItems().removeOne(mState2Tool);
	childItems().removeOne(mStateBoxTool);
	mState1Tool->setParentItem(NULL);
	mState2Tool->setParentItem(NULL);
	mStateBoxTool->setParentItem(NULL);
	Tool::generateXml(element, document);
	// Restoring childen
	mState1Tool->setParentItem(this);
	mState2Tool->setParentItem(this);
	mStateBoxTool->setParentItem(this);
	mLayout->insertItem(0, mStateBoxTool);
	stateChanged(mCurrentState);
	// TODO: Implement hierarchical children ignoring
}

void Trigger::deserializeWidget(QWidget *parent, QDomElement const &element)
{
	TriggerWidget *triggerWidget = new TriggerWidget;
	setWidget(triggerWidget);

	Tool::deserializeWidget(parent, element);
	triggerWidget->setAction(mAction);
	triggerWidget->changeState(Inactive);
	QString const xml1 = widget1Xml();
	QString const xml2 = widget2Xml();
	QDomDocument widget1Doc;
	QDomDocument widget2Doc;
	widget1Doc.setContent(xml1);
	widget2Doc.setContent(xml2);
	QDomElement const element1 = widget1Doc.childNodes().at(0).toElement();
	QDomElement const element2 = widget2Doc.childNodes().at(0).toElement();
	QWidget *widget1 = ToolFactory::instance()->deserializeWidget(element1);
	QWidget *widget2 = ToolFactory::instance()->deserializeWidget(element2);
	triggerWidget->setWidgets(widget1, widget2);
}

void Trigger::load(LayoutTool *parent, const QDomElement &element)
{
	if (mState1Tool) {
		mController->removeChild(mState1Tool);
		mState1Tool = NULL;
	}
	if (mState2Tool) {
		mController->removeChild(mState2Tool);
		mState2Tool = NULL;
	}
	Tool::load(parent, element);
	QString const xml1 = widget1Xml();
	QString const xml2 = widget2Xml();
	QDomDocument widget1Doc;
	QDomDocument widget2Doc;
	widget1Doc.setContent(xml1);
	widget2Doc.setContent(xml2);
	QDomElement const element1 = widget1Doc.childNodes().at(0).toElement();
	QDomElement const element2 = widget2Doc.childNodes().at(0).toElement();
	mState1Tool = ToolFactory::instance()->loadElement(NULL, element1, mController);
	mState2Tool = ToolFactory::instance()->loadElement(NULL, element2, mController);
	mCurrentState = -1;
	int const numericalState = mInitialState == Active ? 0 : 1;
	dynamic_cast<QComboBox *>(mStateBoxTool->widget())->setCurrentIndex(numericalState);
	stateChanged(numericalState);
	setUpSubtool(mState1Tool);
	setUpSubtool(mState2Tool);
}
