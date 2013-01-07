#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QKeyEvent>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtXml/QDomDocument>

#include "widgetsEditor.h"
#include "ui_widgetsEditor.h"
#include "private/toolFactory.h"
#include "private/editorScene.h"
#include "private/toolList.h"
#include "tools/root.h"
#include "tools/layoutHelpers/layoutHelperFactory.h"

#include "../../../../qrutils/outFile.h"
#include "../../../../qrutils/inFile.h"
#include "../../../../qrutils/xmlUtils.h"

using namespace qReal::widgetsEdit;

WidgetsEditor::WidgetsEditor(bool isIconEditor, QWidget *parent)
	: QWidget(parent), NavigationPageWithMenu()
	, mIsIconEditor(isIconEditor)
	, mUi(new Ui::WidgetsEditor), mController(NULL)
{
	initComponents();
}

WidgetsEditor::~WidgetsEditor()
{
	delete mUi;
}

void WidgetsEditor::onShown(navigation::NavigationState *state)
{
	NavigationPageWithMenu::onShown(state);
	mDocumentBuilder = state->state<qReal::elementEdit::TemplateDocumentBuilder *>();
	QDomDocument wtf = mIsIconEditor
			? mDocumentBuilder->iconWtf()
			: mDocumentBuilder->wtf();
	if (wtf.childNodes().count() == 0) {
		if (mIsIconEditor) {
			mDocumentBuilder->mergeIconWidgetGraphics(mEmptyCaseWtf);
		} else {
			mDocumentBuilder->mergeWidgetGraphics(mEmptyCaseWtf);
		}
	}
	mDocumentBuilder->toGraphics(wtf);
	load(wtf);
}

QWidget *WidgetsEditor::deserializeWidget(QDomDocument const &document)
{
	QDomElement widgetTemplate = document.firstChildElement("widget-template");
	return deserializeWidget(widgetTemplate);
}

QWidget *WidgetsEditor::deserializeWidget(QDomElement const &widgetTemplate)
{
	QDomElement rootElement = widgetTemplate.firstChild().toElement();
	return ToolFactory::instance()->deserializeWidget(rootElement);
}

void WidgetsEditor::load(QDomDocument const &graphics)
{
	QDomDocument const loadableGraphics = graphics.childNodes().count() == 0
			? mEmptyCaseWtf
			: graphics;
	if (mRoot) {
		mScene->removeItem(mRoot);
		delete mRoot;
		mRoot = NULL;
	}
	initController();
	initRoot(ToolFactory::instance()->loadDocument(mController, loadableGraphics));
	if (mIsIconEditor) {
		mDocumentBuilder->mergeIconDocument(graphics);
	} else {
		mDocumentBuilder->mergeDocument(graphics);
	}
}

void WidgetsEditor::keyPressEvent(QKeyEvent *event)
{
	mController->processKeyEvent(event);
}

void WidgetsEditor::initComponents()
{
	mUi->setupUi(this);
	initController();
	initLayoutButtons();
	initControlButtons();
	initScene();
	loadTools();
	initPropertyBrowser();
	initRoot();
	initEmptyCase();
}

void WidgetsEditor::initController()
{
	if (mController) {
		delete mController;
	}
	mController = new ToolController;
	connect(mController, SIGNAL(selectionChanged(Tool*)),
			this, SLOT(onSelectionChanged(Tool*)));
	connect(mController, SIGNAL(savePressed()), this, SLOT(save()));
	connect(mController, SIGNAL(saveAsPressed()), this, SLOT(saveToDisk()));
	connect(mController, SIGNAL(openPressed()), this, SLOT(loadFromDisk()));
}

void WidgetsEditor::initLayoutButtons()
{
	mLayoutButtons = new LayoutButtons;
	connect(mLayoutButtons, SIGNAL(buttonClicked(LayoutType)),
			this, SLOT(onLayoutButtonClicked(LayoutType)));
}

