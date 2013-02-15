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
	initTags();
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

Tool *ToolFactory::makeItem(QString const &tag, ToolController *controller)
{
	Tool *result = NULL;
	if (tag == "PushButton") {
		result = new PushButton(controller);
	}
	if (tag == "RadioButton") {
		result = new RadioButton(controller);
	}
	if (tag == "CheckBox") {
		result = new CheckBox(controller);
	}
	if (tag == "GroupBox") {
		result = new GroupBox(controller);
	}
	if (tag == "ScrollArea") {
		result = new ScrollArea(controller);
	}
	if (tag == "Frame") {
		result = new Frame(controller);
	}
	if (tag == "Widget") {
		result = new Widget(controller);
	}
	if (tag == "ComboBox") {
		result = new ComboBox(controller);
	}
	if (tag == "LineEdit") {
		result = new LineEdit(controller);
	}
	if (tag == "PlainTextEdit") {
		result = new PlainTextEdit(controller);
	}
	if (tag == "SpinBox") {
		result = new SpinBox(controller);
	}
	if (tag == "DoubleSpinBox") {
		result = new DoubleSpinBox(controller);
	}
	if (tag == "Label") {
		result = new Label(controller);
	}
	if (tag == "HorizontalSpacer") {
		result = new Spacer(Qt::Horizontal, controller);
	}
	if (tag == "VerticalSpacer") {
		result = new Spacer(Qt::Vertical, controller);
	}
	if (tag == "Trigger") {
		result = new Trigger(controller);
	}

	if (result) {
		result->onLoaded();
	}

	if (tag == "Root") {
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

QPixmap ToolFactory::widgetPixmap(QString const &tag)
{
	foreach (Tool *item, mItems) {
		if (item->tag() == tag) {
			return QPixmap::grabWidget(item->widget()
					, item->widget()->geometry());
		}
	}
	return QPixmap();
}

QWidget *ToolFactory::deserializeWidget(QDomElement const &element
		, QList<PropertyEditorInterface *> &editors)
{
	return deserializeWidget(NULL, element, editors);
}

QWidget *ToolFactory::deserializeWidget(QWidget *parent, const QDomElement &element
		, QList<PropertyEditorInterface *> &editors)
{
	QString const tag = element.tagName();
	if (tag.isEmpty()) {
		return NULL;
	}
	Tool *tool = makeItem(tag, NULL);
	if (!tool) {
		return NULL;
	}
	tool->deserializeWidget(parent, element, editors);
	QDomElement childElement = element.firstChildElement();
	while (!childElement.isNull()) {
		deserializeWidget(tool->widget(), childElement, editors);
		childElement = childElement.nextSiblingElement();
	}
	QWidget *result = tool->widget();
	if (parent) {
		result->setParent(parent);
	}

	PropertyEditorInterface *iface = dynamic_cast<PropertyEditorInterface *>(result);
	if (iface && !iface->binding().isEmpty()) {
		editors << iface;
	}

	// Returning ownership to us
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
	QString const tag = element.tagName();
	if (tag.isEmpty()) {
		return NULL;
	}
	Tool *tool = makeItem(tag, controller);
	if (!tool) {
		return NULL;
	}
	tool->load(parent, element);
	LayoutTool *nextParent = dynamic_cast<LayoutTool *>(tool);
	if (nextParent) {
		QDomElement childElement = element.firstChildElement();
		while (!childElement.isNull()) {
			loadElement(nextParent, childElement, controller);
			childElement = childElement.nextSiblingElement();
		}
	}
	return tool;
}

void ToolFactory::initTags()
{
	mTags << "PushButton";
	mTags << "RadioButton";
	mTags << "CheckBox";
	mTags << "GroupBox";
	mTags << "ScrollArea";
	mTags << "Frame";
	mTags << "Widget";
	mTags << "ComboBox";
	mTags << "LineEdit";
	mTags << "PlainTextEdit";
	mTags << "SpinBox";
	mTags << "DoubleSpinBox";
	mTags << "Label";
	mTags << "HorizontalSpacer";
	mTags << "VerticalSpacer";
	mTags << "Trigger";
}

void ToolFactory::initItems()
{
	foreach (QString const &tag, mTags) {
		mItems << makeItem(tag, NULL);
	}
}
