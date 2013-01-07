#include "toolFactory.h"
#include "../tools/root.h"
#include "../tools/pushButton.h"
#include "../tools/radioButton.h"
#include "../tools/checkBox.h"
#include "../tools/groupBox.h"
#include "../tools/scrollArea.h"
#include "../tools/frame.h"
#include "../tools/widget.h"
#include "../tools/comboBox.h"
#include "../tools/lineEdit.h"
#include "../tools/plainTextEdit.h"
#include "../tools/spinBox.h"
#include "../tools/doubleSpinBox.h"
#include "../tools/label.h"
#include "../tools/spacer.h"
#include "../tools/trigger.h"
#include "../tools/triggerWidget.h"

using namespace qReal::widgetsEdit;

ToolFactory::ToolFactory()
{
	initTitles();
	initItems();
}

ToolFactory *ToolFactory::instance()
{
	static ToolFactory instance;
	return &instance;
}

QListIterator<Tool *> ToolFactory::itemsIterator()
{
	return QListIterator<Tool *>(mItems);
}

Tool *ToolFactory::makeItem(const QString &title, ToolController *controller)
{
	Tool *result = NULL;
	if (title == "Push Button") {
		result = new PushButton(controller);
	}
	if (title == "Radio Button") {
		result = new RadioButton(controller);
	}
	if (title == "Check Box") {
		result = new CheckBox(controller);
	}
	if (title == "Group Box") {
		result = new GroupBox(controller);
	}
	if (title == "Scroll Area") {
		result = new ScrollArea(controller);
	}
	if (title == "Frame") {
		result = new Frame(controller);
	}
	if (title == "Widget") {
		result = new Widget(controller);
	}
	if (title == "Combo Box") {
		result = new ComboBox(controller);
	}
	if (title == "Line Edit") {
		result = new LineEdit(controller);
	}
	if (title == "Plain Text Edit") {
		result = new PlainTextEdit(controller);
	}
	if (title == "Spin Box") {
		result = new SpinBox(controller);
	}
	if (title == "Double Spin Box") {
		result = new DoubleSpinBox(controller);
	}
	if (title == "Label") {
		result = new Label(controller);
	}
	if (title == "Horizontal Spacer") {
		result = new Spacer(Qt::Horizontal, controller);
	}
	if (title == "Vertical Spacer") {
		result = new Spacer(Qt::Vertical, controller);
	}
	if (title == "Trigger") {
		result = new Trigger(controller);
	}

	if (result) {
		result->onLoaded();
	}

	if (title == "Root") {
		result = makeRoot(controller);
	}
	if (controller != NULL && result != NULL) {
		controller->addChild(result);
	}
	return result;
}

Root *ToolFactory::makeRoot(ToolController *controller) const
{
	Root *root = new Root(controller);
	root->onLoaded();
	return root;
}

QPixmap ToolFactory::widgetPixmap(QString const &title)
{
	foreach (Tool *item, mItems) {
		if (item->title() == title) {
			return QPixmap::grabWidget(item->widget()
					, item->widget()->geometry());
		}
	}
	return QPixmap();
}

QWidget *ToolFactory::deserializeWidget(const QDomElement &element)
{
	return deserializeWidget(NULL, element);
}

QWidget *ToolFactory::deserializeWidget(QWidget *parent, const QDomElement &element)
{
	QString const title = tagNameToToolTitle(element.tagName());
	if (title.isEmpty()) {
		return NULL;
	}
	Tool *tool = makeItem(title, NULL);
	if (!tool) {
		return NULL;
	}
	tool->deserializeWidget(parent, element);
	for (int i = 0; i < element.childNodes().count(); ++i) {
		QDomNode node = element.childNodes().at(i);
		if (!node.isNull()) {
			QDomElement childElement = node.toElement();
			deserializeWidget(tool->widget(), childElement);
		}
	}
	QWidget *result = tool->widget();
	if (parent) {
		result->setParent(parent);
	}
	result->setVisible(true);
	// because result mustn`t be destroyed
	tool->setWidget(NULL);
	delete tool;
	return result;
}

Root *ToolFactory::loadDocument(ToolController *controller
		, QDomDocument const &document)
{
	QDomElement graphics = document.firstChildElement("graphics");
	QDomElement widgetTemplate = graphics.firstChildElement("widget-template");
	QDomElement const rootElement = widgetTemplate.firstChildElement("Root");
	return dynamic_cast<Root *>(loadElement(NULL, rootElement, controller));
}

Tool *ToolFactory::loadElement(LayoutTool *parent, const QDomElement &element
		, ToolController *controller)
{
	// TODO: reuse deserializeWidget() mechanism
	QString const title = tagNameToToolTitle(element.tagName());
	if (title.isEmpty()) {
		return NULL;
	}
	Tool *tool = makeItem(title, controller);
	if (!tool) {
		return NULL;
	}
	tool->load(parent, element);
	LayoutTool *nextParent = dynamic_cast<LayoutTool *>(tool);
	if (nextParent) {
		for (int i = 0; i < element.childNodes().count(); ++i) {
			QDomNode node = element.childNodes().at(i);
			if (!node.isNull()) {
				QDomElement childElement = node.toElement();
				loadElement(nextParent, childElement, controller);
			}
		}
	}
	return tool;
}

void ToolFactory::initTitles()
{
	//first value is everywhere-used title, second used for serialzation
	mTitles << TitleTagPair("Push Button", "PushButton");
	mTitles << TitleTagPair("Radio Button", "RadioButton");
	mTitles << TitleTagPair("Check Box", "CheckBox");
	mTitles << TitleTagPair("Group Box", "GroupBox");
	mTitles << TitleTagPair("Scroll Area", "ScrollArea");
	mTitles << TitleTagPair("Frame", "Frame");
	mTitles << TitleTagPair("Widget", "Widget");
	mTitles << TitleTagPair("Combo Box", "ComboBox");
	mTitles << TitleTagPair("Line Edit", "LineEdit");
	mTitles << TitleTagPair("Plain Text Edit", "PlainTextEdit");
	mTitles << TitleTagPair("Spin Box", "SpinBox");
	mTitles << TitleTagPair("Double Spin Box", "DoubleSpinBox");
	mTitles << TitleTagPair("Label", "Label");
	mTitles << TitleTagPair("Horizontal Spacer", "HorizontalSpacer");
	mTitles << TitleTagPair("Vertical Spacer", "VerticalSpacer");
	mTitles << TitleTagPair("Trigger", "Trigger");
}

void ToolFactory::initItems()
{
	foreach (TitleTagPair const &titlePair, mTitles) {
		mItems << makeItem(titlePair.title, NULL);
	}
}

QString ToolFactory::toolTitleToTagName(const QString &title) const
{
	foreach (TitleTagPair const &titlePair, mTitles) {
		if (titlePair.title == title) {
			return titlePair.tagName;
		}
	}
	return QString();
}

QString ToolFactory::tagNameToToolTitle(const QString &tagName) const
{
	foreach (TitleTagPair const &titlePair, mTitles) {
		if (titlePair.tagName == tagName) {
			return titlePair.title;
		}
	}
	if (tagName == "Root") {
		return tagName;
	}
	return QString();
}
