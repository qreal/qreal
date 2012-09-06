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
#include "../tools/propertyManagers/rootPropertyManager.h"
#include "../tools/propertyManagers/pushButtonPropertyManager.h"
#include "../tools/propertyManagers/radioButtonPropertyManager.h"
#include "../tools/propertyManagers/checkBoxPropertyManager.h"
#include "../tools/propertyManagers/groupBoxPropertyManager.h"
#include "../tools/propertyManagers/scrollAreaPropertyManager.h"
#include "../tools/propertyManagers/framePropertyManager.h"
#include "../tools/propertyManagers/widgetPropertyManager.h"
#include "../tools/propertyManagers/comboBoxPropertyManager.h"
#include "../tools/propertyManagers/lineEditPropertyManager.h"
#include "../tools/propertyManagers/plainTextEditPropertyManager.h"
#include "../tools/propertyManagers/spinBoxPropertyManager.h"
#include "../tools/propertyManagers/doubleSpinBoxPropertyManager.h"
#include "../tools/propertyManagers/labelPropertyManager.h"
#include "../tools/propertyManagers/spacerPropertyManager.h"

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
	if (title == "Push Button") {
		PushButton *pushButton = new PushButton(controller);
		PushButtonPropertyManager *manager
				= new PushButtonPropertyManager(pushButton);
		pushButton->setPropertyManager(manager);
		return pushButton;
	}
	if (title == "Radio Button") {
		RadioButton *radioButton = new RadioButton(controller);
		RadioButtonPropertyManager *manager
				= new RadioButtonPropertyManager(radioButton);
		radioButton->setPropertyManager(manager);
		return radioButton;
	}
	if (title == "Check Box") {
		CheckBox *checkBox = new CheckBox(controller);
		CheckBoxPropertyManager *manager
				= new CheckBoxPropertyManager(checkBox);
		checkBox->setPropertyManager(manager);
		return checkBox;
	}
	if (title == "Group Box") {
		GroupBox *groupBox = new GroupBox(controller);
		GroupBoxPropertyManager *manager
				= new GroupBoxPropertyManager(groupBox);
		groupBox->setPropertyManager(manager);
		return groupBox;
	}
	if (title == "Scroll Area") {
		ScrollArea *scrollArea = new ScrollArea(controller);
		ScrollAreaPropertyManager *manager
				= new ScrollAreaPropertyManager(scrollArea);
		scrollArea->setPropertyManager(manager);
		return scrollArea;
	}
	if (title == "Frame") {
		Frame *frame = new Frame(controller);
		FramePropertyManager *manager
				= new FramePropertyManager(frame);
		frame->setPropertyManager(manager);
		return frame;
	}
	if (title == "Widget") {
		Widget *widget = new Widget(controller);
		WidgetPropertyManager *manager
				= new WidgetPropertyManager(widget);
		widget->setPropertyManager(manager);
		return widget;
	}
	if (title == "Combo Box") {
		ComboBox *comboBox = new ComboBox(controller);
		ComboBoxPropertyManager *manager
				= new ComboBoxPropertyManager(comboBox);
		comboBox->setPropertyManager(manager);
		return comboBox;
	}
	if (title == "Line Edit") {
		LineEdit *lineEdit = new LineEdit(controller);
		LineEditPropertyManager *manager
				= new LineEditPropertyManager(lineEdit);
		lineEdit->setPropertyManager(manager);
		return lineEdit;
	}
	if (title == "Plain Text Edit") {
		PlainTextEdit *plainTextEdit = new PlainTextEdit(controller);
		PlainTextEditPropertyManager *manager
				= new PlainTextEditPropertyManager(plainTextEdit);
		plainTextEdit->setPropertyManager(manager);
		return plainTextEdit;
	}
	if (title == "Spin Box") {
		SpinBox *spinBox = new SpinBox(controller);
		SpinBoxPropertyManager *manager
				= new SpinBoxPropertyManager(spinBox);
		spinBox->setPropertyManager(manager);
		return spinBox;
	}
	if (title == "Double Spin Box") {
		DoubleSpinBox *doubleSpinBox = new DoubleSpinBox(controller);
		DoubleSpinBoxPropertyManager *manager
				= new DoubleSpinBoxPropertyManager(doubleSpinBox);
		doubleSpinBox->setPropertyManager(manager);
		return doubleSpinBox;
	}
	if (title == "Label") {
		Label *label = new Label(controller);
		LabelPropertyManager *manager
				= new LabelPropertyManager(label);
		label->setPropertyManager(manager);
		return label;
	}
	if (title == "Horizontal Spacer") {
		Spacer *spacer = new Spacer(Qt::Horizontal, controller);
		SpacerPropertyManager *manager
				= new SpacerPropertyManager(spacer);
		spacer->setPropertyManager(manager);
		return spacer;
	}
	if (title == "Vertical Spacer") {
		Spacer *spacer = new Spacer(Qt::Vertical, controller);
		SpacerPropertyManager *manager
				= new SpacerPropertyManager(spacer);
		spacer->setPropertyManager(manager);
		return spacer;
	}
	if (title == "Root") {
		return makeRoot(controller);
	}
	return NULL;
}

Root *ToolFactory::makeRoot(ToolController *controller) const
{
	Root *root = new Root(controller);
	RootPropertyManager *manager = new RootPropertyManager(root);
	root->setPropertyManager(manager);
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

QWidget *ToolFactory::deserializeWidget(const QDomElement &element
		, const QDomElement &shape)
{
	QWidget *result = deserializeWidget(NULL, element);
	RootWidget *root = dynamic_cast<RootWidget *>(result);

	QDomDocument shapeDocument;
	// to make newDocument non-null
	shapeDocument.createElement("");
	QDomElement newShape = shape.cloneNode(true).toElement();
	shapeDocument.appendChild(newShape);
	root->setShape(shapeDocument);

	return root;
}

QWidget *ToolFactory::deserializeWidget(QWidget *parent, const QDomElement &element)
{
	QString title = tagNameToToolTitle(element.tagName());
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
		QDomElement childElement = node.toElement();
		deserializeWidget(tool->widget(), childElement);
	}
	QWidget *result = tool->widget();
	result->setAttribute(Qt::WA_PaintOnScreen, false);
	// because result mustn`t be destroyed
	tool->setWidget(NULL);
	delete tool;
	return result;
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
