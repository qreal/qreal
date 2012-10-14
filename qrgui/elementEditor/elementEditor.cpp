#include <QtGui/QGridLayout>

#include "elementEditor.h"
#include "../../qrutils/outFile.h"

using namespace qReal;

ElementEditor::ElementEditor(const QPersistentModelIndex &index
		, const int &role, QWidget *parent)
	: QWidget(parent), mIndex(index), mRole(role)
	, mWidgetBased(false), mLayout(new QStackedLayout)
	, mStartWidget(new QWidget), mWidgetsEditor(NULL)
	, mShapeEditor(NULL), mOpenedFromMetaEditor(true)
{
	initComponents();
}

ElementEditor::ElementEditor(QWidget *parent)
	: QWidget(parent), mRole(0), mWidgetBased(false)
	, mLayout(new QStackedLayout), mStartWidget(new QWidget)
	, mWidgetsEditor(NULL), mShapeEditor(NULL)
	, mOpenedFromMetaEditor(false)
{
	initComponents();
}

ShapeEdit *ElementEditor::shapeEditor() const
{
	return mShapeEditor;
}

widgetsEdit::WidgetsEditor *ElementEditor::widgetEditor() const
{
	return mWidgetsEditor;
}

bool ElementEditor::isWidgetBased() const
{
	return mWidgetBased;
}

void ElementEditor::load(const QString &data)
{
	QDomDocument document;
	document.setContent(data);
	if (document.isNull()) {
		return;
	}
	if (isWidgetBasedDocument(document)) {
		onWidgetBasedButtonClicked();
		mWidgetsEditor->load(document);
	} else {
		onShapeBasedButtonClicked();
		mShapeEditor->load(document);
	}
}

bool ElementEditor::isWidgetBasedDocument(QDomDocument const &document)
{
	QDomElement const graphicsElement = document.firstChild().toElement();
	if (graphicsElement.isNull()) {
		return false;
	}
	return !graphicsElement.firstChildElement("widget-template").isNull();
}

void ElementEditor::initComponents()
{
	setLayout(mLayout);
	initStartWidget();
}

void ElementEditor::initStartWidget()
{
	QPushButton *shapeEditorButton
			= new QPushButton("Create shape-based element");
	connect(shapeEditorButton, SIGNAL(clicked()),
			this, SLOT(onShapeBasedButtonClicked()));
	QPushButton *widgetsEditorButton
			= new QPushButton("Create widget-based element");
	connect(widgetsEditorButton, SIGNAL(clicked())
		, this, SLOT(onWidgetBasedButtonClicked()));
	QGridLayout *startLayout = new QGridLayout;
	startLayout->setMargin(3);
	startLayout->addWidget(shapeEditorButton, 0, 0);
	startLayout->addWidget(widgetsEditorButton, 0, 1);
	mStartWidget->setLayout(startLayout);

	mLayout->addWidget(mStartWidget);
}

void ElementEditor::showWidget(QWidget *widget)
{
	if (mLayout->count() > 1) {
		mLayout->removeWidget(mLayout->widget(mLayout->count()-1));
	}
	mLayout->addWidget(widget);
	mLayout->setCurrentIndex(mLayout->count()-1);
}

void ElementEditor::showShapeEditor()
{
	showWidget(mShapeEditor);
}

void ElementEditor::showWidgetsEditor()
{
	showWidget(mWidgetsEditor);
}

void ElementEditor::onWidgetEditorRequestedShape()
{
	showShapeEditor();
}

void ElementEditor::onWidgetBasedButtonClicked()
{
	mWidgetBased = true;
	if (mOpenedFromMetaEditor) {
		mWidgetsEditor = new widgetsEdit::WidgetsEditor(mIndex, mRole);
		mShapeEditor = new ShapeEdit(mIndex, mRole, true);
	} else {
		mWidgetsEditor = new widgetsEdit::WidgetsEditor;
		mShapeEditor = new ShapeEdit(true);
	}
	connect(mShapeEditor, SIGNAL(shapeSaved(QString,QPersistentModelIndex,int))
			, this, SLOT(onShapeEditorSavedShape(QString,QPersistentModelIndex,int)));
	connect(mShapeEditor, SIGNAL(shapeSaved(QDomDocument))
			, this, SLOT(onShapeEditorSavedShape(QDomDocument)));
	connect(mWidgetsEditor, SIGNAL(shapeRequested())
			, this, SLOT(onWidgetEditorRequestedShape()));
	connect(mWidgetsEditor, SIGNAL(widgetSaved(QString,QPersistentModelIndex,int))
			, this, SLOT(onWidgetEditorSavedShape(QString,QPersistentModelIndex,int)));
	showWidgetsEditor();
}

void ElementEditor::onShapeBasedButtonClicked()
{
	mWidgetBased = false;
	if (mOpenedFromMetaEditor) {
		mShapeEditor = new ShapeEdit(mIndex, mRole);
	} else {
		mShapeEditor = new ShapeEdit;
	}
	connect(mShapeEditor, SIGNAL(shapeSaved(QString,QPersistentModelIndex,int))
			, this, SLOT(onShapeEditorSavedShape(QString,QPersistentModelIndex,int)));
	connect(mShapeEditor, SIGNAL(shapeSaved(QDomDocument))
			, this, SLOT(onShapeEditorSavedShape(QDomDocument)));
	showShapeEditor();
}

void ElementEditor::giveShapeToWidgetsEditor(QDomDocument const &document)
{
	mWidgetsEditor->setShape(document);
	showWidgetsEditor();
}

void ElementEditor::onWidgetEditorSavedShape(const QString &widget
		, const QPersistentModelIndex &index, const int &role)
{
	emit widgetSaved(widget, index, role);
}

void ElementEditor::onShapeEditorSavedShape(const QString &shape
		, const QPersistentModelIndex &index, const int &role)
{
	emit shapeSaved(shape, index, role);
}

void ElementEditor::onShapeEditorSavedShape(const QDomDocument &document)
{
	if (isWidgetBased()) {
		giveShapeToWidgetsEditor(document);
	}
}
