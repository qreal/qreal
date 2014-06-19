#include "toolFactory.h"
#include <qrgui/elementEditor/widgetsEdit/tools/root.h>
#include <qrgui/elementEditor/widgetsEdit/tools/pushButton.h>
#include <qrgui/elementEditor/widgetsEdit/tools/radioButton.h>
#include <qrgui/elementEditor/widgetsEdit/tools/checkBox.h>
#include <qrgui/elementEditor/widgetsEdit/tools/groupBox.h>
#include <qrgui/elementEditor/widgetsEdit/tools/scrollArea.h>
#include <qrgui/elementEditor/widgetsEdit/tools/frame.h>
#include <qrgui/elementEditor/widgetsEdit/tools/widget.h>
#include <qrgui/elementEditor/widgetsEdit/tools/comboBox.h>
#include <qrgui/elementEditor/widgetsEdit/tools/lineEdit.h>
#include <qrgui/elementEditor/widgetsEdit/tools/plainTextEdit.h>
#include <qrgui/elementEditor/widgetsEdit/tools/spinBox.h>
#include <qrgui/elementEditor/widgetsEdit/tools/doubleSpinBox.h>
#include <qrgui/elementEditor/widgetsEdit/tools/labelTool.h>
#include <qrgui/elementEditor/widgetsEdit/tools/spacer.h>
#include <qrgui/elementEditor/widgetsEdit/tools/trigger.h>
#include <qrgui/elementEditor/widgetsEdit/tools/triggerWidget.h>

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
		result = new LabelTool(controller);
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
	QPointF a = tool->pos();
	tool->setWidget(NULL);
	if (parent) {
		result->setParent(parent);
		result->move(a.toPoint());
	}
	result->show();

	PropertyEditorInterface *iface = dynamic_cast<PropertyEditorInterface *>(result);
	if (iface && !iface->binding().isEmpty()) {
		editors << iface;
	}

	// Returning ownership to us
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
