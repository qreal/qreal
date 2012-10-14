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

using namespace qReal::widgetsEdit;

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
	return ToolFactory::instance()->deserializeWidget(rootElement);
}

void WidgetsEditor::load(QDomDocument const &widgetTemplate)
{
	Q_UNUSED(widgetTemplate)
	// TODO: IMPLEMENT IT!!!
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
	initPreviewButton();
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
	mShapeButton = new QPushButton(tr("Shape"));
	connect(mShapeButton, SIGNAL(clicked()),
			this, SLOT(onShapeButtonClicked()));
	mUi->layoutFrame->layout()->addWidget(mShapeButton);
}

void WidgetsEditor::initSaveButton()
{
	QPushButton *saveButton = new QPushButton(QIcon(":icons/widgetsEditor/save.png"), "");
	saveButton->setToolTip(tr("Save as XML"));
	saveButton->setContentsMargins(0,0,0,0);
	connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
	mUi->layoutFrame->layout()->addWidget(saveButton);
}

void WidgetsEditor::initPreviewButton()
{
	QPushButton *previewButton = new QPushButton(QIcon(":icons/preview.png"), "");
	previewButton->setToolTip(tr("Preview widget"));
	previewButton->setContentsMargins(0,0,0,0);
	connect(previewButton, SIGNAL(clicked()), this, SLOT(preview()));
	mUi->layoutFrame->layout()->addWidget(previewButton);
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

void WidgetsEditor::onLayoutButtonClicked(const LayoutType type)
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

void WidgetsEditor::serializeWidget(QDomDocument &target)
{
	QDomElement widgetTemplateElement = target.createElement("widget-template");
	QDomElement rootElement = target.createElement(mRoot->title());
	mRoot->generateXml(rootElement, target);
	widgetTemplateElement.appendChild(rootElement);
	target.appendChild(widgetTemplateElement);
}

void WidgetsEditor::save()
{
	QString const path = QFileDialog::getSaveFileName(this
			, tr("Save as"), QString()
			, tr("Widget template format files") + "(*.wtf);;" + tr("All Files") + "(*.*)");
	if (path.isEmpty()) {
		return;
	}

	QDomDocument document;
	serializeWidget(document);

	QString const xml = document.toString(4);
	utils::OutFile file(path);
	file() << xml;
	emit widgetSaved(xml, mIndex, mRole);
}

void WidgetsEditor::preview()
{
	QDomDocument document;
	serializeWidget(document);
	if (document.isNull()) {
		return;
	}
	QWidget *widget = WidgetsEditor::deserializeWidget(document);
	if (widget) {
		preview(widget);
	}
}

void WidgetsEditor::preview(QWidget *widget)
{
	QDialog *dialog = new QDialog(this);
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
	layout->addWidget(widget);
	dialog->setLayout(layout);
	dialog->exec();
	delete dialog;
}