void WidgetsEditor::initControlButtons()
{
	mControlButtons = new qReal::elementEdit::ControlButtons(false, mIsIconEditor);
	connect(mControlButtons, SIGNAL(saveClicked()), this, SLOT(save()));
	connect(mControlButtons, SIGNAL(saveToDiskClicked()), this, SLOT(saveToDisk()));
	connect(mControlButtons, SIGNAL(loadFromDiskClicked()), this, SLOT(loadFromDisk()));
	connect(mControlButtons, SIGNAL(previewClicked()), this, SLOT(preview()));
	connect(mControlButtons, SIGNAL(shapeClicked()), this, SLOT(save()));
	connect(mControlButtons, SIGNAL(iconAccepted()), this, SLOT(save()));

	QListIterator<QPushButton *> buttonsIterator =
			mLayoutButtons->buttonsIterator();
	int index = 0;
	while (buttonsIterator.hasNext()) {
		mControlButtons->insertButton(buttonsIterator.next(), index++);
	}
	setMenuContent(mControlButtons);
}

void WidgetsEditor::initScene()
{
	mUi->editorArea->setScene(new EditorScene(mController));
}

void WidgetsEditor::loadTools()
{
	ToolList *toolList = new ToolList(this);
	connect(toolList, SIGNAL(keyPressed(QKeyEvent*)), mController, SLOT(processKeyEvent(QKeyEvent*)));
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
	initRoot(ToolFactory::instance()->makeRoot(mController));
}

void WidgetsEditor::initRoot(Root *root)
{
	mScene = new QGraphicsScene;
	mUi->editorArea->setScene(mScene);
	mRoot = root;
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

void WidgetsEditor::onSelectionChanged(Tool *tool)
{
	switchLayoutButtonsActiveState(tool);
}

void WidgetsEditor::serializeWidget(QDomDocument &target)
{
	QDomElement graphicsElement = target.createElement("graphics");
	QDomElement widgetTemplateElement = target.createElement("widget-template");
	QDomElement rootElement = target.createElement(mRoot->title());
	mRoot->generateXml(rootElement, target);
	widgetTemplateElement.appendChild(rootElement);
	graphicsElement.appendChild(widgetTemplateElement);
	target.appendChild(graphicsElement);
}

void WidgetsEditor::save()
{
	QDomDocument document;
	serializeWidget(document);
	if (mIsIconEditor) {
		mDocumentBuilder->mergeIconWidgetGraphics(document);
	} else {
		mDocumentBuilder->mergeWidgetGraphics(document);
	}

	emit widgetSaved();
}

void WidgetsEditor::saveToDisk()
{
	QString path = QFileDialog::getSaveFileName(this
			, tr("Save as"), QString()
			, tr("Widget template format files") + "(*.wtf);;" + tr("All Files") + "(*.*)");
	if (path.isEmpty()) {
		return;
	}
	if (!path.endsWith(".wtf", Qt::CaseInsensitive)) {
		path.append(".wtf");
	}

	save();

	QString const xml = mDocumentBuilder->buildTemplate().toString(4);
	utils::OutFile file(path);
	file() << xml;
	QMessageBox::information(this, tr("Widgets Editor")
		, tr("Saved successfully!"), QMessageBox::Ok);
}

void WidgetsEditor::loadFromDisk()
{
	QString const path = QFileDialog::getOpenFileName(this
			, tr("Open"), QString()
			, tr("Widget template format files") + "(*.wtf);;" + tr("All Files") + "(*.*)");
	if (path.isEmpty()) {
		return;
	}

	QDomDocument document;
	document.setContent(utils::InFile::readAll(path));
	if (!document.isNull()) {
		load(document);
	}
}

void WidgetsEditor::preview()
{
	QDomDocument document;
	serializeWidget(document);
	if (document.isNull()) {
		return;
	}
	QDomElement graphics = document.firstChildElement("graphics");
	QDomElement widgetTemplate = graphics.firstChildElement("widget-template");
	QWidget *widget = WidgetsEditor::deserializeWidget(widgetTemplate);
	preview(widget);
}

void WidgetsEditor::preview(QWidget *widget)
{
	if (!widget) {
		return;
	}
	QDialog *dialog = new QDialog(this);
	dialog->setGeometry(widget->geometry());
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
	layout->addWidget(widget);
	dialog->setLayout(layout);
	dialog->exec();
	delete dialog;
}

QDomDocument WidgetsEditor::currentTemplate()
{
	QDomDocument result;
	serializeWidget(result);
	return result;
}

qReal::elementEdit::ControlButtons *WidgetsEditor::controlButtons() const
{
	return mControlButtons;
}

void WidgetsEditor::initEmptyCase()
{
	serializeWidget(mEmptyCaseWtf);
}
