#include "elementEditor.h"
#include "common/startPage.h"

using namespace qReal;
using namespace qReal::elementEdit;
using namespace navigation;

ElementEditor::ElementEditor(const QPersistentModelIndex &index
		, const int &role, QWidget *parent)
	: NavigationView(QMap<NavigationPageInterface*, QWidget *>()
			, new NavigationController(new NavigationState), parent)
	, mIndex(index), mRole(role), mWidgetsEditor(NULL), mShapeEditor(NULL)
	, mIconWidgetsEditor(NULL), mIconShapeEditor(NULL), mDocumentBuilder(NULL)
{
	enableMenuSupport();
    qReal::elementEdit::StartPage *startPage = new qReal::elementEdit::StartPage;
	initStartPageInteraction(startPage);
	mPages.insert(startPage, startPage);
	mController->switchTo(startPage);
	load(QDomDocument());
}

ElementEditor::~ElementEditor()
{
}

ShapeEdit *ElementEditor::shapeEditor() const
{
	return mShapeEditor;
}

widgetsEdit::WidgetsEditor *ElementEditor::widgetEditor() const
{
	return mWidgetsEditor;
}

void ElementEditor::initStartPageInteraction(qReal::elementEdit::StartPage *startPage)
{
	connect(startPage, SIGNAL(widgetSelected()), this, SLOT(switchToWidgetsEditor()));
	connect(startPage, SIGNAL(shapeSelected()), this, SLOT(switchToShapeEditor()));
}

void ElementEditor::initControlButtonsInteraction(ControlButtons *buttons)
{
	connect(buttons, SIGNAL(iconAccepted()), this, SLOT(switchToRegularEditor()));
	connect(buttons, SIGNAL(iconRejected()), this, SLOT(switchToRegularEditor()));
	connect(buttons, SIGNAL(iconClicked()), this, SLOT(switchToIconEditor()));
	connect(buttons, SIGNAL(shapeClicked()), this, SLOT(switchToShapeEditor()));
	connect(buttons, SIGNAL(widgetClicked()), this, SLOT(switchToWidgetsEditor()));
}

void ElementEditor::initRegularWidgetsEditor()
{
	mWidgetsEditor = new widgetsEdit::WidgetsEditor(false);
	mPages.insert(mWidgetsEditor, mWidgetsEditor);
	initControlButtonsInteraction(mWidgetsEditor->controlButtons());
	connect(mWidgetsEditor, SIGNAL(widgetSaved()), this, SLOT(onWidgetSaved()));
}

void ElementEditor::initRegularShapeEditor()
{
	mShapeEditor = new ShapeEdit(false);
	mPages.insert(mShapeEditor, mShapeEditor);
	initControlButtonsInteraction(mShapeEditor->controlButtons());
	connect(mShapeEditor, SIGNAL(shapeSaved()), this, SLOT(onShapeSaved()));
}

void ElementEditor::initIconWidgetsEditor()
{
	mIconWidgetsEditor = new widgetsEdit::WidgetsEditor(true);
	mPages.insert(mIconWidgetsEditor, mIconWidgetsEditor);
	initControlButtonsInteraction(mIconWidgetsEditor->controlButtons());
}

void ElementEditor::initIconShapeEditor()
{
	mIconShapeEditor = new ShapeEdit(true);
	mPages.insert(mIconShapeEditor, mIconShapeEditor);
	initControlButtonsInteraction(mIconShapeEditor->controlButtons());
}

void ElementEditor::switchToRegularWidgetsEditor()
{
	if (!mWidgetsEditor) {
		initRegularWidgetsEditor();
	}
	mController->switchTo(mWidgetsEditor);
	mDocumentBuilder->setWidgetBased(true);
}

void ElementEditor::switchToRegularShapeEditor()
{
	if (!mShapeEditor) {
		initRegularShapeEditor();
	}
	mController->switchTo(mShapeEditor);
	mDocumentBuilder->setWidgetBased(false);
}

void ElementEditor::switchToIconWidgetsEditor()
{
	if (!mIconWidgetsEditor) {
		initIconWidgetsEditor();
	}
	mController->switchTo(mIconWidgetsEditor);
	mDocumentBuilder->setIconWidgetBased(true);
}

void ElementEditor::switchToIconShapeEditor()
{
	if (!mIconShapeEditor) {
		initIconShapeEditor();
	}
	mController->switchTo(mIconShapeEditor);
	mDocumentBuilder->setIconWidgetBased(false);
}

void ElementEditor::switchToIconEditor()
{
	if (mDocumentBuilder->isIconWidgetBased()) {
		switchToIconWidgetsEditor();
	} else {
		switchToIconShapeEditor();
	}
}

void ElementEditor::switchToWidgetsEditor()
{
	if (isEditingIcon()) {
		switchToIconWidgetsEditor();
	} else {
		switchToRegularWidgetsEditor();
	}
}

void ElementEditor::switchToShapeEditor()
{
	if (isEditingIcon()) {
		switchToIconShapeEditor();
	} else {
		switchToRegularShapeEditor();
	}
}

void ElementEditor::switchToRegularEditor()
{
	if (mDocumentBuilder->isWidgetBased()) {
		switchToRegularWidgetsEditor();
	} else {
		switchToRegularShapeEditor();
	}
}

bool ElementEditor::isEditingIcon() const
{
	NavigationPageInterface *current = currentPage();
	return current == mIconWidgetsEditor || current == mIconShapeEditor;
}

void ElementEditor::load(QString const &data)
{
	QDomDocument document;
	document.setContent(data);
	load(document);
}

void ElementEditor::load(QDomDocument const &data)
{
	if (mDocumentBuilder) {
		delete mDocumentBuilder;
	}
	mDocumentBuilder = TemplateDocumentBuilder::fromDocument(data);
	mController->state()->setState(mDocumentBuilder);
	if (!data.isNull()) {
		switchToRegularEditor();
	}
}

void ElementEditor::onWidgetSaved()
{
	emit widgetSaved(mDocumentBuilder->buildTemplate().toString(4), mIndex, mRole);
}

void ElementEditor::onShapeSaved()
{
	emit shapeSaved(mDocumentBuilder->shape().toString(4), mIndex, mRole);
}
