#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QKeyEvent>
#include <QtGui/QFileDialog>
#include <QtXml/QDomDocument>

#include "widgetsEditor.h"
#include "ui_widgetsEditor.h"
#include "private/toolFactory.h"
#include "private/editorScene.h"
#include "private/toolList.h"
#include "tools/root.h"
#include "tools/layoutHelpers/layoutHelperFactory.h"

#include "../../../../qrutils/outFile.h"
#include "../../../../qrutils/xmlUtils.h"

WidgetsEditor::WidgetsEditor(const QPersistentModelIndex &index
		, const int &role, QWidget *parent)
	: QWidget(parent), mUi(new Ui::WidgetsEditor), mIndex(index), mRole(role)
{
	initComponents();
}

WidgetsEditor::WidgetsEditor(QWidget *parent)
	: QWidget(parent), mUi(new Ui::WidgetsEditor), mRole(0)
{
	initComponents();
}

WidgetsEditor::~WidgetsEditor()
{
	delete mUi;
}

void WidgetsEditor::setShape(QDomDocument const &shape)
{
	mRoot->setShape(shape);
}

QWidget *WidgetsEditor::deserializeWidget(QDomDocument const &document)
{
	QDomElement widgetTemplateElement = document.documentElement();
	QDomElement rootElement = widgetTemplateElement.firstChild().toElement();
	QDomElement shapeElement = rootElement.nextSiblingElement();
	return shapeElement.isNull()
			? Ui::WidgetsEdit::ToolFactory::instance()->deserializeWidget(rootElement)
			: Ui::WidgetsEdit::ToolFactory::instance()->deserializeWidget(rootElement, shapeElement);
}

void WidgetsEditor::keyPressEvent(QKeyEvent *event)
{
	if (event->key() != Qt::Key_Delete || event->modifiers() != Qt::NoModifier) {
		event->ignore();
		return;
	}
	event->accept();
	if (mController->selectedItem() != mRoot) {
		mController->removeChild(mController->selectedItem());
	}
}

void WidgetsEditor::initComponents()
{
	mUi->setupUi(this);
	initController();
	initLayoutButtons();
	initShapeButton();
	initSaveButton();
	initScene();
	loadTools();
	initPropertyBrowser();
	initRoot();
}

void WidgetsEditor::initController()
{
	mController = new ToolController;
	connect(mController, SIGNAL(selectionChanged(Tool*)),
			this, SLOT(onSelectionChanged(Tool*)));
}

void WidgetsEditor::initLayoutButtons()
{
	QHBoxLayout *layoutFrameLayout = new QHBoxLayout;
	layoutFrameLayout->setSpacing(0);
	layoutFrameLayout->setMargin(0);

	mLayoutButtons = new LayoutButtons;
	connect(mLayoutButtons, SIGNAL(buttonClicked(LayoutType)),
			this, SLOT(onLayoutButtonClicked(LayoutType)));

	QListIterator<QPushButton *> buttonsIterator =
			mLayoutButtons->buttonsIterator();
	while (buttonsIterator.hasNext()) {
		layoutFrameLayout->addWidget(buttonsIterator.next());
	}
	layoutFrameLayout->addStretch(10);

	delete mUi->layoutFrame->layout();
	mUi->layoutFrame->setLayout(layoutFrameLayout);
}

void WidgetsEditor::initShapeButton()
{
	mShapeButton = new QPushButton("Shape");
	connect(mShapeButton, SIGNAL(clicked()),
			this, SLOT(onShapeButtonClicked()));
	mUi->layoutFrame->layout()->addWidget(mShapeButton);
}

void WidgetsEditor::initSaveButton()
{
	QPushButton *saveButton = new QPushButton("Save");
	connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
	mUi->layoutFrame->layout()->addWidget(saveButton);
}

void WidgetsEditor::initScene()
{
	mUi->editorArea->setScene(new EditorScene(mController));
}

void WidgetsEditor::loadTools()
{
	ToolList *toolList = new ToolList(this);
	mUi->toolDock->setWidget(toolList);	
}

void WidgetsEditor::initPropertyBrowser()
{
	QtTreePropertyBrowser *browser = new QtTreePropertyBrowser;
	mUi->toolFrame->layout()->addWidget(browser);
	PropertyBrowserController *controller = new PropertyBrowserController(browser);
	mController->setPropertyBrowserController(controller);
}

void WidgetsEditor::initRoot()
{
	mScene = new QGraphicsScene;
	mUi->editorArea->setScene(mScene);
	mRoot = ToolFactory::instance()->makeRoot(mController);
	mController->addChild(mRoot);
	mScene->addItem(mRoot);
}

void WidgetsEditor::switchLayoutButtonsActiveState(Tool *tool)
{
	LayoutTool *layoutTool = dynamic_cast<LayoutTool *>(tool);
	if (!layoutTool) {
		mLayoutButtons->disableAllButtons();
		return;
	}
	LayoutType type = layoutTool->layoutFactory()->currentType();
	mLayoutButtons->enableAllButtonsExcept(type);
}

void WidgetsEditor::onLayoutButtonClicked(LayoutType type)
{
	mController->setLayout(mController->selectedItem(), type);
	mLayoutButtons->enableAllButtonsExcept(type);
}

void WidgetsEditor::onShapeButtonClicked()
{
	emit shapeRequested();
}

void WidgetsEditor::onSelectionChanged(Tool *tool)
{
	switchLayoutButtonsActiveState(tool);
	mShapeButton->setEnabled(tool == mRoot);
}

void WidgetsEditor::save()
{
	QString loadPath = "/home/dvvrd/dev/qreal/widgets/test.wtf";

	QDomDocument document1 = utils::xmlUtils::loadDocument(loadPath);
	if (document1.isNull()) {
		return;
	}
	QWidget *widget = WidgetsEditor::deserializeWidget(document1);

/*	QDialog dialog;
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
	layout->addWidget(widget);
	dialog.setLayout(layout);
	widget->show();
	widget->setVisible(true);
	dialog.exec();*/

	QGraphicsScene *scene =  mUi->editorArea->scene();
	scene->clear();
	QGraphicsProxyWidget *grWidget = scene->addWidget(widget);
	return;

/*	QString path = QFileDialog::getSaveFileName(this,
							tr("Save as"), QString(),
							"Widget template format files (*.wtf)"
							+ QString(";;AllFiles (*.*)"));
	if (path == "")
		return;*/
	QDomDocument document;
	QDomElement widgetTemplateElement = document.createElement("widget-template");
	QDomElement rootElement = document.createElement(mRoot->title());
	mRoot->generateXml(rootElement, document);
	widgetTemplateElement.appendChild(rootElement);
	document.appendChild(widgetTemplateElement);
	QDomDocument shapeDocument = mRoot->shapeDocument();
	QDomElement shapeElement = shapeDocument.documentElement();
	if (!shapeElement.isNull()) {
		widgetTemplateElement.appendChild(shapeElement);
	}

//	utils::OutFile file(path);
//	file() << document.toString(4);

	emit widgetSaved(document.toString(4), mIndex, mRole);

/*	QWidget *widget = deserializeWidget(document);
	QDialog dialog;
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
	layout->addWidget(widget);
	dialog.setLayout(layout);
	widget->show();
	widget->setVisible(true);
	dialog.exec();*/
}
